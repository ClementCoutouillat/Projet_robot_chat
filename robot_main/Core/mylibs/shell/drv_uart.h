/**
 * @file drv_uart.h
 * @author jiangbo WANG
 * @brief
 * @version 0.1
 * @date 2023-12-28
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef __DRV_UART_H__
#define __DRV_UART_H__
#include <stdio.h>
#include <stdint.h>
// #include <usart.h>
#include "main.h"
#include "cmsis_os.h"
extern UART_HandleTypeDef huart2;
#define UART_DEVICE huart2
#define UART_NAME USART2

int drv_uart_init(void);
uint8_t drv_uart_transmit(char *str, uint16_t size);
uint8_t drv_uart_receive(char *str, uint16_t size);
void shell_uart_receive_irq_cb(void);

#endif // __DRV_UART_H__
