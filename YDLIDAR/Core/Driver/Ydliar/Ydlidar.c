#include "Ydlidar.h"
#include "main.h"
#include "math.h"
ydlidar_t ydlidar;
ScanPoint_t scanBuffer[2][MAX_SCAN_POINTS];
int currentBuffer = 0;
int currentPointIndex = 0;
extern UART_HandleTypeDef huart1;

// #define YDLIDAR_DEBUG  //TODO:comment this line to disable debug mode

/**
 * @brief This function is used to calculate the checksum of the data CRC16
 *
 * @param data  The data to be calculated
 * @param length  The length of the data to be calculated
 * @return uint16_t
 */
uint16_t calculateChecksum(uint16_t *data, size_t length)
{
    uint16_t checksum = 0;
    for (size_t i = 0; i < length; i++)
    {
        checksum ^= data[i];
    }

    return checksum;
}

/**
 * @brief Get the Device Info object
 *
 * @param device_info  The device info Struct Pointer
 * @return result_t  RESULT_OK or RESULT_FAIL
 */
result_t getDeviceInfo(ydlidar_device_info_t *device_info)
{
    ydlidar_response_header_t response_header;
    sendCommand(YDLIDAR_CMD_GET_DEVICE_INFO);

    receiveResponseHeader(&response_header);

    if (response_header.type_mode == YDLIDAR_RESPONSE_DEVICVE_TYPE_MODE && response_header.response_length == YDLIDAR_RESPONSE_DEVICE_INFO_SIZE)
    {
        uint8_t *infobuf = (uint8_t *)device_info;
        ydlidar.func.receive_response(infobuf, YDLIDAR_RESPONSE_DEVICE_INFO_SIZE);
        if (device_info->model == YDLIDAR_MODEL_X4)
        {
            return RESULT_OK;
        }
        else
        {
            return RESULT_FAIL;
        }
    }
    else
    {
        return RESULT_FAIL;
    }
}

/**
 * @brief Get the Device Health object
 *
 * @param device_health  The device health Struct Pointer
 * @return result_t  RESULT_OK or RESULT_FAIL
 */
result_t getDeviceHealth(ydlidar_device_health_t *device_health)
{
    ydlidar_response_header_t response_header;
    sendCommand(YDLIDAR_CMD_GET_DEVICE_HEALTH);
    receiveResponseHeader(&response_header);
    if (response_header.type_mode == YDLIDAR_RESPONSE_HEALTH_TYPE_MODE && response_header.response_length == YDLIDAR_RESPONSE_DEVICE_HEALTH_SIZE)
    {
        uint8_t *healthbuf = (uint8_t *)device_health;
        ydlidar.func.receive_response(healthbuf, YDLIDAR_RESPONSE_DEVICE_HEALTH_SIZE);
        if (device_health->status == 0)
        {
            return RESULT_OK;
        }
        else
        {
            return RESULT_FAIL;
        }
    }
    else
    {
        return RESULT_FAIL;
    }
}

/**
 * @brief  This function is used to stop the scan
 *
 */
void stopScan()
{
    sendCommand(YDLIDER_CMD_STOP);
}

/**
 * @brief  This function is used to start the scan
 *
 * @return result_t  RESULT_OK or RESULT_FAIL
 */
result_t startScan()
{
    stopScan();
    HAL_Delay(1000);
    sendCommand(YDLIDER_CMD_SCAN);
    ydlidar_response_header_t response_header;
    receiveResponseHeader(&response_header);
    if (response_header.type_mode == YDLIDAR_RESPONSE_MEASUREMENT_TYPE_MODE)
    {
        return RESULT_OK;
    }
    else
    {
        return RESULT_FAIL;
    }
}
/**
 * @brief This function is used to send the command
 *
 * @param cmd  The command to be sent
 */
void sendCommand(uint8_t cmd)
{
    ydlidar_cmd_packet_t cmd_packet;
    cmd_packet.syncByte = YDLIDER_CMD_SYNC_BYTE;
    cmd_packet.cmd_flag = cmd;
    ydlidar.func.send_command((uint8_t *)&cmd_packet, sizeof(cmd_packet));
}

/**
 * @brief This function is used to receive the response header
 *
 * @param response_header  The response header Struct Pointer
 * @return result_t  RESULT_OK or RESULT_FAIL
 */
result_t receiveResponseHeader(ydlidar_response_header_t *response_header)
{
    uint8_t *header = (uint8_t *)response_header;
    ydlidar.func.receive_response(header, YDLIDAR_RESPONSE_HEADER_SIZE);
#ifdef YDLIDAR_DEBUG
    printf("[DEBUG] header[0] = %x\r\n", header[0]);
    printf("[DEBUG] header[1] = %x\r\n", header[1]);
    printf("[DEBUG] header[2] = %x\r\n", header[2]);
    printf("[DEBUG] header[3] = %x\r\n", header[3]);
    printf("[DEBUG] header[4] = %x\r\n", header[4]);
    printf("[DEBUG] header[5] = %x\r\n", header[5]);
    printf("[DEBUG] header[6] = %x\r\n", header[6]);
    printf("[DEBUG] Response start_flag: %04x\r\n", response_header->start_flag);
    printf("[DEBUG] Response length: %d\r\n", response_header->response_length);
    printf("[DEBUG] Response mode: %d\r\n", response_header->response_mode);
    printf("[DEBUG] Response type mode: %x\r\n", response_header->type_mode);
    printf("[DEBUG] Response type mode: %d\r\n", response_header->type_mode);

#endif
    return RESULT_OK;
}

/**
 * @brief   This function is used to parse the distance
 *
 * @param rawDistance  The raw distance data
 * @param LSN  The number of the sample data
 * @param realDistance  The real distance after parse
 */
void parseDistance(uint16_t *rawDistance, int LSN, double *realDistance)
{
    for (int i = 0; i < LSN; i++)
    {
        realDistance[i] = (double)rawDistance[i] / 4.0f;
    }
}

/**
 * @brief This function is used to parse the angle
 *
 * @param FSA  The start angle
 * @param LSA  The end angle
 * @param LSN  The number of the sample data
 * @param angles  The angle after parse
 */
void AngleFirstLevelParse(uint16_t FSA, uint16_t LSA, int LSN, double *angles)
{
    // calculate start angle and end angle
    double startAngle = (double)(FSA >> 1) / 64.0;
    double endAngle = (double)(LSA >> 1) / 64.0;
    // calculate angle range
    double angleRange = endAngle - startAngle;
    // calculate angle resolution
    double angleResolution = angleRange / (double)(LSN - 1);
    for (int i = 0; i < LSN; i++)
    {
        angles[i] = startAngle + angleResolution * i;
    }
}

/**
 * @brief  This function is used to parse the second level angle
 *
 * @param angles  The angle after first level parse
 * @param LSN  The number of the sample data
 * @param distances  The distance after parse
 */
void AngleSecondLevelParse(double *angles, int LSN, double *distances)
{
    // IF Distance𝑖 == 0 AngCorrect𝑖 = 0
    // ELSE AngCorrect𝑖 = tand−1(21.8 ∗ 155.3−Distance𝑖 155.3∗Distance𝑖) for (int i = 1; i < LSN; i++)
    double AngCorrect = 0.0;
    for (int i = 1; i < LSN; i++)
    {
        if (distances[i] != 0)
        {
            AngCorrect = atan(21.8 * (155.3 - distances[i]) / (155.3 * distances[i]));
        }
        else
        {
            AngCorrect = 0.0;
        }
        angles[i] += AngCorrect;
    }
}

uint8_t scanPoints[10][MAX_SCAN_POINTS];
uint8_t SCAN_CIRCLE_INDEX = 0;

/**
 * @brief  This function is used to start receive the scan data
 *
 */
void startReceiveScanData(void)
{
    // printf("[DEBUG] SCAN_CIRCLE_INDEX = %d\r\n", SCAN_CIRCLE_INDEX);
    ydlidar.func.receive_data_dma(scanPoints[SCAN_CIRCLE_INDEX], sizeof(scanPoints[SCAN_CIRCLE_INDEX]));

    // check the start flag
}

int8_t processScanDataIndex = 0;
/**
 * @brief  This function is used to process the scan data
 *
 */
void dataProcess(void)
{
    if (processScanDataIndex >= 10)
    {
        processScanDataIndex = 0;
    }
    if (processScanDataIndex == SCAN_CIRCLE_INDEX)
    {
        return;
    }
    printf("[DEBUG] processScanDataIndex = %d, SCAN_CIRCLE_INDEX = %d\r\n", processScanDataIndex, SCAN_CIRCLE_INDEX);
    uint8_t *data = (uint8_t *)&scanPoints[processScanDataIndex];
    uint16_t checkSumrResult = 0;
    for (int i = 0; i < MAX_SCAN_POINTS; i++)
    {
        if (data[i] == 0xAA && data[i + 1] == 0x55) // check the start flag
        {

            ydlidar_data_packet_t *data_packet = (ydlidar_data_packet_t *)&data[i];
            if (data_packet->size_LSN == 1)
            {
                continue; // skip the start data packet
            }
            checkSumrResult = calculateChecksum((uint16_t *)&data[i], 4); // calculate the checksum
            uint16_t sampleDatas_SI[data_packet->size_LSN];
            uint16_t *sampledata = (uint16_t *)&data[i + sizeof(ydlidar_data_packet_t)];

            for (int j = 0; j < data_packet->size_LSN; j++) // convert the data to uint16_t
            {
                sampleDatas_SI[j] = sampledata[j];
            }
            checkSumrResult += calculateChecksum(sampledata, data_packet->size_LSN); // calculate the checksum
            if (checkSumrResult == data_packet->crc_CS)
            {
                double distances[data_packet->size_LSN];
                double angles[data_packet->size_LSN];
                parseDistance(sampleDatas_SI, data_packet->size_LSN, distances);                                                       // parse the distance
                AngleFirstLevelParse(data_packet->startAngle_FSA, data_packet->endAngle_LSA, data_packet->size_LSN, (double *)angles); // parse the first level angle
                AngleSecondLevelParse((double *)angles, data_packet->size_LSN, (double *)distances);                                   // parse the second level angle
                for (int j = 0; j < data_packet->size_LSN; j++)
                {
                    if (distances[j] != 0)
                    {
                        printf("[DEBUG] angles[%2d] : %f => distances[%2d] : %f\r\n", j, angles[j], j, distances[j]);
                    }
                }
            }
            else
            {
                printf("[ERROR] Checksum error!\r\n");
            }

#ifdef YDLIDAR_DEBUG
            printf("[DEBUG] data_packet->start_flag = %x\r\n", data_packet->start_flag);
            printf("[DEBUG] data_packet->type_CT = %x\r\n", data_packet->type_CT);
            printf("[DEBUG] data_packet->size_LSN = %x\r\n", data_packet->size_LSN);
            printf("[DEBUG] data_packet->startAngle_FSA = %x\r\n", data_packet->startAngle_FSA);
            printf("[DEBUG] data_packet->endAngle_LSA = %x\r\n", data_packet->endAngle_LSA);
            printf("[DEBUG] data_packet->crc_CS = %x\r\n", data_packet->crc_CS);
            printf("[DEBUG] calculate checksum = %x\r\n", checkSumrResult);
#endif
        }
    }
    processScanDataIndex++;
}
void dataProcess1(void)
{
    if (processScanDataIndex >= 10)
    {
        processScanDataIndex = 0;
    }
    if (processScanDataIndex == SCAN_CIRCLE_INDEX)
    {
        return;
    }
    printf("[DEBUG] processScanDataIndex = %d\r\n", processScanDataIndex);
    uint8_t *data = (uint8_t *)&scanPoints[processScanDataIndex];

    int i = 0;
    while (i < MAX_SCAN_POINTS)
    {
        // 查找数据包的开始标志
        if (data[i] == 0xAA && data[i + 1] == 0x55)
        {
            ydlidar_data_packet_t *data_packet = (ydlidar_data_packet_t *)&data[i];

            // 检查数据包长度
            if (i + sizeof(ydlidar_data_packet_t) + data_packet->size_LSN * sizeof(uint16_t) > MAX_SCAN_POINTS)
            {
                break; // 数据包不完整
            }

            // 计算校验和
            uint16_t checkSumResult = calculateChecksum((uint16_t *)&data[i], data_packet->size_LSN + 2);

            if (checkSumResult == data_packet->crc_CS)
            {
                double distances[data_packet->size_LSN];
                double angles[data_packet->size_LSN];
                parseDistance((uint16_t *)(data + i + sizeof(ydlidar_data_packet_t)), data_packet->size_LSN, distances);
                AngleFirstLevelParse(data_packet->startAngle_FSA, data_packet->endAngle_LSA, data_packet->size_LSN, angles);
                AngleSecondLevelParse(angles, data_packet->size_LSN, distances);

                // 打印数据
                for (int j = 0; j < data_packet->size_LSN; j++)
                {
                    if (distances[j] != 0)
                    {
                        printf("[DEBUG] angles[%2d] : %f => distances[%2d] : %f\r\n", j, angles[j], j, distances[j]);
                    }
                }
            }
            else
            {
                printf("[ERROR] Checksum error!\r\n");
            }

            // 移动到下一个可能的数据包起始位置
            i += sizeof(ydlidar_data_packet_t) + data_packet->size_LSN * sizeof(uint16_t);
        }
        else
        {
            i++;
        }
    }

    processScanDataIndex++;
}

/**
 * @brief  This function is used to reset the YDLIDAR
 *
 */
void reset()
{
    sendCommand(YDLIDAR_CMD_RESET);
}

/**
 * @brief  This function is used to restart the scan
 *
 */
void restartScan(void)
{
    stopScan();
    HAL_Delay(1000);
    ydlidar_device_info_t deviceinfo;
    if (getDeviceInfo(&deviceinfo) == RESULT_OK)
    {
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
        HAL_Delay(1000);
        if (getDeviceHealth(&healthinfo) == RESULT_OK)
        {
            printf("[YDLIDAR INFO] YDLIDAR running correctly! The health status: %s\r\n", healthinfo.status == 0 ? "well" : "bad");
            if (startScan() == RESULT_OK)
            {
                startReceiveScanData();
                printf("\r\nNow YDLIDAR is scanning ...... \r\n");
            }
            else
            {
                printf("start YDLIDAR is failed!  Continue........ \r\n");
            }
        }
        else
        {
            printf("cannot retrieve YDLIDAR health\r\n ");
        }
    }
    else
    {
        printf("YDLIDAR get DeviceInfo Error!!!'\r\n");
    }
}
