//
// Created by srfas on 3/25/2026.
//

#include "../include/Slove.h"

Slove::Slove(const int &vendorID, const int &productCode, const int &alias, const int &position, const std::size_t ioMapSize) : vendorID(vendorID),
        productCode(productCode), alias(alias), position(position), ioMapSize(ioMapSize), IOMap{ioMapSize}{}

SyncManager& Slove::createAndRegisterSyncManager(SyncManager::SyncManagerType syncManagerType) {
        return syncManagers.emplace_back(syncManagerType);
}
