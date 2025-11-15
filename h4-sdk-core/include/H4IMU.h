//
// Created by srfas on 9/6/2025.
//

#ifndef H4_SDK_WARMUP_H4IMU_H
#define H4_SDK_WARMUP_H4IMU_H

#include "AbstractIMU.h"
#include <numbers>
#include <ethercatcpp/slave_device.h>

#define RX_PDO_ID 0x1600
#define TX_PDO_ID 0x1a00

class H4IMU : public AbstractIMU, public ethercatcpp::SlaveDevice
{
  struct [[gnu::packed]]buffer_in_cyclic_status_t {
    int16_t accelX = 0;
    int16_t accelY = 0;
    int16_t accelZ = 0;
    int16_t gyroX = 0;
    int16_t gyroY = 0;
    int16_t gyroZ = 0;
    int16_t imuTemp = 0;
    int16_t boardTemp = 0;
    uint16_t SlaveCycleCounter = 0;
    uint16_t slaveCycleTime = 0;
  } __attribute__((packed));

  struct [[gnu::packed]] buffer_out_cyclic_command_t {
    uint16_t reset = 0;
  } __attribute__((packed));

  static constexpr uint32_t VENDOR_ID = 0x1011;
  static constexpr uint32_t PRODUCT_CODE = 0x00000200;

  const float GRAVITY = 9.80665;
  const float RAW_ACCEL_TO_G = 0.000244;
  const float RAW_ACCEL_TO_RAD_PER_SEC_PER_SEC = RAW_ACCEL_TO_G * GRAVITY;
  const float RAW_GYRO_TO_RAD_PER_SEC = (2000.0 * (std::numbers::pi * 2.0) / 360.0) / 65535.0;
  const float RAW_TEMP_TO_CELCIUS_SCALAR = 1.0 / 132.48;
  const float RAW_TEMP_TO_CELCIUS_CONSTANT = 25.0;

  float boardTemp = 0.0;

  public:
  H4IMU(const std::string& name, const uint32_t& alias);

  void update() override {}

  void update_command_buffer();

  void unpack_status_buffer();

  void setBoardTemp(const float& boardTemp_) { this->boardTemp = boardTemp_; }

  float getBoardTemp() const { return boardTemp; }

  std::string getName() const override { return name_; }

  ~H4IMU() override = default;
};


#endif// H4_SDK_WARMUP_H4IMU_H
