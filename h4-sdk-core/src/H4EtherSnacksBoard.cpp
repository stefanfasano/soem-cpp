//
// Created by srfas on 9/6/2025.
//

#include "H4EtherSnacksBoard.h"
#include "H4EtherSnacksBoard.h"
#include "H4EtherSnacksBoard.h"
#include <iostream>

H4EtherSnacksBoard::H4EtherSnacksBoard(const string& name, bool addImu) : name(name), abstractIMU(name + "_imu")
{

}

void H4EtherSnacksBoard::read() {
    std::cout << "---attempting to uppack status buffer---" << std::endl;

    abstractIMU.setPosition(0.0, 0.0, 0.0);
    abstractIMU.setQuaternion(0.0, 0.0, 0.0, 0.0);
    abstractIMU.setAngularVelocity(pd->inputs.gyroX * RAW_GYRO_TO_RAD_PER_SEC, pd->inputs.gyroY * RAW_GYRO_TO_RAD_PER_SEC, pd->inputs.gyroZ * RAW_GYRO_TO_RAD_PER_SEC);
    abstractIMU.setLinearAcceleration(pd->inputs.accelX * RAW_ACCEL_TO_RAD_PER_SEC_PER_SEC, pd->inputs.accelY * RAW_ACCEL_TO_RAD_PER_SEC_PER_SEC, pd->inputs.accelZ * RAW_ACCEL_TO_RAD_PER_SEC_PER_SEC);
    abstractIMU.setIMUTemp(pd->inputs.imuTemp * RAW_TEMP_TO_CELCIUS_SCALAR + RAW_TEMP_TO_CELCIUS_CONSTANT);
    //abstract_imu_.setBoardTemp(buffer->sensorTemp * RAW_TEMP_TO_CELCIUS_SCALAR + RAW_TEMP_TO_CELCIUS_CONSTANT);
}

void H4EtherSnacksBoard::print() {
    abstractIMU.print();
}

AbstractIMU H4EtherSnacksBoard::get_imu() {
    return abstractIMU;
}

string H4EtherSnacksBoard::get_name() {
    return name;
}
