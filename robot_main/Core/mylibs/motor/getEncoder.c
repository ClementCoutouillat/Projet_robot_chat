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
int32_t getEncoderValue(void)
{
    int32_t Encoder_Count = 0;
    /* Get the current counter value */
    Encoder_Count = __HAL_TIM_GET_COUNTER(&htim3); // motor 1
    // printf("Encoder_Count:%d\r\n", Encoder_Count);
    /* Add the overflow count to the counter value */
    Encoder_Count += Encoder_Overflow_Count * 65536;
    return Encoder_Count;
}

int32_t getEncoderValue2(void)
{
    int32_t Encoder_Count = 0;
    /* Get the current counter value */
    Encoder_Count = __HAL_TIM_GET_COUNTER(&htim1); // motor 1
    // printf("Encoder_Count11:%d\r\n", Encoder_Count);
    /* Add the overflow count to the counter value */
    Encoder_Count += Encoder_Overflow_Count2 * 65536;
    return Encoder_Count;
}
