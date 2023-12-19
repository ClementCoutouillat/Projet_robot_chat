#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "FreeRTOS.h"
#include "task.h"
#define RATE_1_HZ 1
#define RATE_10_HZ 10
#define RATE_100_HZ 100

uint32_t getSysTickCnt(void);
#endif // __SYSTEM_H__