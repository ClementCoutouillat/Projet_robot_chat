#include "system.h"
#include "motorInterface.h"

extern ScanPointData_t PointDataProcess[MaxScanPointCount];
extern uint8_t PointDataProcessIndex;
TaskHandle_t AvoidTask_Handler;

static uint32_t sysTickCnt = 0;
uint32_t getSysTickCnt(void)
{
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) /*system is running*/
        return xTaskGetTickCount();
    else
        return sysTickCnt;
}

/**
 * @brief Avoid task
 * @note This task will be notified by ydlidar task, and then it will process the data and control the car
 *       we detect the angle of the obstacle, if the angle is between 135 and 225, we will turn the car, if the obstacle is
 *       at the left side, we will turn right, if the obstacle is at the right side, we will turn left
 *
 * @param parameter
 */
void Avoid(void *parameter)
{
    uint8_t i;
    uint32_t buf;
    int32_t fsignal = 0;
    int32_t ffsignal = 0;
    int32_t swing;

    while (1)
    {
        /* receive notification */
        if (ulTaskNotifyTake(pdTRUE, portMAX_DELAY) > 0)
        {
            for (i = 1; i < PointDataProcessIndex; i++)
            {
                for (uint16_t x = 0; x < PointDataProcess[i].count; x++)
                {
                    if (PointDataProcess[i].scanPoint[x].distance <= 1200)
                    {
                        if (PointDataProcess[i].scanPoint[x].distance <= 600)
                        {
                            buf = (uint32_t)PointDataProcess[i].scanPoint[x].angle;
                            if ((buf > 135) && (buf < 225))
                            {
                                if (buf > 135 && buf < 180)
                                {
                                    fsignal++;
                                }
                                else if (buf > 180 && buf < 225)
                                {
                                    fsignal--;
                                }
                            }
                        }
                        else
                        {
                            buf = (uint32_t)PointDataProcess[i].scanPoint[x].angle;
                            if (buf > 180)
                            {
                                ffsignal++;
                            }
                            else if (buf < 180)
                            {
                                ffsignal--;
                            }
                        }
                    }
                }
            }
            if (fsignal != 0)
            {
                if ((fsignal >= -10) && (fsignal <= 10))
                {
                    carStop();
                    swing++;
                }
                else if (fsignal > 10)
                {
                    carRight();
                    swing++;
                }
                else
                {
                    carLeft();
                    swing++;
                }

                if (swing >= 4)
                {
                    swing = 0;
                    ffsignal += fsignal;
                    if (ffsignal > 0)
                    {
                        carRight();
                    }
                    else
                    {
                        carLeft();
                    }
                }
            }
            else
            {
                swing = 0;
                carForward();
            }
            fsignal = 0;
            ffsignal = 0;
            // startScan();
            printf("Avoid task is running\r\n");
        }
        else
        {
            printf("Avoid task is not running\r\n");
            vTaskDelay(100);
        }
    }
}
void createSystemTask(void)
{
    // Task Handle
    xTaskCreate(Avoid, "Avoid", 512, NULL, 1, &AvoidTask_Handler);
}
// void createSystemTask(void);
