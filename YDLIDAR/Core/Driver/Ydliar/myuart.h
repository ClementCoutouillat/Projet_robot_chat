#ifndef __UART_H__
#define __UART_H__

#include "stdio.h"
#include "string.h"
#include "Ydlidar.h"
extern UART_HandleTypeDef huart1;
extern uint8_t SCAN_CIRCLE_INDEX;
extern uint8_t processScanDataIndex;
int8_t uartSendCommand(uint8_t *cmd, uint32_t size)
{
    HAL_UART_Transmit(&huart1, cmd, size, 1000);
    return 0;
}

int8_t uartReceiveResponse(uint8_t *data, uint32_t size)
{
    HAL_UART_Receive(&huart1, data, size, 1000);
    return 0;
}
int8_t uartReceiveDataDMA(uint8_t *data, uint32_t size)
{
    HAL_UART_Receive_DMA(&huart1, data, size);
    return 0;
}

// uart callback function
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        SCAN_CIRCLE_INDEX++;
        if (SCAN_CIRCLE_INDEX >= 10)
        {
            SCAN_CIRCLE_INDEX = 0;
        }

        startReceiveScanData();
    }
}

#endif // __UART_H__
