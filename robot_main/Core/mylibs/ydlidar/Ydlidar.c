/**
 * @file Ydlidar.c
 * @author JiangboWANG
 * @brief
 * @version 0.1
 * @date 2024-01-09
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "Ydlidar.h"
#include "ydlidar_protocol.h"
#include "math.h"
#include "string.h"
#include "myuart.h"

extern ydlidar_t ydlidar;
extern UART_HandleTypeDef huart4;

SemaphoreHandle_t semYdlidarUartrRead;
QueueHandle_t obstacleAngleAndDistanceQueue;

ScanPoint_t obstacleAngleAndDistances[RobotNumber] = {0};
uint8_t obstacleAngleAndDistancesIndex = 0;

ydlidar_data_process_status_t dataProcessStatus = WAITING_DATA;
static bool isFirstRobot = true;
static bool NotFirstTimeReceiveData = false;

uint8_t ydlidarUartRawData[MAX_SCAN_BUFFER_SIZE][MAX_SCAN_POINTS];
uint8_t SCAN_CIRCLE_INDEX = 0;
uint8_t PROCESS_SCAN_DATA_INDEX = 0;

// #define YDLIDAR_DEBUG
// #define YDLIDAR_DEBUG_LEVEL_2

/**
 * @brief init the ydlidar
 *
 */
void YdlidarInit(void)
{
    // init the ydlidar function
    ydlidar.func.send_command = uartSendCommand;
    ydlidar.func.receive_response = uartReceiveResponse;
    ydlidar.func.receive_data_dma = uartReceiveDataDMA;
}

/**
 * @brief Ydlidar start scan function with dma
 *        we use two buffer to store the scan data, one for process, one for receive
 *        for example: when buffer1 is processing, buffer2 is receiving,and receiving speed is faster than processing speed
 *        so when buffer1 is processed, buffer2 is already received, so after buffer1 is processed, we can process buffer2,
 *        and then save data to buffer1, and then process buffer1, and so on
 */
void YdlidarReceivedScanDataWithDMA(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    if (dataProcessStatus == WAITING_DATA && NotFirstTimeReceiveData == true)
    {
        xSemaphoreGiveFromISR(semYdlidarUartrRead, &xHigherPriorityTaskWoken);
        SCAN_CIRCLE_INDEX = (SCAN_CIRCLE_INDEX + 1) % MAX_SCAN_BUFFER_SIZE;
    }
    startReceiveScanData(ydlidarUartRawData[SCAN_CIRCLE_INDEX], sizeof(ydlidarUartRawData[SCAN_CIRCLE_INDEX]));
    NotFirstTimeReceiveData = true;
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/**
 * @brief  This function is used to restart the scan
 *
 */
void restartScan(void)
{
    stopScan();
    HAL_Delay(100);
    ydlidar_device_info_t deviceinfo;
    // get device info until success
    while (getDeviceInfo(&deviceinfo) != RESULT_OK)
    {
        // reset the microcontroller
        printf("YDLIDAR get DeviceInfo Error!!!\r\n");
        softReset();
        HAL_Delay(1000);
    }
    printf("[YDLIDAR INFO] Connection established in [%s]\r\n", deviceinfo.model == YDLIDAR_MODEL_X4 ? "X4" : "NOT MODEL X4");
    printf("[YDLIDAR INFO] Firmware version: %d.%d\r\n", deviceinfo.major_firmware_version, deviceinfo.minor_firmware_version);
    printf("[YDLIDAR INFO] Hardware version: %d\r\n", deviceinfo.hardware_version);
    printf("[YDLIDAR INFO] Serial number: ");
    for (int i = 0; i < 16; i++)
    {
        printf("%x", deviceinfo.serialnum[i]);
    }
    printf("\r\n");
    ydlidar_device_health_t healthinfo;
    HAL_Delay(100);
    // get device health until success
    while (getDeviceHealth(&healthinfo) != RESULT_OK)
    {
        printf("cannot retrieve YDLIDAR health\r\n ");
        HAL_Delay(100);
    }
    printf("[YDLIDAR INFO] YDLIDAR running correctly! The health status: %s\r\n", healthinfo.status == 0 ? "well" : "bad");
    if (startScan() == RESULT_OK)
    {
        YdlidarReceivedScanDataWithDMA();
        printf("[YDLIDAR INFO] Now YDLIDAR is scanning ...... \r\n\r\n");
    }
    else
    {
        printf("[YDLIDAR ERROR] start YDLIDAR is failed!  Continue........ \r\n");
    }
}
void checkIfIsSameRobotData(const double oldObstacleStartAngle, const double oldObstacleEndAngle, const double oldObstacleDistance, const double oldobstacleAngle, const double obstacleStartAngle, double const obstacleEndAngle, double *obstacleAngle, double *obstacleDistance)
{
    // printf("[checkIfIsSameRobotData] oldObstacleStartAngle = %f, oldObstacleEndAngle = %f, oldObstacleDistance = %f\r\n", oldObstacleStartAngle, oldObstacleEndAngle, oldObstacleDistance);
    // printf("[checkIfIsSameRobotData] obstacleStartAngle = %f, obstacleEndAngle = %f, obstacleDistance = %f\r\n", obstacleStartAngle, obstacleEndAngle, *obstacleDistance);
    if (fabs(oldObstacleEndAngle - obstacleStartAngle) < 2.0)
    {
        if (oldObstacleStartAngle < obstacleEndAngle)
        {
            *obstacleAngle = (oldObstacleStartAngle + obstacleEndAngle) / 2;
            *obstacleDistance = (oldObstacleDistance + *obstacleDistance) / 2;
        }
        else
        {
            *obstacleAngle = oldObstacleStartAngle + (360 - oldObstacleStartAngle + obstacleEndAngle) / 2;
            if (*obstacleAngle > 360.0)
            {
                *obstacleAngle -= 360;
            }
            // printf("[checkIfIsSameRobotData] *obstacleAngle1 = %f\r\n", *obstacleAngle);
            *obstacleDistance = (oldObstacleDistance + *obstacleDistance) / 2;
        }
    }
    else if (fabs(oldObstacleStartAngle - obstacleEndAngle) < 2.0)
    {
        if (obstacleStartAngle < oldObstacleEndAngle)
        {
            *obstacleAngle = (oldObstacleEndAngle + obstacleStartAngle) / 2;
            *obstacleDistance = (oldObstacleDistance + *obstacleDistance) / 2;
        }
        else
        {
            *obstacleAngle = obstacleStartAngle + (360 - obstacleStartAngle + oldObstacleEndAngle) / 2;
            if (*obstacleAngle > 360.0)
            {
                *obstacleAngle -= 360;
            }
            // printf("[checkIfIsSameRobotData] *obstacleAngle2 = %f\r\n", *obstacleAngle);
            *obstacleDistance = (oldObstacleDistance + *obstacleDistance) / 2;
        }
    }
    else if (fabs(oldObstacleStartAngle - obstacleStartAngle) < 2.0 && fabs(oldObstacleEndAngle - obstacleEndAngle) < 2.0)
    {
        // printf("[checkIfIsSameRobotData] receive the same data\r\n");
        *obstacleAngle = oldobstacleAngle;
        *obstacleDistance = oldObstacleDistance;
    }
}

bool checkIfIsSameRobot(const double oldObstacleAgnle, const double obstacleAngle)
{
    // printf("[checkIfIsSameRobot] oldObstacleAgnle = %f, obstacleAngle = %f\r\n", oldObstacleAgnle, obstacleAngle);
    if (fabs(oldObstacleAgnle - obstacleAngle) < 2.0)
    {
        // printf("[checkIfIsSameRobot] is same robot\r\n");
        return true;
    }
    else
    {
        // printf("[checkIfIsSameRobot] is not same robot\r\n");
        return false;
    }
}

/**
 * @brief  This function is used to get the obstacle angle after process,
 *         the obstacle angle is the angle of the obstacle which is the nearest to the robot,
 *         the obstacle angle is in the range of [0, 360]
 *         for chat: this angle is used to hit the souris
 *         for souris: this angle is used to avoid the obstacle[the chat]
 */
bool getAngleAndDistanceAfterProcess(double *angles, double *distances, int LSN)
{
    double obstacleStartAngle = 0.0;
    double obstacleEndAngle = 0.0;
    double obstacleDistance = 0.0;
    static double oldObstacleStartAngle = 0.0;
    static double oldObstacleEndAngle = 0.0;
    static double oldObstacleDistance = 0.0;
    static double oldObstacleAngle = 0.0;
    static bool isFirstTime = true;
    static bool isSameRobot = false;
    uint8_t usefullDistanceCount = 0;
    for (uint8_t i = 0; i < LSN; i++)
    {
        if (distances[i] > MAX_DISTANCE)
        {
            distances[i] = 0;
            continue;
        }
        else if (distances[i] == 0)
        {
            continue;
        }
        else if (distances[i] > 0 && distances[i] < MAX_DISTANCE)
        {
            usefullDistanceCount++;
        }
        // printf("[INFO] angles[%d] = %f: distances[%d] = %f\r\n", i, angles[i], i, distances[i]);
    }
    if (usefullDistanceCount <= 10)
    {
        return false;
    }
    else
    {
        for (uint8_t i = 0; i < LSN; i++)
        {
            if (distances[i] == 0)
            {
                continue;
            }
            else
            {
                obstacleStartAngle = angles[i];
                for (uint8_t j = i + 1; j < LSN; j++)
                {
                    if (j == LSN - 1)
                    {
                        obstacleEndAngle = angles[j];
                    }
                    if (distances[j] == 0 || j == LSN - 1)
                    {
                        obstacleDistance = distances[(i + j) / 2];
                        // printf("[INFO] index = %d, obstacleDistance = %f\r\n", (i + j) / 2, obstacleDistance);
                        break;
                    }
                    else
                    {
                        obstacleEndAngle = angles[j];
                    }
                }
                // printf("[INFO] obstacleStartAngle = %f, obstacleEndAngle = %f, obstacleDistance = %f\r\n", obstacleStartAngle, obstacleEndAngle, obstacleDistance);
                break;
            }
        }
        double obstacleAngle = 0;
        if (obstacleStartAngle < obstacleEndAngle)
        {
            obstacleAngle = (obstacleStartAngle + obstacleEndAngle) / 2;
        }
        else
        {
            obstacleAngle = obstacleStartAngle + (360 - obstacleStartAngle + obstacleEndAngle) / 2;
        }
        if (isFirstTime == true)
        {
            isFirstTime = false;
            oldObstacleStartAngle = obstacleStartAngle;
            oldObstacleEndAngle = obstacleEndAngle;
            oldObstacleDistance = obstacleDistance;
            oldObstacleAngle = obstacleAngle;
            return false;
        }
        checkIfIsSameRobotData(oldObstacleStartAngle, oldObstacleEndAngle, oldObstacleDistance, oldObstacleAngle, obstacleStartAngle, obstacleEndAngle, &obstacleAngle, &obstacleDistance);
        // isSameRobot = checkIfIsSameRobot(oldObstacleAngle, obstacleAngle);
        // if (isSameRobot == true)
        // {
        //     printf("[INFO] isSameRobot = true\r\n");
        //     return false;
        // }
        oldObstacleStartAngle = obstacleStartAngle;
        oldObstacleEndAngle = obstacleEndAngle;
        oldObstacleDistance = obstacleDistance;
        oldObstacleAngle = obstacleAngle;
        if (isFirstRobot == true)
        {
            isFirstRobot = false;
            obstacleAngleAndDistances[FirstRobot].angle = obstacleAngle;
            obstacleAngleAndDistances[FirstRobot].distance = obstacleDistance;
            // printf("[FirstRobot INFO] obstacleAngleAndDistances[FirstRobot].angle = %f,distance = %f\r\n", obstacleAngleAndDistances[FirstRobot].angle, obstacleAngleAndDistances[FirstRobot].distance);
            return false;
        }
        else
        {
            obstacleAngleAndDistances[SecondRobot].angle = obstacleAngle;
            obstacleAngleAndDistances[SecondRobot].distance = obstacleDistance;
            // printf("[SecondRobot INFO] obstacleAngleAndDistances[SecondRobot].angle = %f,distance = %f\r\n", obstacleAngleAndDistances[SecondRobot].angle, obstacleAngleAndDistances[SecondRobot].distance);
            return true;
        }
    }
}

/**
 * @brief  This function is used to process the scan data
 *
 */
void dataProcess(void)
{
    dataProcessStatus = WAITING_DATA;
    ydlidar_data_packet_t *data_packet = NULL;
    xSemaphoreTake(semYdlidarUartrRead, portMAX_DELAY);
    dataProcessStatus = PROCESSING_DATA;
    bool getTwoRobotObstacle = false;
    uint8_t *data = (uint8_t *)&ydlidarUartRawData[PROCESS_SCAN_DATA_INDEX];
    uint16_t checkSumResult = 0;
    uint16_t dataIndex = 0;
    while (dataIndex < MAX_SCAN_POINTS)
    {
        // printf("dataIndex = %d\r\n", dataIndex);
        if (data[dataIndex] == 0xAA && data[dataIndex + 1] == 0x55) // check the start flag
        {
            data_packet = (ydlidar_data_packet_t *)&data[dataIndex];
#ifdef YDLIDAR_DEBUG
            for (int j = 0; j < sizeof(ydlidar_data_packet_t); j++)
            {
                printf("[DEBUG] data[%d] = %2x\r\n", dataIndex + j, data[dataIndex + j]);
            }
            printf("[DEBUG] data_packet->start_flag = %4x\r\n", data_packet->start_flag);
            printf("[DEBUG] data_packet->type_CT = %2x\r\n", data_packet->type_CT);
            printf("[DEBUG] data_packet->size_LSN = %2x\r\n", data_packet->size_LSN);
            printf("[DEBUG] data_packet->startAngle_FSA = %4x\r\n", data_packet->startAngle_FSA);
            printf("[DEBUG] data_packet->endAngle_LSA = %4x\r\n", data_packet->endAngle_LSA);
#endif
            if (data_packet->size_LSN == 1)
            {
                // continue; // skip the start data packet
                dataIndex += sizeof(ydlidar_data_packet_t) + data_packet->size_LSN * sizeof(uint16_t);
                continue;
            }
            if (dataIndex + sizeof(ydlidar_data_packet_t) + data_packet->size_LSN * sizeof(uint16_t) > MAX_SCAN_POINTS)
            {
                break;
            }
            checkSumResult = calculateChecksum((uint8_t *)&data[dataIndex], data_packet->size_LSN + 4); // calculate the checksum
            uint16_t sampleDatas_SI[data_packet->size_LSN];
            uint16_t *sampledata = (uint16_t *)&data[dataIndex + sizeof(ydlidar_data_packet_t)];

            for (int j = 0; j < data_packet->size_LSN; j++) // convert the data to uint16_t
            {
                sampleDatas_SI[j] = sampledata[j];
            }
            // #ifdef YDLIDAR_DEBUG_LEVEL_2
            // printf("[DEBUG] dataIndex = %d\r\n", dataIndex);
            // printf("[DEBUG] checkSumResult = %4x\r\n", checkSumResult);
            // printf("[DEBUG] data_packet->crc_CS = %4x\r\n", data_packet->crc_CS);
            // #endif
            if (checkSumResult == data_packet->crc_CS)
            {
                double distances[data_packet->size_LSN];
                double angles[data_packet->size_LSN];
                // parse the distance
                parseDistance(sampleDatas_SI, data_packet->size_LSN, distances);
                // parse the first level angle
                AngleFirstLevelParse(data_packet->startAngle_FSA, data_packet->endAngle_LSA, data_packet->size_LSN, (double *)angles);
                // parse the second level angle
                AngleSecondLevelParse((double *)angles, data_packet->size_LSN, (double *)distances);
                // get the obstacle angle after process
                getTwoRobotObstacle = getAngleAndDistanceAfterProcess((double *)angles,
                                                                      (double *)distances, data_packet->size_LSN);
                if (getTwoRobotObstacle)
                {
                    printf("[DEBUg]:send data to queue success\r\n");
                    for (int i = 0; i < RobotNumber; i++)
                    {
                        printf("[INFO] obstacleAngleAndDistances[%d].angle = %f,distance = %f\r\n", i, obstacleAngleAndDistances[i].angle, obstacleAngleAndDistances[i].distance);
                    }
                    xQueueOverwrite(obstacleAngleAndDistanceQueue, &obstacleAngleAndDistances);
                    isFirstRobot = true;

                    break;
                }
            }
            else
            {
                printf("[ERROR] Checksum error!,with dataindex = %d,checkSumResult = %4x, data_packet->crc_CS = %4x\r\n", dataIndex, checkSumResult, data_packet->crc_CS);
            }
            dataIndex += data_packet->size_LSN * sizeof(uint16_t) + sizeof(ydlidar_data_packet_t);
        }
        else
        {
            dataIndex++;
        }
    }
    memset(ydlidarUartRawData[PROCESS_SCAN_DATA_INDEX], 0, sizeof(ydlidarUartRawData[PROCESS_SCAN_DATA_INDEX]));
    PROCESS_SCAN_DATA_INDEX = (PROCESS_SCAN_DATA_INDEX + 1) % MAX_SCAN_BUFFER_SIZE;

    // stopScan();
}

// task to run the ydlidar
void task_ydlidar(void *argument)
{
    TickType_t lastWakeTime = getSysTickCnt();

    while (1)
    {
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(30));
        dataProcess();
        // printf("task_ydlidar is running\r\n");
    }
}

// create the task to start the scan
void createYdlidarTask(void)
{
    semYdlidarUartrRead = xSemaphoreCreateBinary();
    if (semYdlidarUartrRead == NULL)
    {
        printf("[ERROR]: Ydlidar Semaphore create failed.\r\n");
        softReset();
    }
    else
    {
        printf("[INFO]: Ydlidar Semaphore create success.\r\n");
    }
    obstacleAngleAndDistanceQueue = xQueueCreate(1, sizeof(ScanPoint_t) * RobotNumber);
    if (obstacleAngleAndDistanceQueue == NULL)
    {
        printf("[ERROR]: Ydlidar Queue create failed.\r\n");
        softReset();
    }
    else
    {
        printf("[INFO]: Ydlidar Queue create success.\r\n");
    }

    printf("[INFO]: Create YDLIDAR task.\r\n");
    xTaskCreate((TaskFunction_t)task_ydlidar, "task_ydlidar", Ydliar_TASK_STACK_SIZE, NULL, Ydliar_TASK_PRIORITY, NULL);
}
