#include "Ydlidar.h"
#include "ydlidar_protocol.h"
#include "main.h"
#include "math.h"
#include "string.h"
#include "myuart.h"

extern ydlidar_t ydlidar;
extern UART_HandleTypeDef huart4;

ScanPoint_t obstacleAngleAndDistances[RobotNumber] = {0};
uint8_t obstacleAngleAndDistancesIndex = 0;
SemaphoreHandle_t semYdlidarUartrRead;
ydlidar_data_process_status_t dataProcessStatus = WAITING_DATA;
static bool isFirstRobot = true;

uint8_t ydlidarUartRawData[MAX_SCAN_BUFFER_SIZE][MAX_SCAN_POINTS];
uint8_t SCAN_CIRCLE_INDEX = 0;
uint8_t PROCESS_SCAN_DATA_INDEX = 0;
bool receiveFlag = false;

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

void YdlidarReceivedScanDataWithDMA(void)
{
    // receive scan data use ydlidarUartRawData[SCAN_CIRCLE_INDEX] address and size
    startReceiveScanData(ydlidarUartRawData[SCAN_CIRCLE_INDEX], sizeof(ydlidarUartRawData[SCAN_CIRCLE_INDEX]));
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
        printf("\r\nNow YDLIDAR is scanning ...... \r\n");
    }
    else
    {
        printf("start YDLIDAR is failed!  Continue........ \r\n");
    }
}

/**
 * @brief  This function is used to process the scan data
 *
 */
void dataProcess(void)
{
    ydlidar_data_packet_t *data_packet = NULL;
    xSemaphoreTake(semYdlidarUartrRead, portMAX_DELAY);
    printf("receiveFlag = %d\r\n", receiveFlag);
    bool getTwoRobotObstacle = false;
    uint8_t *data = (uint8_t *)&ydlidarUartRawData[PROCESS_SCAN_DATA_INDEX];
    // #ifdef YDLIDAR_DEBUG_LEVEL_2
    for (int i = 0; i < MAX_SCAN_POINTS; i++)
    {
        printf("%02x ", data[i]);
        if (i % 10 == 0)
        {
            printf("| ");
        }
        if (i % 40 == 0)
        {
            printf("\r\n");
        }
    }
    printf("\r\n");
    // #endif
    uint16_t checkSumResult = 0;
    uint16_t dataIndex = 0;
    while (dataIndex < MAX_SCAN_POINTS)
    {
        printf("dataIndex = %d\r\n", dataIndex);
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
            printf("[DEBUG] dataIndex = %d\r\n", dataIndex);
            printf("[DEBUG] checkSumResult = %4x\r\n", checkSumResult);
            printf("[DEBUG] data_packet->crc_CS = %4x\r\n", data_packet->crc_CS);
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
                getTwoRobotObstacle = getAngleAndDistanceAfterProcess((double *)angles, (double *)distances, data_packet->size_LSN);
                if (getTwoRobotObstacle)
                {
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
    receiveFlag = false;
    memset(ydlidarUartRawData[PROCESS_SCAN_DATA_INDEX], 0, sizeof(ydlidarUartRawData[PROCESS_SCAN_DATA_INDEX]));
    PROCESS_SCAN_DATA_INDEX = (PROCESS_SCAN_DATA_INDEX + 1) % MAX_SCAN_BUFFER_SIZE;
    // stopScan();
}

/**
 * @brief  This function is used to get the obstacle angle after process,
 *         the obstacle angle is the angle of the obstacle which is the nearest to the robot,
 *         the obstacle angle is in the range of [0, 360]
 *         for chat: this angle is used to hit the souris
 *         for souris: this angle is used to avoid the obstacle[the chat]
 *
 */

bool getAngleAndDistanceAfterProcess(double *angles, double *distances, int LSN)
{
    double obstacleStartAngle = 0.0;
    double obstacleEndAngle = 0.0;
    double obstacleDistance = 0.0;
    uint8_t usefullDistanceCount = 0;
    for (uint8_t i = 0; i < LSN; i++)
    {
        if (distances[i] > 1000)
        {
            continue;
        }
        else
        {
            usefullDistanceCount++;
        }
    }
    if (usefullDistanceCount <= 2)
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
                    if (distances[j] == 0)
                    {
                        obstacleDistance = distances[(i + j) / 2];
                        break;
                    }
                    else
                    {
                        obstacleEndAngle = angles[j];
                    }
                }
                break;
            }
        }
        if (isFirstRobot == true)
        {
            isFirstRobot = false;
            obstacleAngleAndDistances[FirstRobot].angle = (obstacleStartAngle + obstacleEndAngle) / 2.0;
            obstacleAngleAndDistances[FirstRobot].distance = obstacleDistance;
            return false;
        }
        else
        {
            obstacleAngleAndDistances[SecondRobot].angle = (obstacleStartAngle + obstacleEndAngle) / 2.0;
            obstacleAngleAndDistances[SecondRobot].distance = obstacleDistance;
            return true;
        }
    }
}

// task to run the ydlidar
void task_ydlidar(void *argument)
{
    TickType_t lastWakeTime = getSysTickCnt();

    while (1)
    {
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(10));
        dataProcess();
        // xTaskNotifyGive(AvoidTask_Handler);
        // printf("task_ydlidar is running\r\n");
    }
}

// create the task to start the scan
void createYdlidarTask(void)
{
    semYdlidarUartrRead = xSemaphoreCreateBinary();
    if (semYdlidarUartrRead == NULL)
    {
        printf("[ERROR]: Semaphore create failed.\r\n"); // TODO: handle error
    }
    else
    {
        printf("[INFO]: Ydlidar uart semaphore create success.\r\n");
    }
    printf("[INFO]: Create YDLIDAR task.\r\n");
    xTaskCreate((TaskFunction_t)task_ydlidar, "task_ydlidar", 1024, NULL, 3, NULL);
}
