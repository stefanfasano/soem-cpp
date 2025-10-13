//
// Created by srfas on 9/6/2025.
//

#ifndef H4_SDK_WARMUP_H4_ETHERSNACKS_BOARD_H
#define H4_SDK_WARMUP_H4_ETHERSNACKS_BOARD_H

#include "abstract_imu.h"
#include <array>
#include <string>
#include <ethercatcpp/slave_device.h>

using namespace std;


class h4_ethersnacks_board
{
  string name;
  abstract_imu abstract_imu_;

  public:
  h4_ethersnacks_board(const string& name, const abstract_imu& abstract_imu);

  virtual void read();

  virtual void write();

  virtual abstract_imu get_imu();

  virtual string get_name();

  virtual ~h4_ethersnacks_board() = default;
};


#endif// H4_SDK_WARMUP_H4_ETHERSNACKS_BOARD_H
