#ifndef REED_SWITCH_H
#define REED_SWITCH_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ----------------------------------------------------------------- */
#include "stm32f1xx_hal.h"

/* Typedefs ----------------------------------------------------------------- */

/**
 * @brief  Reed switch configuration structure
 */
typedef struct {
    GPIO_TypeDef *GPIOx;    /*!< GPIO port (e.g. GPIOA) */
    uint16_t      GPIO_Pin; /*!< GPIO pin  (e.g. GPIO_PIN_9) */
} ReedSwitch_Config_t;

/**
 * @brief  Door state enumeration
 */
typedef enum {
    DOOR_CLOSED = 0,  /*!< Magnet near → reed switch closed → pin LOW  */
    DOOR_OPEN   = 1   /*!< Magnet far  → reed switch open   → pin HIGH */
} DoorState_t;

/* Function Prototypes ------------------------------------------------------ */

/**
 * @brief  Initialize the reed switch GPIO pin as input with internal pull-up.
 * @note   If CubeMX has already configured the pin, this call is optional.
 * @param  cfg  Pointer to ReedSwitch_Config_t structure.
 * @retval None
 */
void ReedSwitch_Init(const ReedSwitch_Config_t *cfg);

/**
 * @brief  Read the raw digital state of the reed switch pin.
 * @param  cfg  Pointer to ReedSwitch_Config_t structure.
 * @retval GPIO_PinState  GPIO_PIN_RESET = closed, GPIO_PIN_SET = open
 */
GPIO_PinState ReedSwitch_Read(const ReedSwitch_Config_t *cfg);

/**
 * @brief  Check if the door is currently open (magnet away from switch).
 * @param  cfg  Pointer to ReedSwitch_Config_t structure.
 * @retval uint8_t  1 = door open, 0 = door closed
 */
uint8_t ReedSwitch_IsDoorOpen(const ReedSwitch_Config_t *cfg);

/**
 * @brief  Check if the door is currently closed (magnet near switch).
 * @param  cfg  Pointer to ReedSwitch_Config_t structure.
 * @retval uint8_t  1 = door closed, 0 = door open
 */
uint8_t ReedSwitch_IsDoorClosed(const ReedSwitch_Config_t *cfg);

#ifdef __cplusplus
}
#endif

#endif /* REED_SWITCH_H */
