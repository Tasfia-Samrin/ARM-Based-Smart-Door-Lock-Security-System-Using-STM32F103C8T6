#ifndef BUZZER_H
#define BUZZER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ----------------------------------------------------------------- */
#include "stm32f1xx_hal.h"

/* Typedefs ----------------------------------------------------------------- */

/**
 * @brief  Buzzer configuration structure
 */
typedef struct {
    GPIO_TypeDef *GPIOx;     /*!< GPIO port (e.g. GPIOA) */
    uint16_t      GPIO_Pin;  /*!< GPIO pin  (e.g. GPIO_PIN_10) */
    uint8_t       ActiveHigh;/*!< 1 = HIGH turns buzzer ON, 0 = LOW turns buzzer ON */
} Buzzer_Config_t;

/* Function Prototypes ------------------------------------------------------ */

/**
 * @brief  Initialize the buzzer GPIO pin as push-pull output.
 * @note   If CubeMX has already configured the pin, this call is optional.
 * @param  cfg  Pointer to Buzzer_Config_t structure.
 * @retval None
 */
void Buzzer_Init(const Buzzer_Config_t *cfg);

/**
 * @brief  Turn the buzzer ON.
 * @param  cfg  Pointer to Buzzer_Config_t structure.
 * @retval None
 */
void Buzzer_On(const Buzzer_Config_t *cfg);

/**
 * @brief  Turn the buzzer OFF.
 * @param  cfg  Pointer to Buzzer_Config_t structure.
 * @retval None
 */
void Buzzer_Off(const Buzzer_Config_t *cfg);

/**
 * @brief  Produce a single beep of specified duration.
 * @param  cfg          Pointer to Buzzer_Config_t structure.
 * @param  duration_ms  Beep duration in milliseconds.
 * @retval None
 */
void Buzzer_Beep(const Buzzer_Config_t *cfg, uint32_t duration_ms);

/**
 * @brief  Produce a repeated beep pattern (useful for alarms).
 * @param  cfg      Pointer to Buzzer_Config_t structure.
 * @param  count    Number of beeps.
 * @param  on_ms    ON duration per beep (ms).
 * @param  off_ms   OFF duration between beeps (ms).
 * @retval None
 */
void Buzzer_BeepPattern(const Buzzer_Config_t *cfg, uint8_t count,
                        uint32_t on_ms, uint32_t off_ms);

#ifdef __cplusplus
}
#endif

#endif /* BUZZER_H */
