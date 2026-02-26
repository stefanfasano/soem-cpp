//
// Created by srfas on 9/11/2025.
//

#ifndef H4_SDK_WARMUP_ABSTRACT_IMU_H
#define H4_SDK_WARMUP_ABSTRACT_IMU_H
#include <array>
#include <string>
using namespace std;

class AbstractIMU
{
  protected:
  const string name;

  array<float, 3> position; // In world frame (x, y, z)
  array<float, 4> quaternion; // In world frame (x, y, z, s)
  array<float, 3> angularVelocity; // In sensor frame (x, y, z)
  array<float, 3> linearAcceleration; // In sensor frame (x, y, z)
  float imuTemp = 0.0;

  public:
  explicit AbstractIMU(const string& name);

  virtual void update();

  virtual void print();

  virtual void setPosition(float x, float y, float z);

  virtual void setQuaternion(float x, float y, float z, float s);

  virtual void setAngularVelocity(float x, float y, float z);

  virtual void setLinearAcceleration(float x, float y, float z);

  virtual void setIMUTemp(float imuTemp) { this->imuTemp = imuTemp; }

  const virtual array<float, 3>& getPosition() { return position; }

  const virtual array<float, 4>& getQuaternion() { return quaternion; }

  const virtual array<float, 3>& getAngularVelocity() { return angularVelocity; }

  const virtual array<float, 3>& getLinearAcceleration() { return linearAcceleration; }

  virtual float getIMUTemp() { return imuTemp; }

  const virtual string& getName() { return name; }

  virtual ~AbstractIMU() = default;
};


#endif// H4_SDK_WARMUP_ABSTRACT_IMU_H
