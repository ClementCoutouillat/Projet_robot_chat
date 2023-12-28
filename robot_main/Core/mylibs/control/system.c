/**
 * @file system.c
 * @author Jiangbo WANG
 *
 * @brief  This project is for a game "chat and souris".
 *         If the robot is souris, it will run away from the cat,and avoid to be catched by the cat(another robot).
 *         If the robot is cat, it will catch the souris(another robot).
 *         The robot will use ydlidar to detect the obstacle, and use the data to control the car.
 *         And we need to create a task to control the car for both chat and souris.
 *         The task will be notified by ydlidar task, and then it will process the data and control the car.
 * @version 0.1
 * @date 2023-12-28
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "system.h"
#include "tim.h"
#include "motorInterface.h"

extern ScanPointData_t PointDataProcess[MaxScanPointCount];
extern uint8_t PointDataProcessIndex;
TaskHandle_t AvoidTask_Handler;
static void timerInit(void);
static void chat(void);
static void souris(void);
static bool isChat(void);

// static void souris(void);
static uint32_t sysTickCnt = 0;
uint32_t getSysTickCnt(void)
{
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) /*system is running*/
        return xTaskGetTickCount();
    else
        return sysTickCnt;
}

void systemInit(void)
{
    // Init the PID parameter
    PIDInit();
    // Init the motor parameter
    dcMotorInit();
    // Init the ydlidar parameter
    YdlidarInit();
    // Init the timer
    timerInit();
    // restart the scan
    restartScan();
}

void systemControl(void *argument)
{
    while (1)
    {
        if (isChat())
        {
            chat();
        }
        else
        {
            souris();
        }
        vTaskDelay(100);
    }
}

static bool isChat(void)
{
    if (BUMPER1_FLAG % 2 == 1 || BUMPER2_FLAG % 2 == 1 || BUMPER3_FLAG % 2 == 1 || BUMPER4_FLAG % 2 == 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

static void chat(void)
{
    while (1)
    {
        /* receive notification */
        if (ulTaskNotifyTake(pdTRUE, portMAX_DELAY) > 0)
        {
            printf("Chat task is running\r\n");
        }
        else
        {
            printf("Chat task is not running\r\n");
            vTaskDelay(100);
        }
    }
}

static void souris(void)
{
    while (1)
    {
        if (ulTaskNotifyTake(pdTRUE, portMAX_DELAY) > 0)
        {
            printf("Souris task is running\r\n");
        }
        else
        {
            printf("Souris task is not running\r\n");
            vTaskDelay(100);
        }
    }
}

static void timerInit(void)
{
    HAL_TIM_Base_Start_IT(&htim1);
    HAL_TIM_Base_Start_IT(&htim3);
    HAL_TIM_Base_Start_IT(&htim7);
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);

    HAL_TIM_PWM_Start(&htim17, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim16, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_1);
}

/**
 * @brief soft reset the system
 *
 */
void softReset(void)
{
    // close all interrupt
    // __set_PRIMASK(1);
    __set_FAULTMASK(1);
    // reset the system
    NVIC_SystemReset();
}

void createSystemTask(void)
{
    // Task Handle
    xTaskCreate(systemControl, "systemControl", 512, NULL, 1, &AvoidTask_Handler);
}
