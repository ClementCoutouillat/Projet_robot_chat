/*
 * setSpeed.h
 *
 *  Created on: Oct 25, 2023
 *      Author: jkx
 */

#ifndef SETSPEED_H_
#define SETSPEED_H_


#include "stdbool.h"
#include "stdio.h"
#include "dcMotor.h"

#define MAX_PWM_DUTY_CYCLE 90
#define DIRECTION_FORWARD 1
#define DIRECTION_BACKWARD -1
#define UART_TX_BUFFER_SIZE 64

// define direction

void updateCurrentSpeed(int updateSpeed);
uint8_t changeSpeedToPWM(float speed);
// void setPWMdutyCicle(float Alpha);
void setPWMdutyCicle(int updatepwmDutyCycle);
void setPWMdutyCiclePID(float Alpha);
void motor_pwm_set(float para);
void motor_pwm_set2(float para);
void dcmotor_dir(uint8_t para);
void dcmotor_dir2(uint8_t para);
void dcmotor_speed(uint16_t para);
void dcmotor_speed2(uint16_t para);
void speedProcess(int numberofCmdArg, char *cmdArg[]);
void speedProcess2(int numberofCmdArg, char *cmdArg[]);

#endif /* SETSPEED_H_ */
