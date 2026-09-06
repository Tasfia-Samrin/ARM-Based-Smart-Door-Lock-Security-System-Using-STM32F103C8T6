#include "servo.h"

extern TIM_HandleTypeDef htim4;

void Servo_GotoAngle(uint8_t angle)
{
    /* ------------------- SERVO CONFIGURATION ------------------- */
    TIM_HandleTypeDef *TIMER = &htim4; //Prescaler = 7, Counter Period = 19999
    uint32_t CHANNEL = TIM_CHANNEL_4;
    /* ----------------------------------------------------------- */

    // Clamp angle
    if (angle > 180) angle = 180;

    // Start PWM
    HAL_TIM_PWM_Start(TIMER, CHANNEL);

    // Map angle (0–180) to pulse width (500–2500 µs)
    uint16_t pulse = 500 + ((uint32_t)angle * 2000) / 180;

    // Set PWM compare value
    __HAL_TIM_SET_COMPARE(TIMER, CHANNEL, pulse);

    // Optional delay for servo to reach position
    HAL_Delay(500);  // adjust based on servo speed
}