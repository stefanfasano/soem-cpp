//
// Created by srfas on 9/15/2025.
//

#ifndef H4_SDK_WARMUP_ABSTRACT_MOTOR_CONTROLLER_H
#define H4_SDK_WARMUP_ABSTRACT_MOTOR_CONTROLLER_H

#include <string>
using namespace std;

class AbstractMotorController
{
  protected:
  const string name;

  float q; // measured position
  float qd; // measured velocity
  float qdd; // measured acceleration
  float tau; // measured torque

  float q_desired; // desired position
  float qd_desired; // desired velocity
  float qdd_desired; // desired acceleration
  float tau_desired; // desired torque

  public:
  explicit AbstractMotorController(const string& name);

  virtual void update() = 0;

  virtual void setMeasuredData(float q, float qd, float qdd, float tau);

  virtual void setDesiredData(float q_desired, float qd_desired, float qdd_desired, float tau_desired);

  virtual float getMeasuredPosition(){ return q; };

  virtual float getMeasuredVelocity(){ return qd; };

  virtual float getMeasuredAcceleration(){ return qdd; };

  virtual float getMeasuredTorque(){ return tau; };

  virtual float getDesiredPosition(){ return q_desired; };

  virtual float getDesiredVelocity(){ return qd_desired; };

  virtual float getDesiredAcceleration(){ return qdd_desired; };

  virtual float getDesiredTorque(){ return tau_desired; };

  const virtual string& getName(){ return name; };

  virtual ~AbstractMotorController()= default;
};


#endif// H4_SDK_WARMUP_ABSTRACT_MOTOR_CONTROLLER_H
