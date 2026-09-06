#include "pir_sensor.h"

/* -------------------------------------------------------------------------- */
/*  Public Functions                                                          */
/* -------------------------------------------------------------------------- */

/**
 * @brief  Initialize the PIR sensor GPIO pin as floating input.
 * @note   The HC-SR501 has its own pull circuitry, so no internal pull is needed.
 *         If CubeMX already configures this pin, calling PIR_Init() is optional.
 */
void PIR_Init(const PIR_Config_t *cfg)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Enable GPIO clock (CubeMX usually does this, but just in case) */
    if (cfg->GPIOx == GPIOA)      __HAL_RCC_GPIOA_CLK_ENABLE();
    else if (cfg->GPIOx == GPIOB) __HAL_RCC_GPIOB_CLK_ENABLE();
    else if (cfg->GPIOx == GPIOC) __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitStruct.Pin   = cfg->GPIO_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;       /* HC-SR501 drives 3.3V logic */
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(cfg->GPIOx, &GPIO_InitStruct);
}

/**
 * @brief  Read the raw GPIO state of the PIR sensor output.
 * @retval GPIO_PIN_SET if motion is detected, GPIO_PIN_RESET otherwise.
 */
GPIO_PinState PIR_Read(const PIR_Config_t *cfg)
{
    return HAL_GPIO_ReadPin(cfg->GPIOx, cfg->GPIO_Pin);
}

/**
 * @brief  Convenience wrapper — returns 1 when motion is detected.
 */
uint8_t PIR_IsMotionDetected(const PIR_Config_t *cfg)
{
// reads A8   
	return (HAL_GPIO_ReadPin(cfg->GPIOx, cfg->GPIO_Pin) == GPIO_PIN_SET) ? 1 : 0;
}
