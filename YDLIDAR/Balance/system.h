#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#define RATE_1_HZ 1
#define RATE_5_HZ 5
#define RATE_10_HZ 10
#define RATE_20_HZ 20
#define RATE_25_HZ 25
#define RATE_50_HZ 50
#define RATE_100_HZ 100
#define RATE_200_HZ 200
#define RATE_250_HZ 250
#define RATE_500_HZ 500
#define RATE_1000_HZ 1000

typedef struct
{
    float VX;
    float VY;
    float VZ;
} Smooth_Control;
extern Smooth_Control smooth_control;
float Move_X = 0.24, Move_Y, Move_Z;

void systemInit(void);

#endif // __SYSTEM_H__