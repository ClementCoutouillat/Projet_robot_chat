/**
 * @file Ydlidar.c
 *
 * @author Jiangbo WANG
 * @brief This file is the driver for YDLIDAR
 * @version 0.1
 * @date 2023-12-28
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "ydlidar_protocol.h"
// #include "main.h"
#include "math.h"
#include "string.h"
// #include "myuart.h"

ydlidar_t ydlidar;
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
 * @brief  This function is used to start the scan
 *
 * @return result_t  RESULT_OK or RESULT_FAIL
 */
result_t startScan()
{
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
 * @brief  This function is used to reset the YDLIDAR
 *
 */
void reset()
{
    sendCommand(YDLIDAR_CMD_RESET);
}
/**
 * @brief This function is used to calculate the checksum of the data CRC16
 *
 * @param data  The data to be calculated
 * @param length  The length of the data to be calculated
 * @return uint16_t
 */
uint16_t calculateChecksum(uint8_t *data, uint32_t length)
{
    uint16_t checksum = 0;

    for (int i = 0; i < 2 * length + 1; i += 2)
    {
        if (i == 8)
        {
            i += 2; //
        }
        uint16_t tmp = (uint16_t)data[i] << 8 | (uint16_t)data[i + 1];
        checksum ^= tmp;
    }
    // 交换checksum的字节序
    checksum = (checksum >> 8) | (checksum << 8);
    return checksum;
}

/**
 * @brief  This function is used to start receive the scan data
 * @param addr  The address of the data to be received, data size
 *
 */
void startReceiveScanData(uint8_t *addr, uint32_t size)
{
    ydlidar.func.receive_response(addr, size);
    // ydlidar.func.receive_data_dma(ydlidarUartRawData[SCAN_CIRCLE_INDEX], sizeof(ydlidarUartRawData[SCAN_CIRCLE_INDEX]));
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
void AngleFirstLevelParse1(uint16_t FSA, uint16_t LSA, int LSN, double *angles)
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
    if (startAngle > endAngle)
    {
        double angleResolution = (360.0 - startAngle + endAngle) / (double)(LSN - 1);
        for (int i = 0; i < LSN; i++)
        {
            angles[i] = startAngle + angleResolution * i;
            if (angles[i] > 360.0)
            {
                angles[i] -= 360.0;
            }
        }
    }
    else
    {
        // calculate angle resolution
        double angleResolution = (endAngle - startAngle) / (double)(LSN);
        for (int i = 0; i < LSN; i++)
        {
            angles[i] = startAngle + angleResolution * i;
        }
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
    // IF Distance == 0 AngCorrect = 0
    // ELSE AngCorrect = tand−1(21.8 ∗ 155.3−Distance 155.3∗Distance) for (int i = 1; i < LSN; i++)
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