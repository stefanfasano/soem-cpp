//
// Created by srfas on 9/6/2025.
//

#include "h4_imu.h"
#include <string>

h4_imu::h4_imu(const std::string &name, const uint32_t &address)
: abstract_imu(name)
{
    // pdoInfo_.rxPdoId_ = RX_PDO_ID;
    // pdoInfo_.txPdoId_ = TX_PDO_ID;
    // pdoInfo_.rxPdoSize_ = sizeof(command_);
    // pdoInfo_.txPdoSize_ = sizeof(reading_);
    // pdoInfo_.moduleId_ = PRODUCT_CODE;
}

// bool h4_imu::startup()
// {
//     // Do nothing else
//     return true;
// }

void h4_imu::update()
{
    // updateRead();

    setPosition(0.0, 0.0, 0.0);
    setQuaternion(0.0, 0.0, 0.0, 0.0);
    setAngularVelocity(reading_.gyroX * RAW_GYRO_TO_RAD_PER_SEC, reading_.gyroY * RAW_GYRO_TO_RAD_PER_SEC, reading_.gyroZ * RAW_GYRO_TO_RAD_PER_SEC);
    setLinearAcceleration(reading_.accelX * RAW_ACCEL_TO_RAD_PER_SEC_PER_SEC, reading_.accelY * RAW_ACCEL_TO_RAD_PER_SEC_PER_SEC, reading_.accelZ * RAW_ACCEL_TO_RAD_PER_SEC_PER_SEC);

    // updateWrite();
}

// void h4_imu::updateRead()
// {
//     bus_->readTxPdo(address_, reading_);
// }
//
// void h4_imu::updateWrite()
// {
//     bus_->writeRxPdo(address_, command_);
// }
//
// void h4_imu::shutdown()
// {
//     // Do nothing
// }
