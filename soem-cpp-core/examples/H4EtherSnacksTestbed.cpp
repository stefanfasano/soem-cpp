//
// Created by srfas on 10/20/2025.
//

#include <chrono>
#include <iostream>
#include "H4EtherSnacksBoard.h"

// using std::chrono_literals::operator ""ms;

int main(int argc, char* argv[]) {
    //auto memory_locker = pid::make_current_thread_real_time();
    std::cout << "staring testbed" << std::endl;;

	// Create slave
	H4EtherSnacksBoard h4EtherSnacksBoard("testBoard", true);

    // Create master and pass in slave
    Master master("enp89s0", h4EtherSnacksBoard);

	// Initialize master
	master.initialize();

	// create RT EtherCAT thread
	osal_thread_create_rt(&Master::threadrt, 128000, master.ecatthread(), NULL);

	// create thread to handle slave error handling in OP
	osal_thread_create(&Master::thread1, 128000, master.ecatcheck(), NULL);
}
