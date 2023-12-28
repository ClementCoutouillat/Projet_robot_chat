/**
 * @file led.h
 * @author Jiangbo WANG
 * @brief 
 * @version 0.1
 * @date 2023-12-28
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef __LED_H__
#define __LED_H__

#include "stdio.h"
#include "main.h"
#include "cmsis_os.h"
#include "shell.h"
#include "stdlib.h"

///< register shell function : led on/off and preiod
int32_t led(h_shell_t *h_shell, int argc, char **argv);

void LedTask(void *argument); // led task
void createLedTask(void);     // create led task

#endif // __LED_H__