#pragma once

#include "DataSource.hpp"
#include <atomic>
#include <array>
#include <mutex>
#include <thread>
#include <glibmm/dispatcher.h>

class RTSource : public DataSource {
public:
    RTSource();

    virtual const DBDescriptor& getDescriptor() const;

    virtual void setBoundaries(Boundaries bounds);
    virtual void addCollectionType(std::string table, std::string field);
    virtual void removeCollectionType(std::string table, std::string field);
    virtual void removeAllCollectionTypes();
    virtual void stopCollection();

    virtual DataSource::SignalType signalOnNewDataArrived() const;

    virtual ~RTSource() = default;

private:
    void newDataArrived();

    std::mutex filtersM;
    std::atomic<bool> filtersUpdated;
    Boundaries bounds;
    std::string table, field;

    std::mutex commonBufM;
    std::vector<std::vector<std::pair<double, double>>> commonBuf;

    DataSource::SignalType newDataArrivedSignal;
    Glib::Dispatcher newDataCollected;

    const unsigned int port = 40055;

    class Worker {
    public:
        Worker(RTSource& par, unsigned int p);

        void start();
        void stop();

    private:
        struct DataSlice {
            double amps[3][3];
            double phas[3][3];
        };

        RTSource& parent;
        const unsigned int port;
        std::unique_ptr<std::thread, std::function<void(std::thread*)>> workerThread;
        std::atomic<bool> pauseTransfer;
        std::atomic<bool> stopWorker;
        void work();
    } worker;

    inline static const DBDescriptor desc
    {
        {
            "amplitude",
            {
                "11",
                "12",
                "13",
                "21",
                "22",
                "23",
                "31",
                "32",
                "33"
            }
        },
        {
            "phase",
            {
                "11",
                "12",
                "13",
                "21",
                "22",
                "23",
                "31",
                "32",
                "33"
            }
        }
    };

};
