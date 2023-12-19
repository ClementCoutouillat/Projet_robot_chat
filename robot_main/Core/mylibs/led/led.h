#ifndef __LED_H__
#define __LED_H__

#include "stdio.h"
#include "main.h"
#include "cmsis_os.h"
#include "shell.h"
#include "stdlib.h"

///< register shell function : led on/off and preiod
int led(h_shell_t *h_shell, int argc, char **argv);

void LedTask(void *argument); // led task
void createLedTask(void);     // create led task

#endif // __LED_H__