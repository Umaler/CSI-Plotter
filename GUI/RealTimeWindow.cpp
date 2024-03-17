#include "RealTimeWindow.hpp"

extern "C" {
    #include "../csi_fun.h"
}

#include <SFML/Network.hpp>
#include <iostream>

RealTimeWindow::RealTimeWindow() :
    plotData(std::vector<double>{}, std::vector<double>{}),
    plot(plotData, "ID", "Value"),
    canvas(),

    toStopCatcher(false),
    dataCatcherThread(nullptr,
                      [&] (std::thread* t) -> void {
                          toStopCatcher.store(true);
                          t->join();
                          delete t;
                      }
                     )
{
    canvas.set_expand();
    canvas.add_plot(plot);

    set_child(canvas);
    newDataCollected.connect(sigc::mem_fun(*this, &RealTimeWindow::onNewDataArrived));

    dataCatcherThread.reset(new std::thread(&RealTimeWindow::dataCatcher, this));
}

void RealTimeWindow::onNewDataArrived() {
    static size_t lastId = 0;

    std::lock_guard<std::mutex> lg(commonBufferM);

    while(!commonBuffer.empty()) {
        double val = commonBuffer.front();
        commonBuffer.pop();

        plotData.add_datapoint(lastId, val);
        lastId++;
    }

    canvas.queue_draw();
}

void RealTimeWindow::dataCatcher() {
    std::queue<double> localBuffer;

    constexpr size_t rawBufSize = 100000;

    const unsigned int port = 5505;

    sf::UdpSocket socket;
    socket.setBlocking(false);

    // Listen to messages on the specified port
    if (socket.bind(port) != sf::Socket::Status::Done)
        return;

    unsigned char                in[rawBufSize];
    std::size_t                  received;
    sf::IpAddress                sender;
    unsigned short               senderPort;

    COMPLEX csi_matrix[3][3][114];
    csi_struct csi_status;
    while (!toStopCatcher.load()) {
        sf::Socket::Status status = socket.receive(in, sizeof(in), received, sender, senderPort);
        if(status != sf::Socket::Status::Done)
            continue;

        record_status(in, received, &csi_status);

        unsigned char* data_buf = new unsigned char[csi_status.payload_len];
        record_csi_payload(in, &csi_status, data_buf, csi_matrix);

        for(unsigned i = 0; i < 114 && i < csi_status.num_tones; i++) {
            localBuffer.push(std::sqrt(csi_matrix[0][0][i].imag * csi_matrix[0][0][i].imag + csi_matrix[0][0][i].real * csi_matrix[0][0][i].real));
        }

        delete [] data_buf;

        commonBufferM.lock();
        std::swap(commonBuffer, localBuffer);
        commonBufferM.unlock();
        newDataCollected.emit();
    }
}
