/*
 * PIDControl.h
 *
 *  Created on: Oct 25, 2023
 *      Author: jkx
 */

#ifndef PIDCONTROL_H_
#define PIDCONTROL_H_


#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "main.h"

#define INCR_LOCT_SELECT 0 // 0: location pid, 1: incremental pid
#if INCR_LOCT_SELECT
// incremental SPEED pid
#define S_KP 8.50f /*p coefficient*/
#define S_KI 5.00f /*i coefficient*/
#define S_KD 0.10f /*d coefficient*/

// incremental cuurrent pid
#define C_KP 0.50f /*p coefficient*/
#define C_KI 0.00f /*i coefficient*/
#define C_KD 0.00f /*d coefficient*/

#else
// location pid
#define S_KP 10.0f /*p coefficient*/
#define S_KI 8.00f  /*i coefficient*/
#define S_KD 0.5f    /*d coefficient*/

#define C_KP 0.80f   /*p coefficient*/
#define C_KI 2650.0f /*i coefficient*/
#define C_KD 0.00f   /*d coefficient*/

#endif
#define PID_SMAPLSE_TIME 50f /*pid sample time*/

typedef struct
{
    float SetPoint;  /* pid goal value */
    float ActualValue; /* pid actual value */
    float SumError;    /* sum of errors */
    float Proportion;  /* p coefficient */
    float Integral;    /* i coefficient */
    float Derivative;  /* d coefficient */
    float Error;       /* current error, error[0]*/
    float LastError;   /* last error error[-1] */
    float PrevError;   /* previous error error[-2] */
} PIDTypeDef;

extern PIDTypeDef gSpeedPID;
extern PIDTypeDef gCurrentPID;

void PIDInit(void);
void setPIDparam(int argc, char *argv[]);
void integraLlimit(PIDTypeDef *PID, float maxLimit, float minLimit);
float incrementPIDControl(PIDTypeDef *pid, float feedbackValue);




#endif /* PIDCONTROL_H_ */
