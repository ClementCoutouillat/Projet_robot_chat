/*
 * dcMotor.c
 *
 *  Created on: Oct 25, 2023
 *      Author: jkx
 */

#include "dcMotor.h"
#include "setSpeed.h"
#include "tim.h"
EncoderTypeDef gencodeSpeed;
EncoderTypeDef gencodeSpeed2;
MotorTypeDef gMotorData;
MotorTypeDef gMotorData2;
uint16_t Encoder_Overflow_Count = 0;
int Encoder_Overflow_Count2 = 0;

/**
 * @brief stop the motor by stop the PWM with the command: stop
 *
 */
void stopMotor(void)
{
    HAL_TIM_PWM_Stop(&htim15, TIM_CHANNEL_1);
   // HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Stop(&htim15, TIM_CHANNEL_2);
   // HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Stop(&htim16, TIM_CHANNEL_1);
    HAL_TIM_PWM_Stop(&htim17, TIM_CHANNEL_1);
    gMotorData.state = MOTOR_STATE_STOP;
}

/**
 * @brief start the motor by start the PWM with the command: start
 *
 */
void startMotor(void)
{
    HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim16, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim17, TIM_CHANNEL_1);

}
/**
 * @brief Set the Speed with the command : speed <speed>
 *
 * @param numberofCmdArg the number of command argument
 * @param cmdArg the command argument
 */
void setSpeed(int numberofCmdArg, char *cmdArg[])
{
    speedProcess(numberofCmdArg, cmdArg);
    gMotorData.state = MOTOR_STATE_START;
}

// founction to init the motor
void dcMotorInit(void)
{
    gencodeSpeed.encode_now = 0;
    gencodeSpeed.encode_old = 0;
    gencodeSpeed.speed = 0;
    gMotorData.state = MOTOR_STATE_STOP;
    gMotorData.direction = 1;
    gMotorData.current = 0;
    gMotorData.voltage = 0;
    gMotorData.speed = 0;
    gMotorData.motorPWM = 0;
    gencodeSpeed2.encode_now = 0;
    gencodeSpeed2.encode_old = 0;
    gencodeSpeed2.speed = 0;
    gMotorData2.state = MOTOR_STATE_STOP;
    gMotorData2.direction = 1;
    gMotorData2.current = 0;
    gMotorData2.voltage = 0;
    gMotorData2.speed = 0;
    gMotorData2.motorPWM = 0;
}
int32_t speedComputeCount = 0;
int32_t speedComputeCount2 = 0;
float speedArray[10] = {0.0};
float speedArray2[10] = {0.0};

int32_t speedArrayFilterIndex = 0;
int32_t speedArrayFilterIndex2 = 0;
void speedCompute(int encode_now, uint8_t computePerms)
{
    if (speedComputeCount == computePerms)
    {
        gencodeSpeed.encode_now = encode_now;
   //     printf("now:%d\r\n",gencodeSpeed.encode_now);
  //      printf("gencodeSpeed.encode_old:%d\r\n",gencodeSpeed.encode_old);
        gencodeSpeed.speed = (encode_now - gencodeSpeed.encode_old);
    //    printf("avant:%f\r\n", gMotorData.speed);
      //  printf("123\n");
//SPEED_COMPUTE_PER_S /  1s
        //calculte speed motor (tour/minute) : g_encode.speed * ((1000 / 100ms) *60 )/44/20  maxspeed （300*2pi/60）  现在是rpm 每分钟
       speedArray[speedArrayFilterIndex++] = (float)(gencodeSpeed.speed * (SPEED_COMPUTE_PER_S /computePerms )* 60  / ENCODER_MODE_DIVISION / ENCODER_COUNT_PER_ROUND/GEAR_RATIO);//*RPM2RADGEAR_RATIO
  //     gMotorData.speed = (float)(gencodeSpeed.speed * (SPEED_COMPUTE_PER_S / computePerms ) / ENCODER_MODE_DIVISION / ENCODER_COUNT_PER_ROUND/GEAR_RATIO);//*RPM2RAD
      //  printf("speedcompute:%f\r\n", gMotorData.speed);  //gMotorData.speed
        gencodeSpeed.encode_old = gencodeSpeed.encode_now;
       //  after 10 times of speed calculation, filter the speed data
        if (speedArrayFilterIndex == 10)
        {
            // bubble sort
            float temp = 0;
            for (int i = 0; i < 10; i++)
            {
                for (int j = 0; j < 10 - i - 1; j++)
                {
                    if (speedArray[j] > speedArray[j + 1])
                    {
                        temp = speedArray[j];
                        speedArray[j] = speedArray[j + 1];
                        speedArray[j + 1] = temp;
                    }
                }
            }

            temp = 0.0;
            // using the middle 6 data to calculate the average value
            for (int i = 2; i < 8; i++)
            {
                temp += speedArray[i];
            }
            // calculate the average value
            temp = temp / 6;
            // low pass filter
            // Y(n)= qX(n) + (1-q)Y(n-1)
            // X(n) is the current input, Y(n) is the current output, Y(n-1) is the previous output, and q is the filter factor.
            // The larger the q, the stronger the filtering effect, but the slower the response.
            gMotorData.speed = (float)((double)temp * 0.5 + (double)gMotorData.speed * 0.5);

            speedArrayFilterIndex = 0;
        }
        speedComputeCount = 0;
    }
    speedComputeCount++;
}



void speedCompute2(int encode_now, uint8_t computePerms)
{
    if (speedComputeCount2 == computePerms)
    {
        gencodeSpeed2.encode_now = encode_now;
   //     printf("now:%d\r\n",gencodeSpeed.encode_now);
  //      printf("gencodeSpeed.encode_old:%d\r\n",gencodeSpeed.encode_old);
        gencodeSpeed2.speed = (encode_now - gencodeSpeed2.encode_old);
    //    printf("avant:%f\r\n", gMotorData.speed);
      //  printf("123\n");
//SPEED_COMPUTE_PER_S /  1s
        //calculte speed motor (tour/minute) : g_encode.speed * ((1000 / 100ms) *60 )/44/20  maxspeed （300*2pi/60）  现在是rpm 每分钟
       speedArray2[speedArrayFilterIndex2++] = (float)(gencodeSpeed2.speed * (SPEED_COMPUTE_PER_S /computePerms )* 60  / ENCODER_MODE_DIVISION / ENCODER_COUNT_PER_ROUND/GEAR_RATIO);//*RPM2RADGEAR_RATIO
  //     gMotorData.speed = (float)(gencodeSpeed.speed * (SPEED_COMPUTE_PER_S / computePerms ) / ENCODER_MODE_DIVISION / ENCODER_COUNT_PER_ROUND/GEAR_RATIO);//*RPM2RAD
      //  printf("speedcompute:%f\r\n", gMotorData.speed);  //gMotorData.speed
        gencodeSpeed2.encode_old = gencodeSpeed2.encode_now;
       //  after 10 times of speed calculation, filter the speed data
        if (speedArrayFilterIndex2 == 10)
        {
            // bubble sort
            float temp = 0;
            for (int i = 0; i < 10; i++)
            {
                for (int j = 0; j < 10 - i - 1; j++)
                {
                    if (speedArray2[j] > speedArray2[j + 1])
                    {
                        temp = speedArray2[j];
                        speedArray2[j] = speedArray2[j + 1];
                        speedArray2[j + 1] = temp;
                    }
                }
            }

            temp = 0.0;
            // using the middle 6 data to calculate the average value
            for (int i = 2; i < 8; i++)
            {
                temp += speedArray2[i];
            }
            // calculate the average value
            temp = temp / 6;
            // low pass filter
            // Y(n)= qX(n) + (1-q)Y(n-1)
            // X(n) is the current input, Y(n) is the current output, Y(n-1) is the previous output, and q is the filter factor.
            // The larger the q, the stronger the filtering effect, but the slower the response.
            gMotorData2.speed = (float)((double)temp * 0.5 + (double)gMotorData2.speed * 0.5);

            speedArrayFilterIndex2 = 0;
        }
        speedComputeCount2 = 0;
    }
    speedComputeCount2++;
}

void moteur_controle(float m_gauche,float m_droite)
{
	 gSpeedPID.SetPoint = m_gauche;
	 gSpeedPID2.SetPoint = m_droite;
	 gMotorData.state = MOTOR_STATE_START;

}


//pc6 pc7 timer3  motor 1
//timer1 motor2
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM3)   //encooder compter  motor 1
    {
      //   Determine the current counter counting direction
        if (__HAL_TIM_IS_TIM_COUNTING_DOWN(htim))
        //     underflow, count down
        { Encoder_Overflow_Count--;

        }
        else{
          //   overflow, count up
            Encoder_Overflow_Count++;

        }
    }
    else if (htim->Instance == TIM1)  //encooder compter  motor 1
    {
        /* Determine the current counter counting direction */
        if (__HAL_TIM_IS_TIM_COUNTING_DOWN(htim))
            /* underflow, count down */
        { Encoder_Overflow_Count2--;
       HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_13); //vert
        }
        else{
            /* overflow, count up */
            Encoder_Overflow_Count2++;
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_12); //orange
        }
    }
    else if (htim->Instance == TIM7)   //every 100ms call the interruption for calculte PID
    {
        int encoderValueNow = getEncoderValue();
        int encoderValueNow2 = getEncoderValue2();
      //  printf("encoderValueNow:%ld\r\n", encoderValueNow);
        speedCompute(encoderValueNow, 1);
      // printf("speedcompute:%f\r\n", gMotorData.speed);  //gMotorData.speed*/

        speedCompute2(encoderValueNow2, 1);
        // printf("speedcompute2:%f\r\n", gMotorData2.speed);  //gMotorData.speed

        if (gMotorData.state == MOTOR_STATE_START)  //MOTOR_STATE_START call in setSpeed
        {
        //	printf("%.2f, %.2f\n", gSpeedPID.SetPoint, gMotorData.speed);
            gMotorData.motorPWM = incrementPIDControl(&gSpeedPID, gMotorData.speed); //gMotorData.speed is set in speedCompute
        //    printf("gMotorData.speed is： %f\r\n ",gMotorData.motorPWM);
            if(gMotorData.motorPWM >= 2560)                     /* ÏÞËÙ */
            {
            	gMotorData.motorPWM = 2560;
            }
            else if (gMotorData.motorPWM <= -2560)
            {
            	gMotorData.motorPWM = -2560;
            }
        //    printf("gMotorData.speed limite is： %f\r\n ",gMotorData.motorPWM);


            gMotorData2.motorPWM = incrementPIDControl2(&gSpeedPID2, gMotorData2.speed); //gMotorData.speed is set in speedCompute

            if(gMotorData2.motorPWM >= 2560)                     /* ÏÞËÙ */
            {
            	gMotorData2.motorPWM = 2560;
            }
            else if (gMotorData2.motorPWM <= -2560)
            {
            	gMotorData2.motorPWM = -2560;
            }

          // setPWMdutyCiclePID(gMotorData.motorPWM);
         //  motor_pwm_set( changeSpeedToPWM(gMotorData.motorPWM));
          motor_pwm_set(gMotorData.motorPWM);
          motor_pwm_set2(gMotorData2.motorPWM);
        }
    }
}

