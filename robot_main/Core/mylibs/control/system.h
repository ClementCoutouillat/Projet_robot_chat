#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "FreeRTOS.h"
#include "task.h"
#include "Ydlidar.h"
#include "stdbool.h"
#define RATE_1_HZ 1
#define RATE_10_HZ 10
#define RATE_100_HZ 100

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

// void Avoid(void) 
// {
//     uint8_t i = 0;
//     uint8_t angle_cnt = 0;
//     float angle;
//     float angle_sum = 0;
//     static uint8_t flag_avoiding;
//     static int Avoid_Distance = 255;
//     for (i = 0; i < 50; i++)
//     {

//         if (0 < PointDataProcess[i].distance && PointDataProcess[i].distance < Avoid_Distance)
//         {
//             if (PointDataProcess[i].angle < 50)
//                 angle_sum += PointDataProcess[i].angle;
//             else
//                 angle_sum += (PointDataProcess[i].angle - 360);
//             angle_cnt++;
//         }
//     }
//     if (angle_cnt == 0)
//     {
//         if (Move_X < 0.24)
//             Move_X += 0.01;
//         Move_Z = 0;
//     }
//     else
//     {
//         Move_X = 0;
//         if ((angle_sum / angle_cnt) > 0)
//         {
//             Move_Z = PI / 3.3;
//         }
//         else
//         {
//             Move_Z = -PI / 3.3;
//         }
//     }
//     Drive_Motor(Move_X, Move_Y, Move_Z);
// }
