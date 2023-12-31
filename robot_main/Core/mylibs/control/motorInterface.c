#include "motorInterface.h"
#include "math.h"
#include "getEncoder.h"
#define Wheel_spacing 0.18f  // The distance between the two wheels of the car
#define Wheel_diameter 0.04f // The diameter of the wheel of the car

/**
 * @brief Thi founction is used to control the car to move
 *
 * @param Vx
 * @param Vz
 */
void driverMotor(uint16_t Vx, uint16_t Vz)
{
    printf("[info] car move: left=%d, right=%d\r\n", Vx, Vz);
    uint16_t Vleft = Vx - Vz * Wheel_spacing / 2.0f;  //  calculate the target speed of the left wheel
    uint16_t Vright = Vx + Vz * Wheel_spacing / 2.0f; // calculate the target speed of the right wheel
    // TODO: add the motor PID speed target set function here
    moteur_controle(Vleft, Vright);
}

/**
 * @brief This founction is used to control the car to turn, the angle is in degree
 *        the m is the delta speed of the two wheel
 *
 * @param angle
 * @return float
 */
static float getdeltaM(float angle)
{
    float m = (angle - 180) / 180 * MAX_SPEED;
    return m;
}
/**
 * @brief This founction is used to control the car to go straight,
 *        the m_gauche and m_droite is the speed of the two wheel
 *
 * @param m_gauche
 * @param m_droite
 */
void goStraight(float speed) // distance in meter
{
    moteur_controle(speed, speed);
}

void speedLimit(float *speed)
{
    if (*speed > MAX_SPEED)
    {
        *speed = MAX_SPEED;
    }
    else if (*speed < -MAX_SPEED)
    {
        *speed = -MAX_SPEED;
    }
}

/**
 * @brief This founction is used to control the car to turn, the angle is in degree
 *
 * @param angle
 */
void changeAngle(float angle) // angle in degree
{
    uint16_t m_gauche, m_droite, omega, delay;
    // version 1
    //  m_gauche = gMotorData.speed + getdeltaM(angle);
    //  m_droite = gMotorData2.speed + getdeltaM(angle);
    //  moteur_controle(m_gauche, m_droite);
    // version 2
    if (angle > 0 && angle <= 180)
    {
        m_gauche = MAX_SPEED;
        m_droite = -MAX_SPEED;
    }
    else if (angle > 180 && angle < 360)
    {
        m_gauche = -MAX_SPEED;
        m_droite = MAX_SPEED;
    }
    // motor control with pwm directly
    moteur_controle_dPWM(m_gauche, m_droite);
    // angle speed
    omega = Wheel_diameter * 3.1415926 / 60 / Wheel_spacing * abs(m_gauche - m_droite);
    // delay
    delay = (uint16_t)(1000 * round(angle / omega));
    HAL_Delay(delay);
    // stop
    moteur_controle_dPWM(0, 0);
}

/**
 * @brief stop the car
 *
 *
 */
void motorStop(void)
{
    motor_contorle_dPWM(0, 0);
}

/**
 * @brief This founction is used to control the car to move, the distance is in meter
 *
 */
void moveDistance(float distance, float speed)
{
    float runcircle = (distance / (Wheel_diameter * 3.1415926));
    uint16_t delay = (uint16_t)(1000 * (runcircle / speed));
    goStraight(speed);
    HAL_Delay(delay);
    goStraight(0);
}