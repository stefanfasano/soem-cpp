//
// Created by srfas on 9/6/2025.
//

#ifndef H4_SDK_WARMUP_H4_ETHERSNACKS_BOARD_H
#define H4_SDK_WARMUP_H4_ETHERSNACKS_BOARD_H

#include <cstdint>

#include "../include/AbstractIMU.h"
#include <numbers>
#include <string>

#include "../include/Master.h"
#include "osal_defs.h"
#include "../include/Slove.h"

using namespace std;

#define RX_PDO_ID 0x1600
#define TX_PDO_ID 0x1a00

class H4EtherSnacksBoard : public Slove
{
  OSAL_PACKED_BEGIN
  struct OSAL_PACKED ProcessData
  {
    struct OSAL_PACKED
    {
      uint16_t status = 0;
    } outputs;

    struct OSAL_PACKED
    {
      int16_t accelX = 0;
      int16_t accelY = 0;
      int16_t accelZ = 0;
      int16_t gyroX = 0;
      int16_t gyroY = 0;
      int16_t gyroZ = 0;
      uint16_t imuTemp = 0;
      uint16_t sensorTemp = 0;
      uint16_t EthersnacksCycleCounter = 0;
      uint16_t LastEthercatCycleTime = 0;
      uint16_t EthercatLinkStatus = 0;
      uint16_t LastGripperCycleTime = 0;
      uint16_t MeasuredInputVoltage = 0;
      uint16_t MeasuredOutputVoltage = 0;
      uint16_t MeasuredCurrentLoad = 0;
    } inputs;
  }
  OSAL_PACKED_END;

  volatile ProcessData* const pd = reinterpret_cast<volatile ProcessData*>(Master::IOmap);

  // Safety check: EtherCAT layout must be exact
  // static_assert(sizeof(ProcessData) == 16, "EtherCAT ProcessData layout mismatch");

  static constexpr uint32_t VENDOR_ID = 0x00000603;
  static constexpr uint32_t PRODUCT_CODE = 0x42000000;

  const float GRAVITY = 9.80665;
  const float RAW_ACCEL_TO_G = 0.000244;
  const float RAW_ACCEL_TO_RAD_PER_SEC_PER_SEC = RAW_ACCEL_TO_G * GRAVITY;
  const float RAW_GYRO_TO_RAD_PER_SEC = (2000.0 * (std::numbers::pi * 2.0) / 360.0) / 65535.0;
  const float RAW_TEMP_TO_CELCIUS_SCALAR = 1.0 / 132.48;
  const float RAW_TEMP_TO_CELCIUS_CONSTANT = 25.0;

  string name;
  AbstractIMU abstractIMU;

  public:
  H4EtherSnacksBoard(const string& name, bool addIMU);

  void update() override;

  void read();

  virtual void print();

  virtual AbstractIMU get_imu();

  virtual string get_name();

  ~H4EtherSnacksBoard() override = default;
};


#endif// H4_SDK_WARMUP_H4_ETHERSNACKS_BOARD_H
