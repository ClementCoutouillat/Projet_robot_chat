#ifndef __DYLIDER_H__
#define __DYLIDER_H__
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "system.h"

#define YDLIDER_CMD_SYNC_BYTE 0xA5
#define YDLIDER_CMD_STOP 0x65
#define YDLIDER_CMD_SCAN 0x60
#define YDLIDAR_CMD_GET_DEVICE_INFO 0x90
#define YDLIDAR_CMD_GET_DEVICE_HEALTH 0x91
#define YDLIDAR_CMD_RESET 0x80

#define YDLIDAR_RESPONSE_SYNC_BYTE 0x5A
#define YDLIDAR_RESPONSE_DEVICE_HEALTH_SIZE 3
#define YDLIDAR_RESPONSE_DEVICE_INFO_SIZE 20
#define YDLIDAR_RESPONSE_HEADER_SIZE 7
#define YDLIDAR_RESPONSE_SCAN_DATA_SIZE 12

///< the device info
#define YDLIDAR_RESPONSE_DEVICVE_TYPE_MODE 0x04
#define YDLIDAR_MODEL_X4 0x06

///< the device health
#define YDLIDAR_RESPONSE_HEALTH_TYPE_MODE 0x06
#define YDLIDAR_HEALTH_OK 0x00
///< the scan data
#define YDLIDAR_RESPONSE_MEASUREMENT_TYPE_MODE 0x81

// send command function
typedef int8_t (*send_command_func)(uint8_t *cmd, uint32_t size);
// receive data function
typedef int8_t (*receive_data_func)(uint8_t *data, uint32_t size);

#define MAX_SCAN_POINTS 360
#define MAX_SCAN_BUFFER_SIZE 2

// #define YDLIDAR_DEBUG
// send and receive function struct
typedef struct
{
    send_command_func send_command;
    receive_data_func receive_response;
    receive_data_func receive_data_dma;
} ydlidar_func_t;

typedef struct
{
    uint8_t syncByte;
    uint8_t cmd_flag;
} __attribute__((packed)) ydlidar_cmd_packet_t;

// ydlidar device info

typedef struct
{
    uint8_t model;
    uint8_t major_firmware_version;
    uint8_t minor_firmware_version;
    uint8_t hardware_version;
    uint8_t serialnum[16];
} __attribute__((packed)) ydlidar_device_info_t;

// ydliar device health
typedef struct
{
    uint8_t status;
    uint16_t error_code;
} __attribute__((packed)) ydlidar_device_health_t;

// ydliar answer header
typedef struct
{
    uint16_t start_flag;
    uint32_t response_length : 30;
    uint32_t response_mode : 2;
    uint8_t type_mode;
} __attribute__((packed)) ydlidar_response_header_t;

// ydlider data packet
typedef struct
{
    uint16_t start_flag;
    uint8_t type_CT;
    uint8_t size_LSN;
    uint16_t startAngle_FSA;
    uint16_t endAngle_LSA;
    uint16_t crc_CS;
} __attribute__((packed)) ydlidar_data_packet_t;

typedef struct
{
    uint8_t quality;
    float angle;
    float distance;
    bool startBit;
} __attribute__((packed)) ydlidar_scan_point_t;

typedef struct
{
    double distance;
    double angle;
} ScanPoint_t;

///< The ydlidar structure for users
typedef struct
{
    ydlidar_func_t func;                   /**< The ydlidar communication function */
    ydlidar_device_info_t device_info;     /**< The device information */
    ydlidar_device_health_t device_health; /**< The device health */
    ydlidar_scan_point_t scan_point;       /**< The scan point */
} __attribute__((packed)) ydlidar_t;

typedef uint8_t result_t;
#define RESULT_OK 0
#define RESULT_FAIL -1

///< get device info
result_t getDeviceInfo(ydlidar_device_info_t *device_info);
///< get device health
result_t getDeviceHealth(ydlidar_device_health_t *device_health);
///< start scan
result_t startScan(void);
///< start receive scan data
void startReceiveScanData(void);
///< data process
void dataProcess(void);
///< restart scan
void restartScan(void);
///< stop scan
void stopScan(void);
/// reset
void reset(void);
///< send command
void sendCommand(uint8_t cmd);
///< receive response header
result_t receiveResponseHeader(ydlidar_response_header_t *response_header);
void createYdlidarTask(void);
#endif // __DYLIDER_H__
