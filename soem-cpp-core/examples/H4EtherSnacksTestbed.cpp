//
// Created by srfas on 10/20/2025.
//

#include <chrono>
#include <iostream>
#include "H4EtherSnacksBoard.h"
#include "../include/Master.h"

int main(int argc, char* argv[]) {
    std::cout << "staring testbed" << std::endl;

	// EtherCat interface name (change to match yours)
	const string etherCatInterface = "enp89s0";

	// Create EtherSnacks board
	H4EtherSnacksBoard h4EtherSnacksBoard(6969, 0, "testBoard", true);
	H4EtherSnacksBoard* h4EtherSnacksBoardPtr = &h4EtherSnacksBoard;

	// Create master and add board to it
	Master master("enp89s0");
	master.registerSlove(h4EtherSnacksBoard);
	// master.registerSlove(std::unique_ptr<H4EtherSnacksBoard>(h4EtherSnacksBoardPtr));
	// master.addSlove(std::make_unique<H4EtherSnacksBoard>(h4EtherSnacksBoard));
	// master.addSlove(std::make_unique<H4EtherSnacksBoard>("testBoard", true));

	// Initialize master
	master.initialize();

	// create RT EtherCAT thread
	osal_thread_create_rt(&Master::threadrt, 128000, master.ecatthread(), NULL);

	// create thread to handle slave error handling in OP
	osal_thread_create(&Master::thread1, 128000, master.ecatcheck(), NULL);
}
