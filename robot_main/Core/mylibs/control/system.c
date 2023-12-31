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

extern ScanPoint_t obstacleAngleAndDistances[RobotNumber];
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
    double nearestSourisAngle = 0.0;
    double nearestSourisDistance = 0.0;
    while (1)
    {
        /* receive notification */
        if (ulTaskNotifyTake(pdTRUE, portMAX_DELAY) > 0)
        {
            if (obstacleAngleAndDistances[FirstRobot].distance < obstacleAngleAndDistances[SecondRobot].distance)
            {
                nearestSourisAngle = obstacleAngleAndDistances[FirstRobot].angle;
                nearestSourisDistance = obstacleAngleAndDistances[FirstRobot].distance;
            }
            else
            {
                nearestSourisAngle = obstacleAngleAndDistances[SecondRobot].angle;
                nearestSourisDistance = obstacleAngleAndDistances[SecondRobot].distance;
            }

            if (nearestSourisAngle > 0 && nearestSourisAngle < 180)
            {
                changeAngle(nearestSourisAngle);
                goStraight(300);
            }
            else if (nearestSourisAngle > 180 && nearestSourisAngle < 360)
            {
                changeAngle(360 - nearestSourisAngle);
                goStraight(300);
            }
            else
            {
                printf("Chat task is not running\r\n");
            }
            vTaskDelay(100);
        }
    }
}

#define MaxChatKeepAngleCount 3
static void souris(void)
{
    double obstacleAngle = 0.0;
    double obstacleDistance = 0.0;
    uint8_t ChatRobot = 0;
    double chatAngle[RobotNumber][MaxChatKeepAngleCount];
    double chatDistance[RobotNumber][MaxChatKeepAngleCount];
    uint8_t chatAngleCount = 0;
    while (1)
    {
        if (ulTaskNotifyTake(pdTRUE, portMAX_DELAY) > 0)
        {
            if (chatAngleCount < MaxChatKeepAngleCount)
            {
                chatAngle[FirstRobot][chatAngleCount] = obstacleAngleAndDistances[FirstRobot].angle;
                chatAngle[SecondRobot][chatAngleCount] = obstacleAngleAndDistances[SecondRobot].angle;
                chatDistance[FirstRobot][chatAngleCount] = obstacleAngleAndDistances[FirstRobot].distance;
                chatDistance[SecondRobot][chatAngleCount] = obstacleAngleAndDistances[SecondRobot].distance;
                chatAngleCount++;
            }
            else
            {
                chatAngleCount = 0;
                if ((chatAngle[FirstRobot][0] - chatAngle[FirstRobot][MaxChatKeepAngleCount - 1]) > -3.0 && (chatAngle[FirstRobot][0] - chatAngle[FirstRobot][MaxChatKeepAngleCount - 1]) < 3.0 && (chatDistance[FirstRobot][1] - chatDistance[FirstRobot][0]) < 0)
                {
                    ChatRobot = FirstRobot;
                }
                else if ((chatAngle[SecondRobot][0] - chatAngle[SecondRobot][MaxChatKeepAngleCount - 1]) > -3.0 && (chatAngle[SecondRobot][0] - chatAngle[SecondRobot][MaxChatKeepAngleCount - 1]) < 3.0 && (chatDistance[SecondRobot][1] - chatDistance[SecondRobot][0]) < 0)
                {
                    ChatRobot = SecondRobot;
                }
                else
                {
                    if (chatDistance[FirstRobot][MaxChatKeepAngleCount - 1] < chatDistance[SecondRobot][MaxChatKeepAngleCount - 1])
                    {
                        ChatRobot = FirstRobot;
                    }
                    else
                    {
                        ChatRobot = SecondRobot;
                    }
                }
                obstacleAngle = obstacleAngleAndDistances[ChatRobot].angle;
                obstacleDistance = obstacleAngleAndDistances[ChatRobot].distance;
                if (obstacleAngle > 0 && obstacleAngle < 180)
                {
                    changeAngle(270);
                    goStraight(200);
                }
                else if (obstacleAngle > 180 && obstacleAngle < 360)
                {
                    changeAngle(90);
                    goStraight(200);
                }
            }
        }
    }
}

static void timerInit(void)
{
    HAL_TIM_Base_Start_IT(&htim1);
    HAL_TIM_Base_Start_IT(&htim3);
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
    __set_PRIMASK(1);
    // __set_FAULTMASK(1);
    // reset the system
    NVIC_SystemReset();
}

/**
 * @brief create the system task
 *
 */
void createSystemTask(void)
{
    // Task Handle
    xTaskCreate(systemControl, "systemControl", 512, NULL, 1, &AvoidTask_Handler);
}
