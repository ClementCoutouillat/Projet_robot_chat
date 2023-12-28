/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    stm32g0xx_it.c
 * @brief   Interrupt Service Routines.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32g0xx_it.h"
#include "FreeRTOS.h"
#include "task.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "stdbool.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */
bool BORDER_BACK_FLAG = false;
bool BORDER_FRONT_FLAG = false;
int16_t BUMPER1_FLAG = 0;
int16_t BUMPER2_FLAG = 0;
int16_t BUMPER3_FLAG = 0;
int16_t BUMPER4_FLAG = 0;
/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim7;
extern DMA_HandleTypeDef hdma_usart4_rx;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart4;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M0+ Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
 * @brief This function handles Non maskable interrupt.
 */
void NMI_Handler(void)
{
    /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

    /* USER CODE END NonMaskableInt_IRQn 0 */
    /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
    while (1)
    {
    }
    /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
 * @brief This function handles Hard fault interrupt.
 */
void HardFault_Handler(void)
{
    /* USER CODE BEGIN HardFault_IRQn 0 */

    /* USER CODE END HardFault_IRQn 0 */
    while (1)
    {
        /* USER CODE BEGIN W1_HardFault_IRQn 0 */
        /* USER CODE END W1_HardFault_IRQn 0 */
    }
}

/**
 * @brief This function handles System tick timer.
 */
void SysTick_Handler(void)
{
    /* USER CODE BEGIN SysTick_IRQn 0 */

    /* USER CODE END SysTick_IRQn 0 */
    HAL_IncTick();
#if (INCLUDE_xTaskGetSchedulerState == 1)
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
    {
#endif /* INCLUDE_xTaskGetSchedulerState */
        xPortSysTickHandler();
#if (INCLUDE_xTaskGetSchedulerState == 1)
    }
#endif /* INCLUDE_xTaskGetSchedulerState */
       /* USER CODE BEGIN SysTick_IRQn 1 */

    /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32G0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32g0xx.s).                    */
/******************************************************************************/

/**
 * @brief This function handles EXTI line 2 and line 3 interrupts.
 */
void EXTI2_3_IRQHandler(void)
{
    /* USER CODE BEGIN EXTI2_3_IRQn 0 */
    // If border_back interruption
    if (__HAL_GPIO_EXTI_GET_IT(BORDER_BACK_Pin) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(BORDER_BACK_Pin);
        char msg[] = "BORDER_BACK \n\r";
        HAL_UART_Transmit(&huart2, (uint8_t *)msg, sizeof(msg), HAL_MAX_DELAY);
        HAL_GPIO_TogglePin(GPIOB, LED_ORANGE_Pin);
        // TODO : fonction_change_direction(direction);
        BORDER_BACK_FLAG = true;
    }
    // If border_back interruption
    if (__HAL_GPIO_EXTI_GET_IT(BORDER_FRONT_Pin) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(BORDER_FRONT_Pin);

        char msg[] = "BORDER_FRONT \n\r";
        HAL_UART_Transmit(&huart2, (uint8_t *)msg, sizeof(msg), HAL_MAX_DELAY);
        HAL_GPIO_TogglePin(GPIOB, LED_ORANGE_Pin);
        // TODO : fonction_change_direction(direction);
        BORDER_FRONT_FLAG = true;
    }
    /* USER CODE END EXTI2_3_IRQn 0 */
    HAL_GPIO_EXTI_IRQHandler(BORDER_BACK_Pin);
    HAL_GPIO_EXTI_IRQHandler(BORDER_FRONT_Pin);
    /* USER CODE BEGIN EXTI2_3_IRQn 1 */

    /* USER CODE END EXTI2_3_IRQn 1 */
}

/**
 * @brief This function handles EXTI line 4 to 15 interrupts.
 */
void EXTI4_15_IRQHandler(void)
{
    /* USER CODE BEGIN EXTI4_15_IRQn 0 */
    if (__HAL_GPIO_EXTI_GET_IT(BUMPER1_Pin) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(BUMPER1_Pin);

        char msg[] = "Contact Bumper 1 \n\r";
        HAL_UART_Transmit(&huart2, (uint8_t *)msg, sizeof(msg), HAL_MAX_DELAY);
        BUMPER1_FLAG += 1;

        // TODO : SET/RESET Variable 'Chat'
        // TODO : Change Direction
    }

    if (__HAL_GPIO_EXTI_GET_IT(BUMPER2_Pin) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(BUMPER2_Pin);

        char msg[] = "Contact Bumper 2 \n\r";
        HAL_UART_Transmit(&huart2, (uint8_t *)msg, sizeof(msg), HAL_MAX_DELAY);
        BUMPER2_FLAG += 1;

        // TODO : SET/RESET Variable 'Chat'
        // TODO : Change Direction
    }
    if (__HAL_GPIO_EXTI_GET_IT(BUMPER3_Pin) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(BUMPER3_Pin);

        char msg[] = "Contact Bumper 3 \n\r";
        HAL_UART_Transmit(&huart2, (uint8_t *)msg, sizeof(msg), HAL_MAX_DELAY);
        BUMPER3_FLAG += 1;

        // TODO : SET/RESET Variable 'Chat'
        // TODO : Change Direction
    }
    if (__HAL_GPIO_EXTI_GET_IT(BUMPER4_Pin) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(BUMPER4_Pin);

        char msg[] = "Contact Bumper 4 \n\r";
        HAL_UART_Transmit(&huart2, (uint8_t *)msg, sizeof(msg), HAL_MAX_DELAY);
        BUMPER4_FLAG += 1;

        // TODO : SET/RESET Variable 'Chat'
        // TODO : Change Direction
    }
    /* USER CODE END EXTI4_15_IRQn 0 */
    HAL_GPIO_EXTI_IRQHandler(BUMPER4_Pin);
    HAL_GPIO_EXTI_IRQHandler(BUMPER3_Pin);
    HAL_GPIO_EXTI_IRQHandler(BUMPER2_Pin);
    HAL_GPIO_EXTI_IRQHandler(BUMPER1_Pin);
    /* USER CODE BEGIN EXTI4_15_IRQn 1 */

    /* USER CODE END EXTI4_15_IRQn 1 */
}

/**
 * @brief This function handles DMA1 channel 1 interrupt.
 */
void DMA1_Channel1_IRQHandler(void)
{
    /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */

    /* USER CODE END DMA1_Channel1_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_usart4_rx);
    /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */

    /* USER CODE END DMA1_Channel1_IRQn 1 */
}

/**
 * @brief This function handles TIM1 break, update, trigger and commutation interrupts.
 */
void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
    /* USER CODE BEGIN TIM1_BRK_UP_TRG_COM_IRQn 0 */

    /* USER CODE END TIM1_BRK_UP_TRG_COM_IRQn 0 */
    HAL_TIM_IRQHandler(&htim1);
    /* USER CODE BEGIN TIM1_BRK_UP_TRG_COM_IRQn 1 */

    /* USER CODE END TIM1_BRK_UP_TRG_COM_IRQn 1 */
}

/**
 * @brief This function handles TIM1 capture compare interrupt.
 */
void TIM1_CC_IRQHandler(void)
{
    /* USER CODE BEGIN TIM1_CC_IRQn 0 */

    /* USER CODE END TIM1_CC_IRQn 0 */
    HAL_TIM_IRQHandler(&htim1);
    /* USER CODE BEGIN TIM1_CC_IRQn 1 */

    /* USER CODE END TIM1_CC_IRQn 1 */
}

/**
 * @brief This function handles TIM3 global interrupt.
 */
void TIM3_IRQHandler(void)
{
    /* USER CODE BEGIN TIM3_IRQn 0 */

    /* USER CODE END TIM3_IRQn 0 */
    HAL_TIM_IRQHandler(&htim3);
    /* USER CODE BEGIN TIM3_IRQn 1 */

    /* USER CODE END TIM3_IRQn 1 */
}

/**
 * @brief This function handles TIM7 global interrupt.
 */
void TIM7_IRQHandler(void)
{
    /* USER CODE BEGIN TIM7_IRQn 0 */

    /* USER CODE END TIM7_IRQn 0 */
    HAL_TIM_IRQHandler(&htim7);
    /* USER CODE BEGIN TIM7_IRQn 1 */

    /* USER CODE END TIM7_IRQn 1 */
}

/**
 * @brief This function handles USART2 global interrupt / USART2 wake-up interrupt through EXTI line 26.
 */
void USART2_IRQHandler(void)
{
    /* USER CODE BEGIN USART2_IRQn 0 */

    /* USER CODE END USART2_IRQn 0 */
    HAL_UART_IRQHandler(&huart2);
    /* USER CODE BEGIN USART2_IRQn 1 */

    /* USER CODE END USART2_IRQn 1 */
}

/**
 * @brief This function handles USART3 and USART4 interrupts.
 */
void USART3_4_IRQHandler(void)
{
    /* USER CODE BEGIN USART3_4_IRQn 0 */

    /* USER CODE END USART3_4_IRQn 0 */
    HAL_UART_IRQHandler(&huart4);
    /* USER CODE BEGIN USART3_4_IRQn 1 */

    /* USER CODE END USART3_4_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
