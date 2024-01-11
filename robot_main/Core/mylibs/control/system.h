#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "FreeRTOS.h"
#include "task.h"
#include "Ydlidar.h"
#include "stdbool.h"
#include "config.h"
#define RATE_1_HZ 1
#define RATE_10_HZ 10
#define RATE_100_HZ 100

#define YDLIDAR_


extern TaskHandle_t AvoidTask_Handler;

extern bool BORDER_BACK_FLAG;
extern bool BORDER_FRONT_FLAG;
extern int16_t BUMPER1_FLAG;
extern int16_t BUMPER2_FLAG;
extern int16_t BUMPER3_FLAG;
extern int16_t BUMPER4_FLAG;

uint32_t getSysTickCnt(void);

void systemInit(void);
void softReset(void);
void systemControl(void *parameter);
void createSystemTask(void);
#endif // __SYSTEM_H__
