//
// Created by srfas on 9/15/2025.
//

#include "abstract_motor_controller.h"

abstract_motor_controller::abstract_motor_controller(const string& name) : name(name), q(), qd(), qdd(), tau(), q_desired(), qd_desired(), qdd_desired(), tau_desired() {}

void abstract_motor_controller::setMeasuredData(float position, float velocity, float acceleration, float torque)
{
  q = position;
  qd = velocity;
  qdd = acceleration;
  tau = torque;
}

void abstract_motor_controller::setDesiredData(float position, float velocity, float acceleration, float torque)
{
  q_desired = position;
  qd_desired = velocity;
  qdd_desired = acceleration;
  tau_desired = torque;
}

float abstract_motor_controller::getMeasuredPosition()
{
  return q;
}

float abstract_motor_controller::getMeasuredVelocity()
{
  return qd;
}

float abstract_motor_controller::getMeasuredAcceleration()
{
  return qdd;
}

float abstract_motor_controller::getMeasuredTorque()
{
  return tau;
}

float abstract_motor_controller::getDesiredPosition()
{
  return q_desired;
}

float abstract_motor_controller::getDesiredVelocity()
{
  return qd_desired;
}

float abstract_motor_controller::getDesiredAcceleration()
{
  return qdd_desired;
}

float abstract_motor_controller::getDesiredTorque()
{
  return tau_desired;
}

string abstract_motor_controller::getName()
{
  return name;
}