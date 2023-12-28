#include "motorInterface.h"
#define Wheel_spacing 0.3f  // The distance between the two wheels of the car
#define Wheel_diameter 0.1f // The diameter of the wheel of the car

/**
 * @brief Thi founction is used to control the car to move
 *
 * @param Vx
 * @param Vz
 */
void driverMotor(uint16_t Vx, uint16_t Vz)
{
    uint16_t amplitude = 300; // Wheel target speed limit
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
void goStraight(float m_gauche, float m_droite) // distance in meter
{
    moteur_controle(m_gauche, m_droite);
}

/**
 * @brief This founction is used to control the car to turn, the angle is in degree
 *
 * @param angle
 */
void changeAngle(float angle) // angle in degree
{
    float m_gauche, m_droite;
    m_gauche = gMotorData.speed + getdeltaM(angle);
    m_droite = gMotorData2.speed + getdeltaM(angle);
    moteur_controle(m_gauche, m_droite);
}