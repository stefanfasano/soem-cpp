//
// Created by srfas on 9/11/2025.
//

#ifndef H4_SDK_WARMUP_ABSTRACT_IMU_H
#define H4_SDK_WARMUP_ABSTRACT_IMU_H
#include <array>
#include <string>


class abstract_imu
{
  protected:
  std::string name_;

  std::array<float, 3> position; // In world frame (x, y, z)
  std::array<float, 4> quaternion; // In world frame (x, y, z, s)
  std::array<float, 3> angularVelocity; // In sensor frame (x, y, z)
  std::array<float, 3> linearAcceleration; // In sensor frame (x, y, z)

  public:
  explicit abstract_imu(const std::string& name);

  virtual void update();

  virtual void setPosition(const float& x, const float& y, const float& z);

  virtual void setQuaternion(const float& x, const float& y, const float& z, const float& s);

  virtual void setAngularVelocity(const float& x, const float& y, const float& z);

  virtual void setLinearAcceleration(const float& x, const float& y, const float& z);

  virtual std::array<float, 3> getPosition() const { return position; }

  virtual std::array<float, 4> getQuaternion() const { return quaternion; }

  virtual std::array<float, 3> getAngularVelocity() const { return angularVelocity; }

  virtual std::array<float, 3> getLinearAcceleration() const { return linearAcceleration; }

  virtual std::string getName() const { return name_; }

  virtual ~abstract_imu() = default;
};


#endif// H4_SDK_WARMUP_ABSTRACT_IMU_H
