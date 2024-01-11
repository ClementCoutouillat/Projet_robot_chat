#ifndef __BOARD_H__
#define __BOARD_H__

#include "stdio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "config.h"

void boardTask(void *pvParameters);
void createBoardTask(void);
#endif // __BOARD_H__
