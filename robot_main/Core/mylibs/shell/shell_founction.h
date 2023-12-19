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
int add(h_shell_t *h_shell, int argc, char **argv);

void createShellTask(void);

#endif // __SHELL_FOUNCTION_H__