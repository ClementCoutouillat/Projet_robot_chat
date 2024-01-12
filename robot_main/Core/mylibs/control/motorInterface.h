/**
 * @file motorInterface.h
 * @author JiangboWANG
 * @brief
 * @version 0.1
 * @date 2023-12-28
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef __MOTOR_INTERFACE_H__
#define __MOTOR_INTERFACE_H__
#include "stdio.h"
#include "dcMotor.h"

void motorStop(void);
void driverMotor(uint16_t Vx, uint16_t Vz);
void changeAngle(uint16_t angle, const char *strPtr);
void goStraight(float speed);
void moveDistance(float distance, float speed);
#endif // __MOTOR_INTERFACE_H__