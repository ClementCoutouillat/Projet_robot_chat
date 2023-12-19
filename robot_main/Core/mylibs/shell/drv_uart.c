
/**
 * @file drv_uart.c
 * @author Jiangbo WANG
 * @brief  This file contains the function to control uart
 * @version 0.1
 * @date 2023-11-23
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "drv_uart.h"

static SemaphoreHandle_t sem_uart_read = NULL;

int drv_uart_init()
{
    sem_uart_read = xSemaphoreCreateBinary();
    if (sem_uart_read == NULL)
    {
        printf("[ERROR]: Semaphore create failed.\r\n"); // TODO: handle error
        return -1;
    }
    else
    {
        printf("[INFO]: Shell uart semaphore create success.\r\n");
    }
    return 0;
}

void shell_uart_receive_irq_cb(void)
{
    BaseType_t pxHigherPriorityTaskWoken = pdFALSE;

    xSemaphoreGiveFromISR(sem_uart_read, &pxHigherPriorityTaskWoken);

    portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
}

uint8_t drv_uart_receive(char *str, uint16_t size)
{
    HAL_UART_Receive_IT(&UART_DEVICE, (uint8_t *)(str), 1);
    xSemaphoreTake(sem_uart_read, portMAX_DELAY);
    return size;
}

uint8_t drv_uart_transmit(char *str, uint16_t size)
{
    HAL_UART_Transmit(&UART_DEVICE, (uint8_t *)str, size, HAL_MAX_DELAY);
    return size;
}

// uart interrupt callback
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == UART_NAME)
    {
        shell_uart_receive_irq_cb();
    }
}