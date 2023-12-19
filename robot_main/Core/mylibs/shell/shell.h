/*
 * shell.h
 *
 *  Created on: 7 juin 2019
 *      Author: laurent
 *  Last Modified: 09/11/2023
 *  Contributors: Jiangbo WANG
 *  Changes: add the function of shell_uart_receive_irq_cb
 *  Description: This is a shell library for STM32 nucleo board
 */

#ifndef INC_LIB_SHELL_SHELL_H_
#define INC_LIB_SHELL_SHELL_H_

#include <stdint.h>

#define ARGC_MAX 8
#define BUFFER_SIZE 40
#define SHELL_FUNC_LIST_MAX_SIZE 64

typedef struct h_shell_struct h_shell_t;

typedef uint8_t (*drv_shell_transmit_t)(char *data, uint16_t size);
typedef uint8_t (*drv_shell_receive_t)(char *data, uint16_t size);
typedef int32_t (*shell_func_pointer_t)(struct h_shell_struct *h_shell, int argc, char **argv);

typedef struct
{
    char c;
    shell_func_pointer_t pfunc;
    char *description;
} shell_func_t;

typedef struct
{
    drv_shell_transmit_t transmit;
    drv_shell_receive_t receive;
} shell_drv_t;

typedef struct h_shell_struct
{
    int shell_func_list_size;
    shell_func_t shell_func_list[SHELL_FUNC_LIST_MAX_SIZE];
    char print_buffer[BUFFER_SIZE];
    char cmd_buffer[BUFFER_SIZE];
    shell_drv_t drv;
} h_shell_t;

int shell_init(h_shell_t *h_shell);
int shell_add(h_shell_t *h_shell, char c, shell_func_pointer_t pfunc, char *description);

void shell_run(h_shell_t *h_shell);
void shell_uart_receive_irq_cb(void);

#endif /* INC_LIB_SHELL_SHELL_H_ */
