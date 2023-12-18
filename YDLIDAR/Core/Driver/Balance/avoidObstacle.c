#include "avoidObstacle.h"
#include "system.h"
#include "robotParameter.h"

int8_t position = 0;
float target_limit_float(float insert, float low, float high)
{
    if (insert < low)
        return low;
    else if (insert > high)
        return high;
    else
        return insert;
}
void Drive_Motor(float Vx, float Vy, float Vz)
{
    float amplitude = 3.5; // Wheel target speed limit 

    // Inverse kinematics 
    MOTOR_A.Target = Vx - Vz * WHEEL_DISTANCE / 2.0f; // calculate the target speed of the left wheel
    MOTOR_B.Target = Vx + Vz * WHEEL_DISTANCE / 2.0f; // calculate the target speed of the right wheel

    // Wheel (motor) target speed limit //车轮(电机)目标速度限幅
    MOTOR_A.Target = target_limit_float(MOTOR_A.Target, -amplitude, amplitude);
    MOTOR_B.Target = target_limit_float(MOTOR_B.Target, -amplitude, amplitude);
}

void avoidObstacle(void)
{
    int8_t i = 0;
    int8_t angle_cnt = 0; // 用于判断50个点中需要做避障的点
    float angle_sum = 0;
    static int Avoid_Distance = 255;
    for (i = 0; i < 50; i++)
    {
        if (0 < PointDataProcess[i].distance && PointDataProcess[i].distance < Avoid_Distance) // 距离小于220mm需要避障,只需要50度范围内点
        {
            if (PointDataProcess[i].angle < 50)
                angle_sum += PointDataProcess[i].angle;
            else
                angle_sum += (PointDataProcess[i].angle - 360);
            angle_cnt++;
        }
    }
    if (angle_cnt == 0) // 不需要避障
    {
        position = 0;
        if (Move_X < 0.24)
            Move_X += 0.01;
        Move_Z = 0;
    }
    else
    {
        Move_X = 0;
        if ((angle_sum / angle_cnt) > 0) // 障碍物偏右
        {
            position = 1;
            Move_Z = PI / 3.3; // 左转
        }
        else // 偏左
        {
            position = 2;
            Move_Z = -PI / 3.3; // 右转
        }
    }
    Drive_Motor(Move_X, Move_Y, Move_Z);
}