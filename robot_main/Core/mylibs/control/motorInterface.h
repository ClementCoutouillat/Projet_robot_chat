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

void carStop(void);
void driverMotor(uint16_t Vx, uint16_t Vz);
void changeAngle(float angle);
void goStraight(float m_gauche, float m_droite);
#endif // __MOTOR_INTERFACE_H__