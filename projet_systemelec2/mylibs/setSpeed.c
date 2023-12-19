/*
 * setSpeed.c
 *
 *  Created on: Oct 25, 2023
 *      Author: jkx
 */

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "usart.h"
#include "setSpeed.h"
#include "PIDControl.h"
#include "tim.h"

// extern htim1
extern TIM_HandleTypeDef htim1;
uint8_t speedTxStringLength;
uint8_t speedTxBuffer[UART_TX_BUFFER_SIZE]; // buffer for the transmission
int8_t currentpwmDutyCycle = 50;

/**
 * @brief       Motor Direction Control
 * @param       para: 0 Forward, 1 Reverse
 */
void dcmotor_dir(uint8_t para)
{

	HAL_TIM_PWM_Stop(&htim17, TIM_CHANNEL_1);
    HAL_TIM_PWM_Stop(&htim15, TIM_CHANNEL_2);
	HAL_TIM_PWM_Stop(&htim15, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim16, TIM_CHANNEL_1);

    if (para == 0)
    {
        HAL_TIM_PWM_Start(&htim17, TIM_CHANNEL_1);
        HAL_TIM_PWM_Stop(&htim16, TIM_CHANNEL_1);
        HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_2);
        HAL_TIM_PWM_Stop(&htim15, TIM_CHANNEL_1);

      // __HAL_TIM_SetCompare(&htim17 , TIM_CHANNEL_1, 600);  //  motor1 avancer    para为设置比较器的�??
      // __HAL_TIM_SetCompare(&htim15 , TIM_CHANNEL_2, 600);  //motor2 avancer
    }
    else if (para == 1)
    {
        HAL_TIM_PWM_Start(&htim16, TIM_CHANNEL_1);
    	HAL_TIM_PWM_Stop(&htim17, TIM_CHANNEL_1);
    	HAL_TIM_PWM_Stop(&htim15, TIM_CHANNEL_2);
        HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_1);
        //  __HAL_TIM_SetCompare(&htim16 , TIM_CHANNEL_1, 300);  //  motor1 reculer
         //__HAL_TIM_SetCompare(&htim15 , TIM_CHANNEL_1, 300);  // motor2 reculer
    }
}


void dcmotor_speed(uint16_t para)
{

/*
    __HAL_TIM_SetCompare(&htim17 , TIM_CHANNEL_1, para);  //PB9
    __HAL_TIM_SetCompare(&htim15 , TIM_CHANNEL_2, para);  //PB15
    __HAL_TIM_SetCompare(&htim16 , TIM_CHANNEL_1, para);  //PB9
    __HAL_TIM_SetCompare(&htim14 , TIM_CHANNEL_1, para);  //PB15
*/
    __HAL_TIM_SetCompare(&htim17 , TIM_CHANNEL_1, para);  //PB9   para为占空比的百分比乘以周期时间
    __HAL_TIM_SetCompare(&htim16 , TIM_CHANNEL_1, para);  //PB15
    __HAL_TIM_SetCompare(&htim15 , TIM_CHANNEL_2, para);  //PB9
    __HAL_TIM_SetCompare(&htim15 , TIM_CHANNEL_1, para);  //PB15


}


void motor_pwm_set(float para)
{
    int val = (int)para;

    if (val >= 0)
    {
        dcmotor_dir(0);
        dcmotor_speed(val);
    }
    else
    {
        dcmotor_dir(1);
        dcmotor_speed(-val);
    }
}


/**
 * @brief this function checks if the speed is valid
 *
 * @param speedStr  the speed string to check
 * @return true  if the speed is valid
 * @return false  if the speed is not valid
 */
bool isSpeedValid(char *speedStr)
{
    int i = 0;
    // get the direction for speedStr[0] with - or +
    if (speedStr[0] == '-' || speedStr[0] == '+')
    {
        i++;
    }
    else
    {
        return false;
    }
    while (speedStr[i] != '\0')
    {
        // if the char is not a number, return false
        if (!(speedStr[i] >= '0' && speedStr[i] <= '9'))
        {
            return false;
        }
        i++;
    }

    return true;
}

/**
 * @brief  this function extract the speed from the string
 *
 * @param data  the data to extract the speed
 * @return int  the speed extracted
 */
float extractSpeed(char *data)
{
    float speed = 0;
    int direction = DIRECTION_FORWARD;
    // if the speed is negative, set the direction to -1,if not, set the direction to 1
    if (data[0] == '-')
    {
        direction = DIRECTION_BACKWARD;
    }
    else
    {
        direction = DIRECTION_FORWARD;
    }
    // change the string speed to int speed without the direction
    speed = atof(&data[1]);
    // return the speed with the direction
    return (float)speed * direction;
}

/**
 * @brief This function change the speed to pwm duty cycle
 * @ 0-300 vitesse    0-640 autoreload(pwm)
 * @param speed the speed to change
 */
uint8_t changeSpeedToPWM(float speed)
{
    int pwmDutyCycle = 0;
    if (speed >= 0)
    {
        pwmDutyCycle = (int)(speed * 2560.0 / 300.0);
    }
    else
    {
        pwmDutyCycle = (int)(-speed * 2560.0 / 300.0);
    }
    return pwmDutyCycle;
}



/**
 * @brief this function process the speed
 *
 * @param argc  the number of arguments
 * @param argv  the arguments of the command "speed xxxx"
 */
void speedProcess2(int numberofCmdArg, char *cmdArg[])
{
    // check if the number of argument is correct
    if (numberofCmdArg != 2)
    {
        printf("[ERROR]: Invalid number of argument\r\n");
        return;
    }
    // check if the speed is valid
    if (!isSpeedValid(cmdArg[1]))
    {
        printf("[ERROR]: Invalid speed\r\n");
        return;
    }
    // extract the speed from the string
    uint8_t updateSpeed = extractSpeed(cmdArg[1]);
    // set the speed
    // setMotorSpeed(updateSpeed);
    setPWMdutyCicle(updateSpeed);
    // update the current speed
    updateCurrentSpeed(updateSpeed);
    printf("[INFO]: Set speed to %d\r\n", updateSpeed);
}

void speedProcess(int numberofCmdArg, char *cmdArg[])
{
    // check if the number of argument is correct
    if (numberofCmdArg != 2)
    {
        printf("[ERROR]: Invalid number of argument\r\n");
        return;
    }
    // check if the speed is valid
    if (!isSpeedValid(cmdArg[1]))
    {
        printf("[ERROR]: Invalid speed\r\n");
        return;
    }
    gSpeedPID.SetPoint = extractSpeed(cmdArg[1]);

    // limit the speed
    if (gSpeedPID.SetPoint > MAX_SPEED)   //MAX_SPEED : 300 tour/minute
    {
        gSpeedPID.SetPoint = MAX_SPEED;
    }
    else if (gSpeedPID.SetPoint < -MAX_SPEED)
    {
        gSpeedPID.SetPoint = -MAX_SPEED;
    }

 /*   //调试motor2
    gSpeedPID2.SetPoint = extractSpeed(cmdArg[1]);
    if (gSpeedPID2.SetPoint > MAX_SPEED)   //MAX_SPEED : 300 tour/minute
    {
        gSpeedPID2.SetPoint = MAX_SPEED;
    }
    else if (gSpeedPID2.SetPoint < -MAX_SPEED)
    {
        gSpeedPID2.SetPoint = -MAX_SPEED;
    }
    printf("[INFO]: Set speed to %f\r\n", gSpeedPID2.SetPoint);*/
}

