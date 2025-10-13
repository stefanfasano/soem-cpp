//
// Created by srfas on 9/15/2025.
//

#ifndef H4_SDK_WARMUP_ABSTRACT_MOTOR_CONTROLLER_H
#define H4_SDK_WARMUP_ABSTRACT_MOTOR_CONTROLLER_H

#include <string>
using namespace std;

class abstract_motor_controller
{
  string name;

  float q; // measured position
  float qd; // measured velocity
  float qdd; // measured acceleration
  float tau; // measured torque

  float q_desired; // desired position
  float qd_desired; // desired velocity
  float qdd_desired; // desired acceleration
  float tau_desired; // desired torque

  public:
  explicit abstract_motor_controller(const string& name);

  virtual void update() = 0;

  virtual void setMeasuredData(float q, float qd, float qdd, float tau);

  virtual void setDesiredData(float q_desired, float qd_desired, float qdd_desired, float tau_desired);

  virtual float getMeasuredPosition();

  virtual float getMeasuredVelocity();

  virtual float getMeasuredAcceleration();

  virtual float getMeasuredTorque();

  virtual float getDesiredPosition();

  virtual float getDesiredVelocity();

  virtual float getDesiredAcceleration();

  virtual float getDesiredTorque();

  virtual string getName();

  virtual ~abstract_motor_controller()= default;
};


#endif// H4_SDK_WARMUP_ABSTRACT_MOTOR_CONTROLLER_H
