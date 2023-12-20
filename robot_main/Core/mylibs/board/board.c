#include "board.h"

void boardTask(void *pvParameters)
{
    while (1)
    {
        printf("[INFO] board task\r\n");
        // TODO: add board task, when the boarder is detected, the car will stop and turn around
        // TODO: this task has the highest priority
        vTaskDelay(10000);
    }
}

void createBoardTask(void)
{
    xTaskCreate((TaskFunction_t)boardTask,        // task function
                (const char *)"boardTask",        // task name
                (uint16_t)BOARD_TASK_STACK_SIZE,  // task stack size
                (void *)NULL,                     // task parameter
                (UBaseType_t)BOARD_TASK_PRIORITY, // task priority
                NULL);                            // task handle
}
