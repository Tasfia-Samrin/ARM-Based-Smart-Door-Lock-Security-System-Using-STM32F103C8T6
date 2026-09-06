#include "reed_switch.h"

/* -------------------------------------------------------------------------- */
/*  Public Functions                                                          */
/* -------------------------------------------------------------------------- */

/**
 * @brief  Initialize the reed switch GPIO pin as input with internal pull-up.
 */
void ReedSwitch_Init(const ReedSwitch_Config_t *cfg)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Enable GPIO clock */
    if (cfg->GPIOx == GPIOA)      __HAL_RCC_GPIOA_CLK_ENABLE();
    else if (cfg->GPIOx == GPIOB) __HAL_RCC_GPIOB_CLK_ENABLE();
    else if (cfg->GPIOx == GPIOC) __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitStruct.Pin   = cfg->GPIO_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;        /* Pull-up: open switch → HIGH */
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(cfg->GPIOx, &GPIO_InitStruct);
}

/**
 * @brief  Read the raw GPIO state of the reed switch.
 * @retval GPIO_PIN_RESET = switch closed (door closed),
 *         GPIO_PIN_SET   = switch open   (door open).
 */
GPIO_PinState ReedSwitch_Read(const ReedSwitch_Config_t *cfg)
{
    return HAL_GPIO_ReadPin(cfg->GPIOx, cfg->GPIO_Pin);
}

/**
 * @brief  Returns 1 when the door is OPEN (pin HIGH — switch contacts open).
 */
uint8_t ReedSwitch_IsDoorOpen(const ReedSwitch_Config_t *cfg)
{
    return (HAL_GPIO_ReadPin(cfg->GPIOx, cfg->GPIO_Pin) == GPIO_PIN_SET) ? 1 : 0;
}

/**
 * @brief  Returns 1 when the door is CLOSED (pin LOW — switch contacts closed).
 */
uint8_t ReedSwitch_IsDoorClosed(const ReedSwitch_Config_t *cfg)
{
    return (HAL_GPIO_ReadPin(cfg->GPIOx, cfg->GPIO_Pin) == GPIO_PIN_RESET) ? 1 : 0;
}
