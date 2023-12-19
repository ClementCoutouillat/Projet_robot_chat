#include "system.h"
static uint32_t sysTickCnt = 0;
uint32_t getSysTickCnt(void)
{
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) /*system is running*/
        return xTaskGetTickCount();
    else
        return sysTickCnt;
}
