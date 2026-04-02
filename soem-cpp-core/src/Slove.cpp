//
// Created by srfas on 3/25/2026.
//

#include "../include/Slove.h"

Slove::Slove(int vendorID, int productCode, int alias, int position, std::size_t ioMapSize) : vendorID(vendorID),
    productCode(productCode), alias(alias), position(position), ioMapSize(ioMapSize), IOMap{ioMapSize}
{
}

SyncManager& Slove::createAndRegisterSyncManager(SyncManager::SyncManagerType syncManagerType)
{
    return syncManagers.emplace_back(syncManagerType);
}

void Slove::linkBuffers(const std::vector<std::byte>& masterIOMap, BufferOffsetHolder& inputOffsetHolder,
                        BufferOffsetHolder& outputOffsetHolder)
{
    for (int i = 0; i < syncManagers.size(); i++)
    {
        if (syncManagers[i].getSyncManagerType() == SyncManager::TXPDO)
            syncManagers[i].linkBuffers(masterIOMap, inputOffsetHolder);
        else if (syncManagers[i].getSyncManagerType() == SyncManager::RXPDO)
            syncManagers[i].linkBuffers(masterIOMap, outputOffsetHolder);
    }

    // if (inputOffsetHolder.getAvailableBits() != 0) //TODO add warning printouts
    // if (outputOffsetHolder.getAvailableBits() != 0) //TODO add warning printouts
    //TODO maybe surround whole thing in try/catch?
}
