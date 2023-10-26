/*
 * PIDControl.c
 *
 *  Created on: Oct 25, 2023
 *      Author: jkx
 */


#include "PIDControl.h"

PIDTypeDef gSpeedPID;
PIDTypeDef gCurrentPID;

void PIDInit(void)
{
    gSpeedPID.SetPoint = 0;    // set the goal speed
    gSpeedPID.ActualValue = 0; // set the actual speed

    gSpeedPID.SumError = 0;  // set the sum error
    gSpeedPID.Error = 0;     // set the current error
    gSpeedPID.LastError = 0; // set the last error
    gSpeedPID.PrevError = 0; // set the previous error

    gSpeedPID.Proportion = S_KP; // set the proportion
    gSpeedPID.Integral = S_KI;   // set the integral
    gSpeedPID.Derivative = S_KD; // set the derivative

    gCurrentPID.SetPoint = 0;    // set the goal current
    gCurrentPID.ActualValue = 0; // set the actual current

    gCurrentPID.SumError = 0;  // set the sum error
    gCurrentPID.Error = 0;     // set the current error
    gCurrentPID.LastError = 0; // set the last error
    gCurrentPID.PrevError = 0; // set the previous error

    gCurrentPID.Proportion = C_KP; // set the proportion
    gCurrentPID.Integral = C_KI;   // set the integral
    gCurrentPID.Derivative = C_KD; // set the derivative
}

void setPIDparam(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("[ERROR]: Invalid number of argument\r\n");
        return;
    }
    float kp = atof(argv[1]);
    float ki = atof(argv[2]);
    float kd = atof(argv[3]);
    gSpeedPID.Proportion = kp;
    gSpeedPID.Integral = ki;
    gSpeedPID.Derivative = kd;
    printf("[INFO]: Set PID param to kp: %f, ki: %f, kd: %f\r\n", kp, ki, kd);
}

float incrementPIDControl(PIDTypeDef *pid, float feedbackValue)
{
    pid->Error = (float)(pid->SetPoint - feedbackValue); // calculate the current error  SetPoint在speedProcess中被赋值
#if INCRLOCT_SELECT  //（Incremental PID）

    pid->ActualValue += (pid->Proportion * (pid->Error - pid->LastError))                         // proportional term
                        + (pid->Integral * pid->Error)                                            // integral term
                        + (pid->Derivative * (pid->Error - 2 * pid->LastError + pid->PrevError)); // derivative term
    pid->PrevError = pid->LastError;                                                              // save the previous error
    pid->LastError = pid->Error;                                                                   // save the last error
#else //（location PID）
    pid->SumError += pid->Error;                                            // calculate the sum error
    pid->ActualValue = (pid->Proportion * pid->Error)                       // proportional term
                       + (pid->Integral * pid->SumError)                    // integral term
                       + (pid->Derivative * (pid->Error - pid->LastError)); // derivative term
    pid->LastError = pid->Error;                                            // save the last error
#endif
    // printf("[INFO]: pid->ActualValue: %f\n", pid->ActualValue);
    return (float)pid->ActualValue; // return the expected value after PID control calculation
}

void integraLlimit(PIDTypeDef *PID, float maxLimit, float minLimit)
{
    if (PID->SumError >= maxLimit) // limit the integral value
    {
        PID->SumError = maxLimit;
    }
    else if (PID->SumError <= minLimit) // limit the integral value
    {
        PID->SumError = minLimit;
    }
}
