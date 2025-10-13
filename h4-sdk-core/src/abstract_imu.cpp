//
// Created by srfas on 9/12/2025.
//

#include "abstract_imu.h"
#include <array>
#include <string>


abstract_imu::abstract_imu(const std::string& name) : name_(name), position(), quaternion(), angularVelocity(), linearAcceleration() {}

void abstract_imu::update() {  }

void abstract_imu::setPosition(const float& x, const float& y, const float& z)
{
  position[0] = x;
  position[1] = y;
  position[2] = z;
}

void abstract_imu::setQuaternion(const float& x, const float& y, const float& z, const float& s)
{
  quaternion[0] = x;
  quaternion[1] = y;
  quaternion[2] = z;
  quaternion[3] = s;
}

void abstract_imu::setAngularVelocity(const float& x, const float& y, const float& z)
{
  angularVelocity[0] = x;
  angularVelocity[1] = y;
  angularVelocity[2] = z;
}

void abstract_imu::setLinearAcceleration(const float& x, const float& y, const float& z)
{
  linearAcceleration[0] = x;
  linearAcceleration[1] = y;
  linearAcceleration[2] = z;
}