//
// Created by srfas on 12/13/2025.
//

#ifndef H4_SDK_MASTER_H
#define H4_SDK_MASTER_H

#include <cstdint>
#include <string>
#include <array>
#include <list>
#include <memory>
#include <vector>

#include "Slove.h"
#include "soem/soem.h"

static int currentgroup = 0;
static int cycle = 0;
static float pgain = 0.01f;
static float igain = 0.00002f;
/* set linux sync point 500us later than DC sync, just as example */
static int64 syncoffset = 500000;

class Master {

    public:

    static constexpr std::uint16_t EC_TIMEOUTMON = 500;
    static constexpr std::uint32_t NSEC_PER_SEC = 1000000000;
    static constexpr std::int64_t cycletime = 1000000;

    static constexpr std::uint16_t DEFAULT_IOMAP_SIZE = 4096;
    // static std::uint8_t IOmap[DEFAULT_IOMAP_SIZE];
    static OSAL_THREAD_HANDLE threadrt, thread1;
    static int expectedWKC;
    static int wkc;
    static int mappingdone, dorun, inOP, dowkccheck;

    static int64 timeerror;

    const std::string ifName;
    static ecx_contextt ctx;

    std::vector<std::unique_ptr<Slove>> registeredSloves ;

    explicit Master(const std::string& ifName);

    void initialize() const;

    void registerSlove(std::unique_ptr<Slove> slove);

    void *ecatthread();

    void *ecatcheck();

    void ec_sync(int64 reftime, int64 cycletime, int64 *offsettime);

    void add_time_ns(ec_timet *ts, int64 addtime);

    int getProcessDataSize(const ec_slavet& ec_slave) const;

    [[nodiscard]] Slove* getRegisteredSlove(int alias, int position) const;

    static int calculateExpectedWorkingCounter();

    static int getCurrentWorkingCounter();

    ~Master() = default;
};

#endif //H4_SDK_MASTER_H
