#include "motorInterface.h"
#include "math.h"
#include "string.h"
#include "getEncoder.h"
#define Wheel_spacing 0.155f  // The distance between the two wheels of the car
#define Wheel_diameter 0.044f // The diameter of the wheel of the car

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
void changeAngle(uint16_t angle, const char *strPtr)
{
    uint16_t m_gauche, m_droite, omega;
    int16_t delay = 0;
    char *string = "right";
    if (strcmp(string, strPtr))
    {
        m_gauche = 0; //-MAX_SPEED;
        m_droite = MAX_SPEED;
        delay += 300;
        // printf("turn left\r\n");
    }
    else // 右转
    {
        m_gauche = MAX_SPEED; //-MAX_SPEED;
        m_droite = 0;         //-MAX_SPEED;
        delay += 100;
        // printf("turn right\r\n");
    }
    // m_gauche = 0;//-MAX_SPEED;
    // m_droite = MAX_SPEED;
    moteur_controle_dPWM(m_gauche, m_droite);
    omega = 0.022 * 2 * 3.14 / 60 / 0.155 * abs(m_gauche - m_droite);
    // printf("omega:%d angle:%d \r\n", omega, angle);
    delay += (uint16_t)(angle * 3.14 / 180 / omega * 1000); // * 1000);
    // printf("delay:%d \r\n", delay);
    HAL_Delay(delay);
    moteur_controle_dPWM(0, 0);
}
/**
 * @brief stop the car
 *
 *
 */
void motorStop(void)
{
    moteur_controle_dPWM(0, 0);
}

void goStraight2(float speed) // distance in meter
{
    moteur_controle_dPWM(speed, speed);
}

/**
 * @brief This founction is used to control the car to move, the distance is in meter
 *
 */
void moveDistance(float distance, float speed)
{
    float runcircle = (distance / (Wheel_diameter * 3.1415926));
    uint16_t delay = (uint16_t)(1000 * (runcircle / speed));
    moteur_controle_dPWM(160, 200);
    HAL_Delay(1500);
    motorStop();
}
