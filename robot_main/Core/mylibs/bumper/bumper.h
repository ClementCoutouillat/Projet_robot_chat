#ifndef __BUMPER_H__
#define __BUMPER_H__

#include "stdio.h"
#include "FreeRTOS.h"
#include "task.h"

#define BUMPER_TASK_STACK_SIZE 128
#define BUMPER_TASK_PRIORITY 3

void BumperTask(void *pvParameters);
void createBumperTask(void);

#endif // __BUMPER_H__