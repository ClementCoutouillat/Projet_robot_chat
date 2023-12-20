/*
 * shell.c
 *
 *  Created on: Oct 25, 2023
 *      Author: jkx
 */

#include "usart.h"
#include <stdio.h>
#include <string.h>
#include "shell.h"
#include "dcMotor.h"
#include "PIDControl.h"

uint8_t prompt[] = "user@ENSEA-STM32G070RBTX>>";
uint8_t started[] =
    "\r\n*-----------------------------*"
    "\r\n| ENSEA-STM32G070RBTX |"
    "\r\n*-----------------------------*"
    "\r\n";
uint8_t cmdNotFound[] = "Command not found\r\n";
uint8_t newline[] = "\r\n";
uint8_t backspace[] = "\b \b";

uint8_t uartRxReceived;                    // Flag for the received char
uint8_t uartRxBuffer[UART_RX_BUFFER_SIZE]; // Beffer for the received char
char cmdBuffer[CMD_BUFFER_SIZE];           // Buffer for the command
uint8_t uartRxCounter = 0;                 // Counter for the received char

uint8_t uartTxBuffer[UART_TX_BUFFER_SIZE]; // buffer for the transmission
uint8_t uartTxStringLength;                // length of the string to transmit
char *argv[MAX_ARGS];
int argc = 0;
char *token;
int newCmdReady = 0;

void Shell_Init(void)
{
    memset(argv, 0, MAX_ARGS * sizeof(char *));
    memset(cmdBuffer, 0, CMD_BUFFER_SIZE * sizeof(char));
    memset(uartRxBuffer, 0, UART_RX_BUFFER_SIZE * sizeof(char));
    memset(uartTxBuffer, 0, UART_TX_BUFFER_SIZE * sizeof(char));

    HAL_UART_Receive_IT(&huart2, uartRxBuffer, UART_RX_BUFFER_SIZE);
    HAL_UART_Transmit(&huart2, started, strlen((char *)started), HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart2, prompt, strlen((char *)prompt), HAL_MAX_DELAY);
}

void Shell_Loop(void)
{

    if (uartRxReceived)
    {
        uartTxStringLength = snprintf((char *)uartTxBuffer, UART_TX_BUFFER_SIZE, "received：%s\r\n", cmdBuffer);
        HAL_UART_Transmit(&huart2, uartTxBuffer, uartTxStringLength, HAL_MAX_DELAY);
        if (uartRxBuffer[0] == ASCII_CR)
        {
            cmdBuffer[uartRxCounter - 1] = '\0';
        }
        else
        {
            cmdBuffer[uartRxCounter] = '\0';
        }
        argc = 0;
        token = strtok(cmdBuffer, " ");

        while (token != NULL)
        {
            argv[argc++] = token;
            token = strtok(NULL, " ");
        }
        newCmdReady = 1;
    }
    if (newCmdReady)
    {
        if (!strcmp(argv[0], "start"))
        {
            startMotor();
            printf("[INFO]: Start the motor\r\n");
        }
        // detect the stop command
        else if (!strcmp(argv[0], "stop"))
        {
            stopMotor();
            printf("[INFO]: Stop the motor\r\n");
        }
        // detect the speed
        else if (!strcmp(argv[0], "speed"))
        {
            setSpeed(argc, argv);
        }
        // debug pid kp ki kd
        else if (!strcmp(argv[0], "debug"))
        {
            setPIDparam(argc, argv);
        }
        else
        {
            printf("[ERROR]: Command not found\r\n");
        }
        HAL_UART_Transmit(&huart2, prompt, sizeof(prompt), HAL_MAX_DELAY);
        newCmdReady = 0;
        // reset uartRxReceived flag and counter and cmdBuffer
        uartRxReceived = 0;
        uartRxCounter = 0;
        memset(cmdBuffer, 0, CMD_BUFFER_SIZE * sizeof(char));
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    // if receied counter is more than CMD_BUFFER_SIZE, reset counter and cmdBuffer, and print error message
    // else increment counter and continue
    if (uartRxCounter > CMD_BUFFER_SIZE)
    {
        uartRxCounter = 0;
        memset(cmdBuffer, 0, CMD_BUFFER_SIZE * sizeof(uint8_t));
        int uartTxStringLength = snprintf((char *)uartTxBuffer, UART_TX_BUFFER_SIZE, "Command too long\r\n");
        HAL_UART_Transmit(&huart2, uartTxBuffer, uartTxStringLength, HAL_MAX_DELAY);
    }
    else
    {
        cmdBuffer[uartRxCounter++] = (char)uartRxBuffer[0];
    }
    // if received \n or \r, set uartRxReceived flag
    if (uartRxBuffer[0] == ASCII_CR || uartRxBuffer[0] == ASCII_LF)
    {
        uartRxReceived = 1;
    }
    HAL_UART_Receive_IT(&huart2, uartRxBuffer, UART_RX_BUFFER_SIZE);
}
