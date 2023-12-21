#ifndef __MOTOR_INTERFACE_H__
#define __MOTOR_INTERFACE_H__
#include "stdio.h"
#include "dcMotor.h"

void carInit(void);
void carLeft(void);
void carRight(void);
void carForward(void);
void carBackward(void);
void carStop(void);
void driverMotor(uint16_t Vx, uint16_t Vz);
void changeAngle(float angle);
void goStraight(float m_gauche, float m_droite);
#endif // __MOTOR_INTERFACE_H__