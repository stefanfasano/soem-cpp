//
// Created by srfas on 9/6/2025.
//

#include "H4EtherSnacksBoard.h"
#include "H4EtherSnacksBoard.h"
#include "H4EtherSnacksBoard.h"
#include <iostream>

H4EtherSnacksBoard::H4EtherSnacksBoard(const string& name, bool addImu) : name(name), abstractIMU(name + "_imu")
{
    set_id(name, VENDOR_ID, PRODUCT_CODE);

    std::cout << "---size of buffer out:---"<< std::endl;
    std::cout << sizeof(buffer_out_cyclic_command_t)<< std::endl;

    std::cout << "---align of buffer out:---"<< std::endl;
    std::cout << alignof(buffer_out_cyclic_command_t)<< std::endl;

    std::cout << "---size of buffer in:---"<< std::endl;
    std::cout << sizeof(buffer_in_cyclic_status_t)<< std::endl;

    std::cout << "---align of buffer in:---"<< std::endl;
    std::cout << alignof(buffer_in_cyclic_status_t)<< std::endl;

    configure_at_init( [this]()
    {
    // Link maps
    // Config Command PDO mapping (Rx PDO assign: 0x1c12)
    start_command_pdo_mapping<uint8_t>();
    add_command_pdo_mapping<uint8_t>(0x1600); //Assign IO Map at CoE index 0x1600 to Rx PDO in 0x1c12.
    end_command_pdo_mapping<uint8_t>();

    // Config Status PDO mapping (Tx PDO assign: 0x1c13)
    start_status_pdo_mapping<uint8_t>();
    add_status_pdo_mapping<uint8_t>(0x1a00);  //Assign IO Map at CoE index 0x1A00 to Tx PDO in 0x1c13.
    end_status_pdo_mapping<uint8_t>();

    std::cout << "---finished configure at init---"<< std::endl;
    });

    // Communication buffer configuration (RxPDO / TxPDO)
    define_physical_buffer<buffer_out_cyclic_command_t>(SYNCHROS_OUT, 0x1100, 0x00010064); //TODO make sure these are right
    define_physical_buffer<buffer_in_cyclic_status_t>(SYNCHROS_IN, 0x1400, 0x00010020);

    std::cout << "---defined physical buffers---"<< std::endl;

    // Decide whether to use a distributed clock
    define_distributed_clock(false);

    add_init_step( [this]() { update_command_buffer(); },
		   [this]() { unpack_status_buffer(); });

    add_run_step(
        [this]() { update_command_buffer(); },
        [this]() { unpack_status_buffer(); }
    );

    //add_end_step(...);
}

void H4EtherSnacksBoard::update_command_buffer() {
    std::cout << "---attempting to update command buffer---" << std::endl;
    const auto buffer = this->output_buffer<buffer_out_cyclic_command_t>(0x1100);
    buffer->status = 0;
}

void H4EtherSnacksBoard::unpack_status_buffer() {
    std::cout << "---attempting to uppack status buffer---" << std::endl;
    const auto buffer = this->input_buffer<buffer_in_cyclic_status_t>(0x1400);

    abstractIMU.setPosition(0.0, 0.0, 0.0);
    abstractIMU.setQuaternion(0.0, 0.0, 0.0, 0.0);
    abstractIMU.setAngularVelocity(buffer->gyroX * RAW_GYRO_TO_RAD_PER_SEC, buffer->gyroY * RAW_GYRO_TO_RAD_PER_SEC, buffer->gyroZ * RAW_GYRO_TO_RAD_PER_SEC);
    abstractIMU.setLinearAcceleration(buffer->accelX * RAW_ACCEL_TO_RAD_PER_SEC_PER_SEC, buffer->accelY * RAW_ACCEL_TO_RAD_PER_SEC_PER_SEC, buffer->accelZ * RAW_ACCEL_TO_RAD_PER_SEC_PER_SEC);
    abstractIMU.setIMUTemp(buffer->imuTemp * RAW_TEMP_TO_CELCIUS_SCALAR + RAW_TEMP_TO_CELCIUS_CONSTANT);
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
