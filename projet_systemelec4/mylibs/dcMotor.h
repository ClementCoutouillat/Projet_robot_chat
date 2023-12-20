/*
 * dcMotor.h
 *
 *  Created on: Oct 25, 2023
 *      Author: jkx
 */

#ifndef DCMOTOR_H_
#define DCMOTOR_H_

#include "stdio.h"
#include "stdint.h"
#include "main.h"

#include "getEncoder.h"
#include "PIDControl.h"

#define SPEED_COMPUTE_PER_S 1000 / 100
#define SECONDPERMINUTE 60.0f
// define the PWM Duty Cycle for stop the motor
#define PWM_DUTY_CYCLE_STOP 50
// define the PWM Duty Cycle for start the motor
#define PWM_DUTY_CYCLE_START 50
// define the encoder mode division for compute the speed
#define ENCODER_MODE_DIVISION 4.0f
// define the encoder count per round
#define ENCODER_COUNT_PER_ROUND 11.0f
// remove the first 3 current value
#define CURRENT_INIT_COUNT 3
///<  change rpm to rad/s
#define RPM2RAD 0.104719755f

#define GEAR_RATIO  20

#define MOTOR_STATE_START 1
#define MOTOR_STATE_STOP 0

//#define ENABLE_MOTOR HAL_GPIO_WritePin(MOTOR_ENABLE_GPIO_Port, MOTOR_ENABLE_Pin, GPIO_PIN_SET)
//#define DISABLE_MOTOR HAL_GPIO_WritePin(MOTOR_ENABLE_GPIO_Port, MOTOR_ENABLE_Pin, GPIO_PIN_RESET)
extern TIM_HandleTypeDef htim1;

typedef struct
{
    int encode_old; /* old encode value */
    int encode_now; /* current encode value */
    float speed;         /* current speed */
} EncoderTypeDef;

typedef struct
{
    uint8_t state;     /* current state */
    uint8_t direction; /* current direction */
    float current;     /* motor current */
    float voltage;     /* motor voltage */
    float speed;       /* motor speed */
    float motorPWM;    /* motor PWM */
} MotorTypeDef;

extern MotorTypeDef gMotorData;
extern MotorTypeDef gMotorData2;
// init the motor
void dcMotorInit(void);
// stop the motor by stop the PWM
void stopMotor(void);
// start the motor by start the PWM
void startMotor(void);
// Set the Speed with the command : speed <speed>
void setSpeed(int numberofCmdArg, char *cmdArg[]);

void moteur_controle(float m_gauche,float m_droite);
// compute speed per xxx ms

void speedCompute2(int encode_now, uint8_t computePerms);

void speedCompute(int encode_now, uint8_t computePerms);

#endif /* DCMOTOR_H_ */
