#ifndef __MOTOR_INTERFACE_H__
#define __MOTOR_INTERFACE_H__
#include "stdio.h"

void carInit(void);
void carLeft(void);
void carRight(void);
void carForward(void);
void carBackward(void);
void carStop(void);
// void driverMotor(uint16_t Vx, uint16_t Vz);

#endif // __MOTOR_INTERFACE_H__