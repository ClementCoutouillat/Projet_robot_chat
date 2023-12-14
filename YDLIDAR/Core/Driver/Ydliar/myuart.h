#ifndef __UART_H__
#define __UART_H__

#include "stdio.h"
#include "stdbool.h"
#include "string.h"
// #include "main.h"
#include "Ydlidar.h"
extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern uint8_t SCAN_CIRCLE_INDEX;
extern uint8_t PROCESS_SCAN_DATA_INDEX;
extern bool receiveFlag;
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
        receiveFlag = true;
        SCAN_CIRCLE_INDEX = SCAN_CIRCLE_INDEX++ % MAX_SCAN_BUFFER_SIZE;
        if (SCAN_CIRCLE_INDEX == PROCESS_SCAN_DATA_INDEX)
        {
            receiveFlag = false;
            SCAN_CIRCLE_INDEX = (SCAN_CIRCLE_INDEX + MAX_SCAN_BUFFER_SIZE - 1) % MAX_SCAN_BUFFER_SIZE;
        }
        startReceiveScanData();
    }
}

#endif // __UART_H__
