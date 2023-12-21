/**
 * @file shell_founction.c
 * @author Jiangbo WANG
 * @brief  This file contains the function to create shell task and register shell commands
 * @version 0.1
 * @date 2023-11-23
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "shell_founction.h"
#include "led.h"
#include "string.h"
#include "dcMotor.h"
#include "PIDControl.h"

// #include "sensorSpam.h"

h_shell_t h_shell;
// add two parameters from the shell
int32_t add(h_shell_t *h_shell, int argc, char **argv)
{
    int a, b;
    if (argc != 3)
    {
        printf("[INFO]: Usage: add a b\r\n");
        return -1;
    }
    a = atoi(argv[1]);
    b = atoi(argv[2]);
    printf("%d + %d = %d\r\n", a, b, a + b);
    return 0;
}

int32_t motorCommand(h_shell_t *h_shell, int argc, char **argv)
{
    // founction to control the motor
    if (!strcmp(argv[1], "start"))
    {
        startMotor();
        printf("[INFO]: Start the motor\r\n");
    }
    // detect the stop command
    else if (!strcmp(argv[1], "stop"))
    {
        stopMotor();
        printf("[INFO]: Stop the motor\r\n");
    }
    // detect the speed
    else if (!strcmp(argv[1], "s"))
    {
        if (argc != 4)
        {
            printf("[INFO]: Usage:m s <speed> <speed>\r\n");
            return -1;
        }
        setSpeed(argc, argv);
    }

    else if (!strcmp(argv[1], "debug"))
    {
        setPIDparam(argc, argv); // debug pid kp ki kd
    }
    else
    {
        printf("[ERROR]: Command not found\r\n");
    }
    return 0 ;
}

void registerShellCommands(h_shell_t *h_shell)
{

    shell_add(h_shell, 'l', led, "Control LED blinking");
    // shell_add(h_shell, 's', spam, "Send spam message on serial");
    shell_add(h_shell, 'a', add, "Add two numbers");
    shell_add(h_shell, 'm', motorCommand, "Control the motor");
}

void task_shell(void *argument)
{
    shell_drv_t drv_uart;
    drv_uart.transmit = drv_uart_transmit;
    drv_uart.receive = drv_uart_receive;
    h_shell.drv = drv_uart;
    drv_uart_init();
    shell_init(&h_shell);
    registerShellCommands(&h_shell);
    printf("[INFO]: Shell init success.\r\n");
    while (1)
    {
        shell_run(&h_shell);
    }
}

void createShellTask(void)
{
    xTaskCreate(task_shell, "shell", 512, NULL, 1, NULL);
}
