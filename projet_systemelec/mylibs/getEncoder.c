/*
 * getEncoder.c
 *
 *  Created on: Oct 25, 2023
 *      Author: jkx
 */

#include "getEncoder.h"
extern TIM_HandleTypeDef htim3;
/**
 * @brief this function starts the encoder
 *
 */
void startEncoder(void)
{
    HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
}

/**
 * @brief Get the Current Encoder Count
 *
 * @return uint16_t The current encoder count
 */
uint8_t getMotorDirectionfromEncoder(void)
{
    return __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim1);
}

/**
 * @brief Get the Current Encoder Count
 *
 * @return uint16_t The current encoder count
 */
uint32_t getEncoderValue(void)
{
    uint32_t Encoder_Count = 0;
    /* Get the current counter value */
    Encoder_Count = __HAL_TIM_GET_COUNTER(&htim3);
    /* Add the overflow count to the counter value */
    Encoder_Count += Encoder_Overflow_Count * 65536;
    return Encoder_Count;
}

/**
 * @brief Get the Current Encoder Speed
 *
 * @return uint16_t The current encoder speed
 */
uint16_t getMotorSpeedfromEncoder(void)
{
    uint32_t motorSpeed = 0;
    motorSpeed = getEncoderValue() / ENCODER_MODE_DIVISION;
    return motorSpeed;
}
