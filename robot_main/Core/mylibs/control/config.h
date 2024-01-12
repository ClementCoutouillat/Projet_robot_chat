#ifndef __CONFIG_H__
#define __CONFIG_H__

#define BOARD_TASK_STACK_SIZE 128
#define BOARD_TASK_PRIORITY 3

#define Ydliar_TASK_STACK_SIZE 1024
#define Ydliar_TASK_PRIORITY 2

#define System_TASK_STACK_SIZE 512
#define System_TASK_PRIORITY 2

#define Shell_TASK_STACK_SIZE 512
#define Shell_TASK_PRIORITY 1

#define Led_TASK_STACK_SIZE 128
#define Led_TASK_PRIORITY 1

#define MAX_DISTANCE 800

#endif // __CONFIG_H__