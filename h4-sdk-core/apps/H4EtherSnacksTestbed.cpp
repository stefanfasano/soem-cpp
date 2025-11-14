//
// Created by srfas on 10/20/2025.
//

#include <ethercatcpp/core.h>
#include <pid/synchro.h>
#include <pid/real_time.h>
#include <pid/signal_manager.h>
#include <pid/log.h>
#include <chrono>

#include "H4EtherSnacksBoard.h"
#include "H4IMU.h"

using std::chrono_literals::operator ""ms;

int main(int argc, char* argv[]) {
    //auto memory_locker = pid::make_current_thread_real_time();

    // Master creation
    ethercatcpp::Master master;

    // Adding network interface (replace by adequet id)
    master.set_primary_interface("enp89s0");

    // Device definition
    H4EtherSnacksBoard h4EtherSnacksBoard("h4EtherSnacksBoard", true);

    // Linking device to bus in hardware order !!
    master.add(h4EtherSnacksBoard);

    // Initilize the network master
    master.init();

    bool stop = false;
    pid::SignalManager::add(pid::SignalManager::Interrupt, "SigInt stop",
                            [&stop]() { stop = true; });

    //const auto period = std::chrono::duration<double>(control_period);
    pid::Period period(1ms);

    pid_log << "Starting periodic loop" << pid::endl;
    while (not stop) {

        // If cycle is correct read data
        if (master.next_cycle()) {
            h4EtherSnacksBoard.print();
        }

        period.sleep();
    }

    //Cleanly terminate communication
    pid::SignalManager::remove(pid::SignalManager::Interrupt, "SigInt stop");
    master.end();
}