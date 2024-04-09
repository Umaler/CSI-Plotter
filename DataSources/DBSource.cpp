#include "DBSource.hpp"

#include <iostream>

DBSource::DBSource(SQLite::Database&& database) :
    db(std::move(database)),

    stopWorker(false),
    workerThread(nullptr,
                 [&](std::thread* thr) {
                    stopWorker = true;
                    if(!thr)
                        return;
                    thr->join();
                    delete thr;
                 }
                 ),

    collectionTypesUpdated(false),

    boundsUpdated(false)
{
    workerGotData.connect([&](){
        signalDataArrived.emit(std::move(commonBuffer));
        commonBuffer.clear();
    });

    threadStoped.connect([&](){
        workerThread.reset();
    });
}

const DBDescriptor& DBSource::getDescriptor() const {
    return dbDescriptor;
}

void DBSource::setBoundaries(Boundaries bounds) {
    std::lock_guard lg(boundsM);
    this->bounds = bounds;
    boundsUpdated = true;

    //workerThread.reset();
    //stopWorker = false;
    //workerThread.reset(new std::thread(&DBSource::worker, this));
}

void DBSource::addCollectionType(std::string table, std::string field) {
    std::lock_guard lg(collectionTypesM);
    collectionTypes.push_back({table, field});
    collectionTypesUpdated = true;

    workerThread.reset();
    stopWorker = false;
    workerThread.reset(new std::thread(&DBSource::worker, this));
}

void DBSource::removeCollectionType(std::string table, std::string field) {
    std::lock_guard lg(collectionTypesM);
    std::pair<std::string, std::string> toDelete{table, field};
    std::remove(collectionTypes.begin(), collectionTypes.end(), toDelete);
    collectionTypesUpdated = true;
}

void DBSource::stopCollection() {
    workerThread.reset();
}

DataSource::SignalType DBSource::signalOnNewDataArrived() const {
    return signalDataArrived;
}

void DBSource::worker() {
    const size_t bufSize = 100;

    auto getQuery = [&](std::string table, std::string field) {
        try {
            std::string request;
            request += "SELECT id, " + field + " FROM " + table + " WHERE id >= ? AND id <= ? AND id_packet >= ? AND id_packet <= ? AND id_measurement >= ? AND id_measurement <= ? AND num_sub >= ? AND num_sub <= ?";

            SQLite::Statement query(db, request);
            query.bind(1, bounds.id.min);     query.bind(2, bounds.id.max);
            query.bind(3, bounds.packId.min); query.bind(4, bounds.packId.max);
            query.bind(5, bounds.measId.min); query.bind(6, bounds.measId.max);
            query.bind(7, bounds.numSub.min); query.bind(8, bounds.numSub.max);

            return query;
        }
        catch(std::exception& ex) {
            std::cerr << ex.what() << std::endl;
        }
        return SQLite::Statement(db, "");
    };

    collectionTypesM.lock();
    std::vector<SQLite::Statement> queries;
    queries.reserve(collectionTypes.size());
    for(const auto& i : collectionTypes) {
        queries.push_back(getQuery(i.first, i.second));
    }
    collectionTypesM.unlock();
    std::vector<bool> correctQueries(queries.size(), true);
    size_t validQueries = queries.size();

    std::vector<std::vector<std::pair<double, double>>> localBuffer;

    while (!stopWorker) {
        localBuffer.resize(queries.size());
        for(size_t i = 0; i < queries.size(); i++) {
            if(!correctQueries[i])
                continue;

            auto& query = queries[i];
            for(size_t j = 0; j < bufSize; j++) {
                try {
                    if(!query.executeStep()) {
                        correctQueries[i] = false;
                        validQueries--;
                        break;
                    }
                    unsigned long long id = query.getColumn(0).getInt64();
                    double value = query.getColumn(1);
                    localBuffer[i].push_back({id, value});
                }
                catch(std::exception& ex) {
                    std::cerr << ex.what() << std::endl;
                    correctQueries[i] = false;
                    validQueries--;
                    break;
                }
            }
        }

        bool commonBufferFree = false;
        while(!commonBufferFree && !stopWorker) {
            commonBufferM.lock();
            commonBufferFree = commonBuffer.empty();
            if(commonBufferFree) {
                commonBuffer = localBuffer;
                localBuffer.clear();
                workerGotData.emit();
            }
            commonBufferM.unlock();
        }

        if(validQueries == 0)
            break;
    }

    threadStoped.emit();
}
