#ifndef SERVO_H
#define SERVO_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ----------------------------------------------------------------- */
#include "stm32f1xx_hal.h"

/* Function Prototypes ------------------------------------------------------ */

/**
 * @brief  Move the servo to a specified angle (0–180 degrees).
 * @note   Uses TIM4 CH4 by default. The timer must be configured in CubeMX
 *         with Prescaler=71 and Period=19999.
 * @param  angle  Target angle in degrees (0 to 180). Values > 180 are clamped.
 * @retval None
 */
void Servo_GotoAngle(uint8_t angle);

#ifdef __cplusplus
}
#endif

#endif /* SERVO_H */
