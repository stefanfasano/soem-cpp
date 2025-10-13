//
// Created by srfas on 9/6/2025.
//

#include "h4_ethersnacks_board.h"


#include "abstract_motor_controller.h"

h4_ethersnacks_board::h4_ethersnacks_board(const string& name, const abstract_imu& abstract_imu_) : name(name), abstract_imu_(name) {}