#ifndef PIR_SENSOR_H
#define PIR_SENSOR_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ----------------------------------------------------------------- */
#include "stm32f1xx_hal.h"

/* Typedefs ----------------------------------------------------------------- */

/**
 * @brief  PIR sensor configuration structure
 */
typedef struct {
    GPIO_TypeDef *GPIOx;    /*!< GPIO port of the PIR output pin (e.g. GPIOA) */
    uint16_t      GPIO_Pin; /*!< GPIO pin  of the PIR output pin (e.g. GPIO_PIN_8) */
} PIR_Config_t;

/* Function Prototypes ------------------------------------------------------ */

/**
 * @brief  Initialize the PIR sensor GPIO as input (no pull).
 * @note   If CubeMX has already configured the pin, this call is optional.
 * @param  cfg  Pointer to a PIR_Config_t structure holding port & pin info.
 * @retval None
 */
void PIR_Init(const PIR_Config_t *cfg);

/**
 * @brief  Read the raw digital output of the PIR sensor.
 * @param  cfg  Pointer to a PIR_Config_t structure.
 * @retval GPIO_PinState  GPIO_PIN_SET (HIGH = motion) or GPIO_PIN_RESET (LOW = no motion)
 */
GPIO_PinState PIR_Read(const PIR_Config_t *cfg);

/**
 * @brief  Check whether motion is currently detected.
 * @param  cfg  Pointer to a PIR_Config_t structure.
 * @retval uint8_t  1 = motion detected, 0 = no motion
 */
uint8_t PIR_IsMotionDetected(const PIR_Config_t *cfg);

#ifdef __cplusplus
}
#endif

#endif /* PIR_SENSOR_H */
