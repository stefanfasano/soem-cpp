//
// Created by srfas on 10/20/2025.
//

#include <chrono>
#include <iostream>
#include "H4EtherSnacksBoard.h"

int main(int argc, char* argv[]) {
    std::cout << "staring testbed" << std::endl;

	// EtherCat interface name (change to match yours)
	const string etherCatInterface = "enp89s0";

	// Create slave
	H4EtherSnacksBoard h4EtherSnacksBoard("testBoard", true);

    // Create master and pass in slave
    Master master(etherCatInterface, h4EtherSnacksBoard);

	// Initialize master
	master.initialize();

	// create RT EtherCAT thread
	osal_thread_create_rt(&Master::threadrt, 128000, master.ecatthread(), NULL);

	// create thread to handle slave error handling in OP
	osal_thread_create(&Master::thread1, 128000, master.ecatcheck(), NULL);
}
