#include "motorInterface.h"
#define Wheel_spacing 0.3f  // The distance between the two wheels of the car
#define Wheel_diameter 0.1f // The diameter of the wheel of the car

void carInit(void)
{
}
void carLeft(void)
{
    printf("[info] car left\r\n");
}
void carRight(void)
{
    printf("[info] car right\r\n");
}
void carForward(void)
{
    printf("[info] car forward\r\n");
}
void carBackward(void)
{
    printf("[info] car backward\r\n");
}
void carStop(void)
{
    printf("[info] car stop\r\n");
}

void driverMotor(uint16_t Vx, uint16_t Vz)
{
    uint16_t amplitude = 300; // Wheel target speed limit
    printf("[info] car move: left=%d, right=%d\r\n", Vx, Vz);
    uint16_t Vleft = Vx - Vz * Wheel_spacing / 2.0f;  //  calculate the target speed of the left wheel
    uint16_t Vright = Vx + Vz * Wheel_spacing / 2.0f; // calculate the target speed of the right wheel
    // TODO: add the motor PID speed target set function here
}

static float getdeltaM(float angle)
{
    float m = (angle - 180) / 180 * MAX_SPEED;
    return m;
}

void goStraight(float m_gauche, float m_droite) // distance in meter
{
    moteur_controle(m_gauche, m_droite);
}

void changeAngle(float angle) // angle in degree
{
    float m_gauche, m_droite;
    m_gauche = gMotorData.speed + getdeltaM(angle);
    m_droite = gMotorData2.speed + getdeltaM(angle);
    moteur_controle(m_gauche, m_droite);
}