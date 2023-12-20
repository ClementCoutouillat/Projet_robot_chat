#ifndef __BOARD_H__
#define __BOARD_H__

#include "stdio.h"
#include "FreeRTOS.h"
#include "task.h"

#define BOARD_TASK_STACK_SIZE 128
#define BOARD_TASK_PRIORITY 3

void boardTask(void *pvParameters);
void createBoardTask(void);
#endif // __BOARD_H__
