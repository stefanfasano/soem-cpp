//
// Created by srfas on 12/13/2025.
//

#ifndef H4_SDK_MASTER_H
#define H4_SDK_MASTER_H

#include <string>

#include "soem/soem.h"

static int currentgroup = 0;
static int cycle = 0;
static float pgain = 0.01f;
static float igain = 0.00002f;
/* set linux sync point 500us later than DC sync, just as example */
static int64 syncoffset = 500000;

class Master {

    public:

    static constexpr uint16 EC_TIMEOUTMON = 500;
    static constexpr uint32 NSEC_PER_SEC = 1000000000;
    static constexpr int64_t cycletime = 1000000;

    static constexpr uint16 IOMAP_SIZE = 4096;
    static uint8 IOmap[IOMAP_SIZE];
    static OSAL_THREAD_HANDLE threadrt, thread1;
    static int expectedWKC;
    static int wkc;
    static int mappingdone, dorun, inOP, dowkccheck;

    static int64 timeerror;

    const std::string ifName;
    static ecx_contextt ctx;

    explicit Master(const std::string& ifName);

    void initialize();

    ~Master() = default;
};

#endif //H4_SDK_MASTER_H