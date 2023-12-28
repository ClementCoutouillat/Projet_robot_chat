/**
 * @file shell_founction.h
 * @author Jiang WANG
 * @version 0.1
 * @date 2023-12-28
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef __SHELL_FOUNCTION_H__
#define __SHELL_FOUNCTION_H__

#include <stdio.h>
#include <stdint.h>
#include "shell.h"
#include "drv_uart.h"
#include "main.h"
#include <stdlib.h>
#include "cmsis_os.h"

///< register shell function : add
int32_t add(h_shell_t *h_shell, int argc, char **argv);

void createShellTask(void);

#endif // __SHELL_FOUNCTION_H__
