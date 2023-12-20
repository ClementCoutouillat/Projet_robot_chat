#include "bumper.h"

void BumperTask(void *pvParameters)
{
    while (1)
    {
        printf("[INFO] bumper task\r\n");
        // TODO: add bumper task, when the bumper is detected, the car will change mode,the mode is between hit and avoid
        vTaskDelay(5000);
    }
}

void createBumperTask(void)
{
    xTaskCreate((TaskFunction_t)BumperTask,        // task function
                (const char *)"BumperTask",        // task name
                (uint16_t)BUMPER_TASK_STACK_SIZE,  // task stack size
                (void *)NULL,                      // task parameter
                (UBaseType_t)BUMPER_TASK_PRIORITY, // task priority
                NULL);                             // task handle
}
