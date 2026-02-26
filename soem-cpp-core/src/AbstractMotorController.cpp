//
// Created by srfas on 9/15/2025.
//

#include "AbstractMotorController.h"

AbstractMotorController::AbstractMotorController(const string& name) : name(name), q(), qd(), qdd(), tau(), q_desired(), qd_desired(), qdd_desired(), tau_desired() {}

void AbstractMotorController::setMeasuredData(float position, float velocity, float acceleration, float torque)
{
  q = position;
  qd = velocity;
  qdd = acceleration;
  tau = torque;
}

void AbstractMotorController::setDesiredData(float position, float velocity, float acceleration, float torque)
{
  q_desired = position;
  qd_desired = velocity;
  qdd_desired = acceleration;
  tau_desired = torque;
}