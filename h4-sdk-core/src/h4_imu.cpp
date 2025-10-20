//
// Created by srfas on 9/6/2025.
//

#include "h4_imu.h"
#include <string>
#include "pid/log.h"

h4_imu::h4_imu(const std::string &name, const uint32_t &alias)
: abstract_imu(name)
{
    set_id(name, VENDOR_ID, PRODUCT_CODE);

    configure_at_init( [this]()
    {
    // Link maps
    // Config Command PDO mapping (Rx PDO assign: 0x1c12)
    start_command_pdo_mapping<uint8_t>();
    add_command_pdo_mapping<uint8_t>(RX_PDO_ID); //Assign IO Map at CoE index 0x1600 to Rx PDO in 0x1c12.
    end_command_pdo_mapping<uint8_t>();

    // Config Status PDO mapping (Tx PDO assign: 0x1c13)
    start_status_pdo_mapping<uint8_t>();
    add_status_pdo_mapping<uint8_t>(TX_PDO_ID);  //Assign IO Map at CoE index 0x1A00 to Tx PDO in 0x1c13.
    end_status_pdo_mapping<uint8_t>();
    });

    // Communication buffer configuration (RxPDO / TxPDO)
    define_physical_buffer<buffer_out_cyclic_command_t>(SYNCHROS_OUT, 0x1060, 0x00010064); //TODO make sure these are right
    define_physical_buffer<buffer_in_cyclic_status_t>(SYNCHROS_IN, 0x10f0, 0x00010020);

    // Decide whether to use a distributed clock
    define_distributed_clock(false);

    add_init_step(
   [this]() { update_command_buffer(); },
   [this]() { unpack_status_buffer(); }
   );

    add_run_step(
        [this]() { update_command_buffer(); },
        [this]() { unpack_status_buffer(); }
    );

    //add_end_step(...);
}

void h4_imu::update_command_buffer() {
    const auto buffer = this->output_buffer<buffer_out_cyclic_command_t>(0x1800);
    buffer->reset = 0;
}

void h4_imu::unpack_status_buffer() {
    const auto buffer = this->input_buffer<buffer_in_cyclic_status_t>(0x1c00);

    setPosition(0.0, 0.0, 0.0);
    setQuaternion(0.0, 0.0, 0.0, 0.0);
    setAngularVelocity(buffer->gyroX * RAW_GYRO_TO_RAD_PER_SEC, buffer->gyroY * RAW_GYRO_TO_RAD_PER_SEC, buffer->gyroZ * RAW_GYRO_TO_RAD_PER_SEC);
    setLinearAcceleration(buffer->accelX * RAW_ACCEL_TO_RAD_PER_SEC_PER_SEC, buffer->accelY * RAW_ACCEL_TO_RAD_PER_SEC_PER_SEC, buffer->accelZ * RAW_ACCEL_TO_RAD_PER_SEC_PER_SEC);
    setIMUTemp(buffer->imuTemp * RAW_TEMP_TO_CELCIUS_SCALAR + RAW_TEMP_TO_CELCIUS_CONSTANT);
    setBoardTemp(buffer->boardTemp * RAW_TEMP_TO_CELCIUS_SCALAR + RAW_TEMP_TO_CELCIUS_CONSTANT);
}