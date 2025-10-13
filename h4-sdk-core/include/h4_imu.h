//
// Created by srfas on 9/6/2025.
//

#ifndef H4_SDK_WARMUP_H4IMU_H
#define H4_SDK_WARMUP_H4IMU_H
#include <cstdint>

#include "abstract_imu.h"
#include <numbers>
#include <ethercatcpp/slave_device.h>

#define RX_PDO_ID 0x1600
#define TX_PDO_ID 0x1a00
#define VENDOR_ID 0x1011
#define PRODUCT_CODE 0x00000200

struct TxPdo {
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

struct RxPdo {
  uint16_t reset = 0;
} __attribute__((packed));

class h4_imu : public abstract_imu
{
  float GRAVITY = 9.80665;
  float RAW_ACCEL_TO_G = 0.000244;
  float RAW_ACCEL_TO_RAD_PER_SEC_PER_SEC = RAW_ACCEL_TO_G * GRAVITY;
  float RAW_GYRO_TO_RAD_PER_SEC = (2000.0 * (std::numbers::pi * 2.0) / 360.0) / 65535.0;
  float RAW_TEMP_TO_CELCIUS_SCALAR = 1.0 / 132.48;
  float RAW_TEMP_TO_CELCIUS_CONSTANT = 25.0;

  // PdoInfo pdoInfo_;
  TxPdo reading_;
  RxPdo command_;

  public:
  explicit h4_imu(const std::string& name, const uint32_t& address);

  // bool startup() override;
  //
  void update() override;
  //
  // void updateRead() override;
  //
  // void updateWrite() override;
  //
  // void shutdown() override;

  float getIMUTemp() const { return reading_.imuTemp * RAW_TEMP_TO_CELCIUS_SCALAR + RAW_TEMP_TO_CELCIUS_CONSTANT; }

  float getBoardTemp() const { return reading_.boardTemp * RAW_TEMP_TO_CELCIUS_SCALAR + RAW_TEMP_TO_CELCIUS_CONSTANT; }

  std::string getName() const override { return name_; }

  // PdoInfo getCurrentPdoInfo() const override { return pdoInfo_; }

  ~h4_imu() override = default;
};


#endif// H4_SDK_WARMUP_H4IMU_H
