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
#include "queue.h"
#include "stdbool.h"

ScanPoint_t obstacleAngleAndDistancesRev[RobotNumber];
extern QueueHandle_t obstacleAngleAndDistanceQueue;
extern uint8_t PointDataProcessIndex;
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
    printf("[INFO]: System init success\r\n");
}
void souris1(void)
{
    moveDistance(0.5, 200);
    // task delay
    vTaskDelay(1000);
}

bool checkIfNeedChangeAngle(const double oldObstacleAgnle, const double obstacleAngle)
{
    // printf("[checkIfIsSameRobot] oldObstacleAgnle = %f, obstacleAngle = %f\r\n", oldObstacleAgnle, obstacleAngle);
    if (fabs(oldObstacleAgnle - obstacleAngle) < 20.0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool changeModeChat = true;
bool changeModeSouris = false;
void systemControl(void *argument)
{
    while (1)
    {
        if (xQueueReceive(obstacleAngleAndDistanceQueue, &obstacleAngleAndDistancesRev, portMAX_DELAY) == pdPASS)
        {
            // printf("obstacleAngleAndDistancesRev[FirstRobot].distance = %f\r\n", obstacleAngleAndDistancesRev[FirstRobot].distance);
            // printf("obstacleAngleAndDistancesRev[SecondRobot].distance = %f\r\n", obstacleAngleAndDistancesRev[SecondRobot].distance);
            if (changeModeChat)
            {
                chat();
            }
            else if (changeModeSouris)
            {
                souris();
            }
        }
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
static double oldObstacleAgnle = 0.0;
static double oldObstacleDistance = 0.0;

static void chat(void)
{
    static uint8_t acc = 0;
    double obstacleAngle = 0.0;
    double obstacleDistance = 0.0;
    uint8_t SourisRobot = 0;
    uint8_t anotherSouris = 0;
    uint8_t chatAngleCount = 0;
    static bool needCHangeAngle = false;

    chatAngleCount = 0;

    if (obstacleAngleAndDistancesRev[FirstRobot].distance < obstacleAngleAndDistancesRev[SecondRobot].distance)
    {
        SourisRobot = FirstRobot;
        anotherSouris = SecondRobot;
    }
    else
    {
        SourisRobot = SecondRobot;
        anotherSouris = FirstRobot;
    }
    obstacleAngle = obstacleAngleAndDistancesRev[SourisRobot].angle;
    obstacleDistance = obstacleAngleAndDistancesRev[SourisRobot].distance;

    if (fabs(oldObstacleAgnle - obstacleAngle) < 20.0)
    {
        needCHangeAngle = false;
    }
    else
    {
        needCHangeAngle = true;
    }
    uint16_t speed1 = 160 + acc;
    uint16_t speed2 = 200 + acc;
    if (speed1 > 290)
    {
        speed1 = 290;
    }
    if (speed2 > 290)
    {
        speed2 = 290;
    }
    if (obstacleDistance < MAX_DISTANCE && obstacleDistance > MIN_DISTANCE)
    {
        if (obstacleAngle > 0 && obstacleAngle < 180)
        {
            if (needCHangeAngle)
            {
                changeAngle(obstacleAngle, "right");
            }

            moteur_controle_dPWM(speed1, speed2);
            moteur_controle_dPWM(160, 200);
        }
        else if (obstacleAngle > 180 && obstacleAngle < 360)
        {
            if (needCHangeAngle)
            {
                changeAngle(360 - obstacleAngle, "left");
            }
            moteur_controle_dPWM(160, 200);
        }
    }
    else
    {
        moteur_controle_dPWM(0, 0);
        if (obstacleDistance < MIN_DISTANCE)
        {
            changeModeChat = false;
            changeModeSouris = true;
        }
    }
    oldObstacleAgnle = obstacleAngle;
    oldObstacleDistance = obstacleDistance;
    acc += 5;
}
static void chat1(void)
{
    double nearestSourisAngle = 0.0;
    double nearestSourisDistance = 0.0;
    /* receive notification */
    if (xQueueReceive(obstacleAngleAndDistanceQueue, &obstacleAngleAndDistancesRev, portMAX_DELAY) == pdPASS)
    {
        if (obstacleAngleAndDistancesRev[FirstRobot].distance < obstacleAngleAndDistancesRev[SecondRobot].distance)
        {
            nearestSourisAngle = obstacleAngleAndDistancesRev[FirstRobot].angle;
            nearestSourisDistance = obstacleAngleAndDistancesRev[FirstRobot].distance;
        }
        else
        {
            nearestSourisAngle = obstacleAngleAndDistancesRev[SecondRobot].angle;
            nearestSourisDistance = obstacleAngleAndDistancesRev[SecondRobot].distance;
        }

        if (nearestSourisAngle > 0 && nearestSourisAngle < 180)
        {
            changeAngle(nearestSourisAngle, "left");
            goStraight(300);
        }
        else if (nearestSourisAngle > 180 && nearestSourisAngle < 360)
        {
            changeAngle(360 - nearestSourisAngle, "right");
            goStraight(300);
        }
        else
        {
            printf("Chat task is not running\r\n");
        }
    }
}
static void souris(void)
{
    double obstacleAngle = 0.0;
    double obstacleDistance = 0.0;
    uint8_t ChatRobot = 0;
    uint8_t anotherSouris = 0;
    uint8_t chatAngleCount = 0;

    chatAngleCount = 0;
    if (obstacleAngleAndDistancesRev[FirstRobot].distance < obstacleAngleAndDistancesRev[SecondRobot].distance)
    {
        ChatRobot = FirstRobot;
        anotherSouris = SecondRobot;
    }
    else
    {
        ChatRobot = SecondRobot;
        anotherSouris = FirstRobot;
    }
    obstacleAngle = obstacleAngleAndDistancesRev[ChatRobot].angle;
    obstacleDistance = obstacleAngleAndDistancesRev[ChatRobot].distance;
    if (obstacleDistance < MAX_DISTANCE && obstacleDistance > MIN_DISTANCE)
    {
        if (obstacleAngle > 0 && obstacleAngle < 180)
        {
            changeAngle(90, "left");
            moveDistance(0.4, 200);
            // changeAngle(90, "left");
            // moveDistance(0.8, 200);
            // changeAngle(180, "left");
        }
        else if (obstacleAngle > 180 && obstacleAngle < 360)
        {
            changeAngle(90, "right");
            // goStraight(0.4);
            // changeAngle(90, "right");
            moveDistance(0.4, 200);
            // changeAngle(180, "right");
        }
    }
    else
    {
        if (obstacleDistance < MIN_DISTANCE)
        {
            moteur_controle_dPWM(0, 0);
            changeModeChat = true;
            changeModeSouris = false;
            HAL_Delay(1000);
        }
    }
}

#define MaxChatKeepAngleCount 3
static void souris12(void)
{
    printf("souris task is running\r\n");
    double obstacleAngle = 0.0;
    double obstacleDistance = 0.0;
    uint8_t ChatRobot = 0;
    uint8_t anotherSouris = 0;
    double chatAngle[RobotNumber][MaxChatKeepAngleCount];
    double chatDistance[RobotNumber][MaxChatKeepAngleCount];
    uint8_t chatAngleCount = 0;
    if (chatAngleCount < MaxChatKeepAngleCount)
    {
        chatAngle[FirstRobot][chatAngleCount] = obstacleAngleAndDistancesRev[FirstRobot].angle;
        chatAngle[SecondRobot][chatAngleCount] = obstacleAngleAndDistancesRev[SecondRobot].angle;
        chatDistance[FirstRobot][chatAngleCount] = obstacleAngleAndDistancesRev[FirstRobot].distance;
        chatDistance[SecondRobot][chatAngleCount] = obstacleAngleAndDistancesRev[SecondRobot].distance;
        chatAngleCount++;
    }
    else
    {
        printf("souris task is running11\r\n");
        chatAngleCount = 0;
        if ((chatAngle[FirstRobot][0] - chatAngle[FirstRobot][MaxChatKeepAngleCount - 1]) > -3.0 && (chatAngle[FirstRobot][0] - chatAngle[FirstRobot][MaxChatKeepAngleCount - 1]) < 3.0 && (chatDistance[FirstRobot][1] - chatDistance[FirstRobot][0]) < 0)
        {
            ChatRobot = FirstRobot;
            anotherSouris = SecondRobot;
        }
        else if ((chatAngle[SecondRobot][0] - chatAngle[SecondRobot][MaxChatKeepAngleCount - 1]) > -3.0 && (chatAngle[SecondRobot][0] - chatAngle[SecondRobot][MaxChatKeepAngleCount - 1]) < 3.0 && (chatDistance[SecondRobot][1] - chatDistance[SecondRobot][0]) < 0)
        {
            ChatRobot = SecondRobot;
            anotherSouris = FirstRobot;
        }
        else
        {
            if (chatDistance[FirstRobot][MaxChatKeepAngleCount - 1] < chatDistance[SecondRobot][MaxChatKeepAngleCount - 1])
            {
                ChatRobot = FirstRobot;
                anotherSouris = SecondRobot;
            }
            else
            {
                ChatRobot = SecondRobot;
                anotherSouris = FirstRobot;
            }
            printf("souris task is running22\r\n");
        }
        obstacleAngle = obstacleAngleAndDistancesRev[ChatRobot].angle;
        obstacleDistance = obstacleAngleAndDistancesRev[ChatRobot].distance;
        if (obstacleAngle > 0 && obstacleAngle < 180)
        {
            changeAngle(90, "left");
            moveDistance(0.4, 200);
            changeAngle(90, "left");
            moveDistance(0.8, 200);
            changeAngle(180, "left");
        }
        else if (obstacleAngle > 180 && obstacleAngle < 360)
        {
            changeAngle(90, "right");
            goStraight(0.4);
            changeAngle(90, "right");
            goStraight(0.8);
            changeAngle(180, "right");
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
    printf("[INFO]: Create system task\r\n");
    xTaskCreate(systemControl, "systemControl", System_TASK_STACK_SIZE, NULL, System_TASK_PRIORITY, NULL);
}
