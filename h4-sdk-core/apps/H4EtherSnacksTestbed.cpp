//
// Created by srfas on 10/20/2025.
//

#include <chrono>
#include <iostream>
#include "H4EtherSnacksBoard.h"

int main(int argc, char* argv[]) {
    std::cout << "staring testbed" << std::endl;;

	// Create EtherSnacks board
	H4EtherSnacksBoard h4EtherSnacksBoard("testBoard", true);
	H4EtherSnacksBoard* h4EtherSnacksBoardPtr = &h4EtherSnacksBoard;

    // Create master and add board to it
    Master master("enp89s0");
	master.addSlove(std::unique_ptr<H4EtherSnacksBoard>(h4EtherSnacksBoardPtr));
	// master.addSlove(std::make_unique<H4EtherSnacksBoard>(h4EtherSnacksBoard));
	// master.addSlove(std::make_unique<H4EtherSnacksBoard>("testBoard", true));

	// Initialize master
	master.initialize();

	// Create RT EtherCAT loop thread
	osal_thread_create_rt(&Master::threadrt, 128000, master.ecatthread(), NULL);

	// Create thread to handle slave error handling in OP
	osal_thread_create(&Master::thread1, 128000, master.ecatcheck(), NULL);
}
