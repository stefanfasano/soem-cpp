//
// Created by srfas on 10/9/2025.
//

#include <ethercatcpp/core.h>
#include <pid/synchro.h>
#include <pid/real_time.h>
#include <pid/signal_manager.h>
#include <pid/log.h>
#include <chrono>
#include "H4IMU.h"

using std::chrono_literals::operator ""ms;

int main(int argc, char* argv[]) {
    // auto memory_locker = pid::make_current_thread_real_time();

    // Master creation
    ethercatcpp::Master master;

    // Adding network interface (replace by adequet id)
    master.set_primary_interface("enp89s0");

    // Device definition
    // ethercatcpp::EK1100 EK1100;
    // ethercatcpp::EL5101 EL5101;
    H4IMU h4IMU("h4IMU", 0);

    // Linking device to bus in hardware order !!
    // master.add(EK1100);
    // master.add(EL5101);
    master.add(h4IMU);

    // Initilize the network master
    master.init();

    bool stop = false;
    pid::SignalManager::add(pid::SignalManager::Interrupt, "SigInt stop",
                            [&stop]() { stop = true; });

    //const auto period = std::chrono::duration<double>(control_period);
    pid::Period period(1ms);

    pid_log << "Starting periodic loop" << pid::endl;
    while (not stop) {

        // SET config => set the command buffer
        // EL5101.enableenable_Latch_On(ethercatcpp::EL5101::latch_pin_C, false);
        // EL5101.enable_Latch_On(ethercatcpp::EL5101::latch_pin_ext_pos, false);
        // EL5101.enable_Latch_On(ethercatcpp::EL5101::latch_pin_ext_neg, false);
        // EL5101.enable_Counter_offset(false);
        // EL5101.set_Counter_Offset_Value(0);

        // If cycle is correct read data
        if (master.next_cycle()) {
            h4IMU.print();
        }

        period.sleep();
    }

    //Cleanly terminate communication
    pid::SignalManager::remove(pid::SignalManager::Interrupt, "SigInt stop");
    master.end();
}