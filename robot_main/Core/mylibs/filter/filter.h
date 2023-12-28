#ifndef __FILTER_H__
#define __FILTER_H__

#include "stdio.h"

void Kalman_Filter(float Accel, float Gyro);
void firstOrderFilter(float angle_m, float gyro_m);

#endif // __FILTER_H__