//
// Created by srfas on 3/23/2026.
//

#ifndef SOEM_CPP_SYNCMANAGER_H
#define SOEM_CPP_SYNCMANAGER_H

#include <memory>
#include <vector>
#include "PDO.h"

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
    std::vector<std::unique_ptr<PDO>> pdos;

public:
    explicit SyncManager(const SyncManagerType& syncManagerType);

    [[nodiscard]] int getPdoAssignIndex() const { return pdoAssignIndex; }

    [[nodiscard]] const SyncManagerType& getSyncManagerType() const { return syncManagerType; }

    void registerPDO(PDO& pdo);
};


#endif //SOEM_CPP_SYNCMANAGER_H