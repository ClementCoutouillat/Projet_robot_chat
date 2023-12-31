#include "board.h"
#include "stdbool.h"
#include "motorInterface.h"

extern bool BORDER_BACK_FLAG;
extern bool BORDER_FRONT_FLAG;

void boardTask(void *pvParameters)
{
    while (1)
    {
        // printf("[INFO] board task\r\n");
        if (BORDER_FRONT_FLAG == true)
        {
            // printf("[INFO] front boarder detected\r\n");
            motorStop();
            moveDistance(0.04, -300);
            changeAngle(180);

            BORDER_FRONT_FLAG = false;
        }
        else if (BORDER_BACK_FLAG == true)
        {
            // printf("[INFO] back boarder detected\r\n");
            motorStop();
            moveDistance(0.04, 300);
            BORDER_BACK_FLAG = false;
        }
        vTaskDelay(100);
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
