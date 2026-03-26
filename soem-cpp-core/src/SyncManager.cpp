//
// Created by srfas on 3/23/2026.
//

#include "../include/SyncManager.h"

SyncManager::SyncManager(SyncManagerType syncManagerType): syncManagerType(syncManagerType)
{
    switch (syncManagerType)
    {
        case ASYNC_OUT: pdoAssignIndex = 0x1c10; break;
        case ASYNC_IN: pdoAssignIndex = 0x1c11; break;
        case RXPDO: pdoAssignIndex = 0x1c12; break;
        case TXPDO: pdoAssignIndex = 0x1c13; break;
        default: pdoAssignIndex = 0; break;
    }
}