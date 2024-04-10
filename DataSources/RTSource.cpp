#include "RTSource.hpp"

extern "C" {
    #include "../csi_fun.h"
}

#include <SFML/Network.hpp>
#include <iostream>
#include <cmath>

RTSource::RTSource() :
    filtersUpdated(false),
    worker(*this, port)
{
    newDataCollected.connect([&](){
        std::lock_guard lg(commonBufM);
        newDataArrivedSignal.emit(std::move(commonBuf));
        commonBuf.clear();
    });
}

const DBDescriptor& RTSource::getDescriptor() const {
    return desc;
}

void RTSource::setBoundaries(Boundaries bounds) {
    std::lock_guard lg(filtersM);
    this->bounds = bounds;
    filtersUpdated = true;
}

void RTSource::addCollectionType(std::string table, std::string field) {
    std::lock_guard lg(filtersM);
    this->table = table;
    this->field = field;
    filtersUpdated = true;

    worker.start();
}

void RTSource::removeCollectionType(std::string table, std::string field) {
    removeAllCollectionTypes();
}

void RTSource::removeAllCollectionTypes() {
    worker.stop();
}

void RTSource::stopCollection() {
    worker.stop();
}

DataSource::SignalType RTSource::signalOnNewDataArrived() const {
    return newDataArrivedSignal;
}

void RTSource::newDataArrived() {
    std::lock_guard lg(commonBufM);
    newDataArrivedSignal.emit(std::move(commonBuf));
    commonBuf.clear();
}

RTSource::Worker::Worker(RTSource& par, unsigned int p) :
    parent(par),
    port(p),
    pauseTransfer(true),
    stopWorker(false),

    workerThread(new std::thread(&RTSource::Worker::work, this),
                 [&](std::thread* thr) {
                    if(!thr)
                        return;
                    stopWorker = true;
                    thr->join();
                    stopWorker = false;
                 }
                 )
{

}

void RTSource::Worker::start() {
    pauseTransfer = false;
}

void RTSource::Worker::stop() {
    pauseTransfer = true;
}

void RTSource::Worker::work() {
    constexpr size_t maxSubcars = 114;
    std::list<std::array<DataSlice, maxSubcars>> allCollectedData;

    const size_t rawBufSize = sf::UdpSocket::MaxDatagramSize;    //maximal size of UDP datagram

    sf::UdpSocket socket;
    socket.setBlocking(false);

    // Listen to messages on the specified port
    if (socket.bind(port) != sf::Socket::Status::Done)
        return;

    unsigned char                in[rawBufSize];
    std::size_t                  received = 0;
    sf::IpAddress                sender;
    unsigned short               senderPort;

    Boundaries boundsToTransfer;
    size_t tx = 0, rx = 0;
    enum class DataType {
        amps,
        phas
    } dataToTransfer;

    COMPLEX csi_matrix[3][3][maxSubcars];
    csi_struct csi_status;
    while (!stopWorker) {
        std::vector<std::vector<std::pair<double, double>>> bufferToTransfer;
        bufferToTransfer.resize(1);
        if(parent.filtersUpdated) {
            { // update local copy of filters
                std::lock_guard lg(parent.filtersM);
                boundsToTransfer = parent.bounds;
                tx = parent.field[0] - '1';
                rx = parent.field[1] - '1';
                if(parent.table == "amplitude") {
                    dataToTransfer = DataType::amps;
                }
                else if (parent.table == "phase") {
                    dataToTransfer = DataType::phas;
                }
                else {
                    pauseTransfer = true;
                }
            }


            int64_t packid = boundsToTransfer.packId.min >= 0 ? boundsToTransfer.packId.min : 0;
            for(auto it = allCollectedData.begin();
                packid < boundsToTransfer.packId.max && it != allCollectedData.end();
                packid++, ++it)
            {
                for(int64_t subcarr = (boundsToTransfer.numSub.min >= 0 ? boundsToTransfer.numSub.min : 0);
                    subcarr < maxSubcars && subcarr < boundsToTransfer.numSub.max;
                    subcarr++)
                {
                    int64_t id = packid * maxSubcars + subcarr;
                    if(id < boundsToTransfer.id.min || id > boundsToTransfer.id.max)
                        continue;
                    if(dataToTransfer == DataType::amps) {
                        bufferToTransfer[0].push_back({id, (*it)[subcarr].amps[tx][rx]});
                    }
                    else {
                        bufferToTransfer[0].push_back({id, (*it)[subcarr].phas[tx][rx]});
                    }
                }
            }

            parent.filtersUpdated = false;
        }

        sf::Socket::Status status = socket.receive(in, sizeof(in), received, sender, senderPort);
        if(status != sf::Socket::Status::Done)
            continue;

        record_status(in, received, &csi_status);

        if(csi_status.payload_len < 1056) {
            continue;
        }

        unsigned char* data_buf = new unsigned char[csi_status.payload_len];
        record_csi_payload(in, &csi_status, data_buf, csi_matrix);

        allCollectedData.push_back({});
        DataSlice slice;
        for(unsigned i = 0; i < csi_status.num_tones; i++) {
            for(unsigned j = 0; j < 3; j++) {
                for(unsigned k = 0; k < 3; k++) {
                    slice.amps[j][k] = std::sqrt(csi_matrix[j][k][i].imag * csi_matrix[j][k][i].imag + csi_matrix[j][k][i].real * csi_matrix[j][k][i].real);
                    slice.phas[j][k] = std::atan(static_cast<double>(csi_matrix[j][k][i].imag) / static_cast<double>(csi_matrix[j][k][i].real));
                }
            }
            allCollectedData.back()[i] = slice;
            if(allCollectedData.size() >= boundsToTransfer.packId.min && allCollectedData.size() <= boundsToTransfer.packId.max &&
               allCollectedData.size() * maxSubcars >= boundsToTransfer.id.min && allCollectedData.size() * maxSubcars <= boundsToTransfer.id.max &&
               i >= boundsToTransfer.numSub.min && i <= boundsToTransfer.numSub.max
               )
            {
                size_t x;
                if(allCollectedData.size() < csi_status.num_tones) {
                    x = i;
                }
                else {
                    x = allCollectedData.size() - csi_status.num_tones + i;
                }
                if(dataToTransfer == DataType::amps)
                    bufferToTransfer[0].push_back({x, slice.amps[tx][rx]});
                else
                    bufferToTransfer[0].push_back({x, slice.phas[tx][rx]});
            }
        }

        if(pauseTransfer) {
            delete [] data_buf;
            continue;
        }

        while(true) {
            std::lock_guard lg(parent.commonBufM);
            if(parent.commonBuf.empty()) {
                std::swap(parent.commonBuf, bufferToTransfer);
                parent.newDataCollected.emit();
                break;
            }
        }

        delete [] data_buf;
    }
}
