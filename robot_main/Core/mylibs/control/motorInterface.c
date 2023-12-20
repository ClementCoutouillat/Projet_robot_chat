#include "motorInterface.h"

#define Wheel_spacing 0.3f  // The distance between the two wheels of the car
#define Wheel_diameter 0.1f // The diameter of the wheel of the car

void driverMotor(uint16_t Vx, uint16_t Vz)
{
    uint16_t amplitude = 300; // Wheel target speed limit
    printf("[info] car move: left=%d, right=%d\n", Vx, Vz);
    uint16_t Vleft = Vx - Vz * Wheel_spacing / 2.0f;  //  calculate the target speed of the left wheel
    uint16_t Vright = Vx + Vz * Wheel_spacing / 2.0f; // calculate the target speed of the right wheel
    // TODO: add the motor PID speed target set function here
    
}
