#ifndef __FILTER_H__
#define __FILTER_H__

extern float angle, angle_dot;

// Kalman filter
void Kalman_Filter(float Accel, float Gyro);
// first order filter
float first_order_filter(float angle_m, float gyro_m);

#endif // __FILTER_H__