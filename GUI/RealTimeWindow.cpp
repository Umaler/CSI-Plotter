#include "RealTimeWindow.hpp"

extern "C" {
    #include "../csi_fun.h"
}

#include <SFML/Network.hpp>
#include <iostream>

RealTimeWindow::RealTimeWindow() :
    plot("ID", "Value"),
    canvas(),
    fieldChooser(rtDescriptor),

    subcarrAdj(Gtk::Adjustment::create(0, 0, std::numeric_limits<int64_t>::max(), 1, 1)),
    subcarrSpinButton(subcarrAdj),
    subcarrFrame("Поднесущая"),

    toStopCatcher(false),
    dataCatcherThread(nullptr,
                      [&] (std::thread* t) -> void {
                          toStopCatcher.store(true);
                          t->join();
                          delete t;
                      }
                     )
{
    set_default_size(1280, 920);

    canvas.set_expand();
    canvas.add_plot(plot);

    //subcarrFrame.set_chil

    set_child(grid);
    grid.attach(canvas, 1, 0);
    grid.attach(fieldChooser, 0, 0, 1, 2);
    grid.attach(subcarrFrame, 1, 1);
    fieldChooser.signalOnChoosed().connect(sigc::mem_fun(*this, &RealTimeWindow::onSelectedSource));

    newDataCollected.connect(sigc::mem_fun(*this, &RealTimeWindow::onNewDataArrived));

    dataCatcherThread.reset(new std::thread(&RealTimeWindow::dataCatcher, this));
}

void RealTimeWindow::onNewDataArrived() {
    static size_t lastId = 0;

    std::lock_guard<std::mutex> lg(commonBufferM);

    while(!commonBuffer.empty()) {
        DataSlice val = commonBuffer.front();
        commonBuffer.pop();


        /*for(unsigned i = 0; i < 3; i++) {
            for(unsigned j = 0; j < 3; j++) {
                for(unsigned k = 0; k < 144 && k < commonBuffer.size() + 1; k++) {
                    amps[i][j][k].add_datapoint(lastId, val.amps[i][j]);
                    phas[i][j][k].add_datapoint(lastId, val.phas[i][j]);
                }
            }
        }*/

        //lastId++;
    }

    canvas.queue_draw();
}

void RealTimeWindow::onSelectedSource(Glib::ustring table, Glib::ustring field) {
    if(isSmthPlotted)
        plot.remove_data(0);
    else
        isSmthPlotted = true;

    /*if(table == "Amplitude") {
        plot.add_data(amps[field[0] - '1'][field[1] - '1'][]);
    }
    else {
        plot.add_data(phas[field[0] - '1'][field[1] - '1'][]);
    }*/
}

void RealTimeWindow::dataCatcher() {
    std::queue<DataSlice> localBuffer;

    constexpr size_t rawBufSize = 65535;    //maximal size of UDP datagram

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

        if(csi_status.payload_len < 1056) {
            continue;
        }

        unsigned char* data_buf = new unsigned char[csi_status.payload_len];
        record_csi_payload(in, &csi_status, data_buf, csi_matrix);

        for(unsigned i = 0; i < 114 && i < csi_status.num_tones; i++) {
            DataSlice slice;
            for(unsigned j = 0; j < 3; j++) {
                for(unsigned k = 0; k < 3; k++) {
                    slice.amps[j][k] = std::sqrt(csi_matrix[j][k][i].imag * csi_matrix[j][k][i].imag + csi_matrix[j][k][i].real * csi_matrix[j][k][i].real);
                    slice.phas[j][k] = std::atan(static_cast<double>(csi_matrix[j][k][i].imag) / static_cast<double>(csi_matrix[j][k][i].real));
                    slice.sub_carr_id = i;
                }
            }
            localBuffer.push(slice);
        }

        commonBufferM.lock();
        std::swap(commonBuffer, localBuffer);
        commonBufferM.unlock();
        newDataCollected.emit();

        delete [] data_buf;
    }
}
