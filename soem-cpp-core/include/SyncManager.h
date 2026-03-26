//
// Created by srfas on 3/23/2026.
//

#ifndef SOEM_CPP_SYNCMANAGER_H
#define SOEM_CPP_SYNCMANAGER_H
#include <vector>


class SyncManager {

public:
    // The four possible Sync Manager types
    enum SyncManagerType {
        ASYNC_OUT,
        ASYNC_IN,
        TXPDO,
        RXPDO
    };

private:
    SyncManagerType syncManagerType;
    int pdoAssignIndex;

public:
    explicit SyncManager(SyncManagerType syncManagerType);
};


#endif //SOEM_CPP_SYNCMANAGER_H