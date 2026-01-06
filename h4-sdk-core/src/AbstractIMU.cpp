//
// Created by srfas on 9/12/2025.
//

#include "AbstractIMU.h"
#include <array>
#include <iostream>
#include <string>

AbstractIMU::AbstractIMU(const std::string& name) : name_(name), position(), quaternion(), angularVelocity(), linearAcceleration() {}

void AbstractIMU::update() {  }

void AbstractIMU::print() {
  std::cout << "---------------------------------------------------------------------------------------------" << std::endl;
  std::cout << "PRINTING IMU STATE: " << std::endl;
  std::cout << "Position x,y,z: " << getPosition()[0] << getPosition()[1] << getPosition()[2] << std::endl;
  std::cout << "Quaternion x,y,z: " << getQuaternion()[0] << getQuaternion()[1] << getQuaternion()[2] << std::endl;
  std::cout << "Gyro x,y,z: " << getAngularVelocity()[0] << getAngularVelocity()[1] << getAngularVelocity()[2] << std::endl;
  std::cout << "Accel x,y,z: " << getLinearAcceleration()[0] << getLinearAcceleration()[1] << getLinearAcceleration()[2] << std::endl;
  std::cout << "IMU temp: " << getIMUTemp() << std::endl;
  std::cout << "---------------------------------------------------------------------------------------------" << std::endl;
}

void AbstractIMU::setPosition(const float& x, const float& y, const float& z)
{
  position[0] = x;
  position[1] = y;
  position[2] = z;
}

void AbstractIMU::setQuaternion(const float& x, const float& y, const float& z, const float& s)
{
  quaternion[0] = x;
  quaternion[1] = y;
  quaternion[2] = z;
  quaternion[3] = s;
}

void AbstractIMU::setAngularVelocity(const float& x, const float& y, const float& z)
{
  angularVelocity[0] = x;
  angularVelocity[1] = y;
  angularVelocity[2] = z;
}

void AbstractIMU::setLinearAcceleration(const float& x, const float& y, const float& z)
{
  linearAcceleration[0] = x;
  linearAcceleration[1] = y;
  linearAcceleration[2] = z;
}