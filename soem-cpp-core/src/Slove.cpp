//
// Created by srfas on 3/25/2026.
//

#include "../include/Slove.h"

Slove::Slove(const int &vendorID, const int &productCode, const int &alias, const int &position, const std::size_t ioMapSize) : vendorID(vendorID),
        productCode(productCode), alias(alias), position(position), ioMapSize(ioMapSize), IOMap{ioMapSize}{}

void Slove::registerSyncManager(SyncManager::SyncManagerType syncManagerTyoe) {
        syncManagers.emplace_back(SyncManager(syncManagerTyoe));
}
