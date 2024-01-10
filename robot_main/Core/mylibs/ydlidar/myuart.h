/**
 * @file myuart.h
 * @author Jiangbo WANG
 * @brief This file is the uart driver for ydlidar
 * @version 0.1
 * @date 2023-12-28
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __UART_H__
#define __UART_H__

#include "drv_uart.h"
#include "Ydlidar.h"

extern UART_HandleTypeDef huart4;

int8_t uartSendCommand(uint8_t *cmd, uint32_t size)
{
    HAL_UART_Transmit(&huart4, cmd, size, 1000);
    return 0;
}

int8_t uartReceiveResponse(uint8_t *data, uint32_t size)
{
    HAL_UART_Receive(&huart4, data, size, 1000);
    return 0;
}
int8_t uartReceiveDataDMA(uint8_t *data, uint32_t size)
{
    HAL_UART_Receive_DMA(&huart4, data, size);
    return 0;
}

// uart callback function

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == UART_NAME)
    {
        shell_uart_receive_irq_cb();
    }
    if (huart->Instance == USART4)
    {
        YdlidarReceivedScanDataWithDMA();
    }
}

#endif // __UART_H__
