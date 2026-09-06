#include "buzzer.h"

/* -------------------------------------------------------------------------- */
/*  Public Functions                                                          */
/* -------------------------------------------------------------------------- */

/**
 * @brief  Initialize the buzzer GPIO pin as push-pull output, default OFF.
 */
void Buzzer_Init(const Buzzer_Config_t *cfg)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Enable GPIO clock */
    if (cfg->GPIOx == GPIOA)      __HAL_RCC_GPIOA_CLK_ENABLE();
    else if (cfg->GPIOx == GPIOB) __HAL_RCC_GPIOB_CLK_ENABLE();
    else if (cfg->GPIOx == GPIOC) __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitStruct.Pin   = cfg->GPIO_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(cfg->GPIOx, &GPIO_InitStruct);

    /* Start with buzzer OFF */
    Buzzer_Off(cfg);
}

/**
 * @brief  Turn the buzzer ON.
 */
void Buzzer_On(const Buzzer_Config_t *cfg)
{
    if (cfg->ActiveHigh)
        HAL_GPIO_WritePin(cfg->GPIOx, cfg->GPIO_Pin, GPIO_PIN_SET);
    else
        HAL_GPIO_WritePin(cfg->GPIOx, cfg->GPIO_Pin, GPIO_PIN_RESET);
}

/**
 * @brief  Turn the buzzer OFF.
 */
void Buzzer_Off(const Buzzer_Config_t *cfg)
{
    if (cfg->ActiveHigh)
        HAL_GPIO_WritePin(cfg->GPIOx, cfg->GPIO_Pin, GPIO_PIN_RESET);
    else
        HAL_GPIO_WritePin(cfg->GPIOx, cfg->GPIO_Pin, GPIO_PIN_SET);
}

/**
 * @brief  Single beep: ON → delay → OFF.
 */
void Buzzer_Beep(const Buzzer_Config_t *cfg, uint32_t duration_ms)
{
    Buzzer_On(cfg);
    HAL_Delay(duration_ms);
    Buzzer_Off(cfg);
}

/**
 * @brief  Repeated beep pattern for alarms.
 *         Example: Buzzer_BeepPattern(&buz, 3, 200, 100) → 3 beeps,
 *         200ms on, 100ms off each.
 */
void Buzzer_BeepPattern(const Buzzer_Config_t *cfg, uint8_t count,
                        uint32_t on_ms, uint32_t off_ms)
{
    for (uint8_t i = 0; i < count; i++)
    {
        Buzzer_On(cfg);
        HAL_Delay(on_ms);
        Buzzer_Off(cfg);
        if (i < (count - 1))  /* No trailing delay after last beep */
        {
            HAL_Delay(off_ms);
        }
    }
}
