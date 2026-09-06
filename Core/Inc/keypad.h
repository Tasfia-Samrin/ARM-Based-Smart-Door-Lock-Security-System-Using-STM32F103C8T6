/**
 * @file    keypad.h
 * @brief   4x4 Matrix Keypad Driver for STM32F1xx
 * @details Scans a 4-row × 4-column matrix keypad using GPIO.
 *          Rows are driven as outputs, columns are read as inputs (pull-down).
 *
 *          Key Mapping:
 *            C1   C2   C3   C4
 *          ┌────┬────┬────┬────┐
 *     R1   │ 1  │ 2  │ 3  │ A  │
 *          ├────┼────┼────┼────┤
 *     R2   │ 4  │ 5  │ 6  │ B  │
 *          ├────┼────┼────┼────┤
 *     R3   │ 7  │ 8  │ 9  │ C  │
 *          ├────┼────┼────┼────┤
 *     R4   │ *  │ 0  │ #  │ D  │
 *          └────┴────┴────┴────┘
 *
 *          Default Pins:
 *            Rows (Output): PA7, PA6, PA5, PA4
 *            Cols (Input):  PA3, PA2, PA1, PA0
 */

#ifndef KEYPAD_H
#define KEYPAD_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ----------------------------------------------------------------- */
#include "stm32f1xx_hal.h"

/* Function Prototypes ------------------------------------------------------ */

/**
 * @brief  Scan the 4×4 matrix keypad and return the pressed key.
 * @note   This function uses a sequential row-scan technique with a
 *         20 ms software debounce delay when a key press is detected.
 * @param  None
 * @retval char  ASCII character of the pressed key ('0'–'9', 'A'–'D', '*', '#'),
 *               or 0 if no key is pressed.
 */
char getPressedKey(void);

#ifdef __cplusplus
}
#endif

#endif /* KEYPAD_H */
