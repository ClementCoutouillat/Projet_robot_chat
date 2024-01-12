/**
 * @file led.c
 * @author Jiangbo WANG
 * @brief This file contains the function to control LED
 * @version 0.1
 * @date 2023-11-23
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "led.h"
#include "stdlib.h"
#include "string.h"
#include <stdbool.h>

static SemaphoreHandle_t ledStartSemaphore = NULL;
static uint32_t blinkingPeriod = 1000; // LED blinking period in ms
static bool ledOn = false;             // LED control variable

int32_t led(h_shell_t *h_shell, int argc, char **argv)
{
    if (argc != 2)
    {
        printf("[ERROR]: Usage: l <period>,The default period is 1000ms\r\n");
        printf("[ERROR]: Usage: l on,Turn on the LED\r\n");
        printf("[ERROR]: Usage: l off,Turn off the LED\r\n");
        return -1; // parameter error
    }

    // compare the parameter with "on" or "off" or <period>
    if (strcmp(argv[1], "on") == 0)
    {
        xSemaphoreGive(ledStartSemaphore); // give semaphore to start LED blinking
        ledOn = true;                      // turn on LED
    }
    else if (strcmp(argv[1], "off") == 0)
    {
        ledOn = false; // turn off LED
    }
    else
    {
        int32_t inputPeriod = strtoul(argv[1], NULL, 10); // convert string to unsigned long
        if (blinkingPeriod >= 0)
        {
            blinkingPeriod = inputPeriod;
        }
    }
    return 0;
}

/**
 * @brief This function is to control the LED
 *        The LED will blink with the period of blinkingPeriod
 *        The LED will be turned on or off by ledOn
 *        The LED will be turned on or off by ledOn
 *
 * @param argument
 */
void LedTask(void *argument)
{
    TickType_t lastWakeTime = getSysTickCnt();
    if (xSemaphoreTake(ledStartSemaphore, portMAX_DELAY) == pdTRUE)
    {
        while (1)
        {
            if (ledOn)
            {
                HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
                HAL_GPIO_TogglePin(LED_ORANGE_GPIO_Port, LED_ORANGE_Pin);
                vTaskDelay(blinkingPeriod);
            }
            vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(1));
        }
    }
    vTaskDelete(NULL);
}

/**
 * @brief Create a Led Task object
 *
 */
void createLedTask(void)
{
    ledStartSemaphore = xSemaphoreCreateBinary();
    if (ledStartSemaphore == NULL)
    {
        printf("[ERROR]: Semaphore create failed.\r\n");
    }
    else
    {
        printf("[INFO]: LED Semaphore create success.\r\n");
        xTaskCreate(LedTask, "LedTask", Led_TASK_STACK_SIZE, NULL, Led_TASK_PRIORITY, NULL);
    }
}
