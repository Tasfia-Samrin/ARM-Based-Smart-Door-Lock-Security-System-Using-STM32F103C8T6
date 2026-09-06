#include "keypad.h"

char getPressedKey(void)
{
    char key = 0;  // Default: no key pressed

    /* ------------------- CONFIGURATION ------------------- */
    /* -------- PORT VARIABLES -------- */
    GPIO_TypeDef *portRow = GPIOA; // change to your rows GPIO port - output
    GPIO_TypeDef *portCol = GPIOA; // change to your columns GPIO port - Input/pull down

    /* -------- PIN VARIABLES -------- */
    uint16_t rowPins[4] = {GPIO_PIN_7, GPIO_PIN_6, GPIO_PIN_5, GPIO_PIN_4}; // change to your row GPIO pins
    uint16_t colPins[4] = {GPIO_PIN_3, GPIO_PIN_2, GPIO_PIN_1, GPIO_PIN_0}; // change to your column GPIO pins
    /* ----------------------------------------------------- */

    // -------- ROW 1 --------
    HAL_GPIO_WritePin(portRow, rowPins[0], GPIO_PIN_SET);
    HAL_GPIO_WritePin(portRow, rowPins[1], GPIO_PIN_RESET);
    HAL_GPIO_WritePin(portRow, rowPins[2], GPIO_PIN_RESET);
    HAL_GPIO_WritePin(portRow, rowPins[3], GPIO_PIN_RESET);

    if (HAL_GPIO_ReadPin(portCol, colPins[0]) == GPIO_PIN_SET) key = '1';
    else if (HAL_GPIO_ReadPin(portCol, colPins[1]) == GPIO_PIN_SET) key = '2';
    else if (HAL_GPIO_ReadPin(portCol, colPins[2]) == GPIO_PIN_SET) key = '3';
    else if (HAL_GPIO_ReadPin(portCol, colPins[3]) == GPIO_PIN_SET) key = 'A';

    // -------- ROW 2 --------
    HAL_GPIO_WritePin(portRow, rowPins[0], GPIO_PIN_RESET);
    HAL_GPIO_WritePin(portRow, rowPins[1], GPIO_PIN_SET);

    if (HAL_GPIO_ReadPin(portCol, colPins[0]) == GPIO_PIN_SET) key = '4';
    else if (HAL_GPIO_ReadPin(portCol, colPins[1]) == GPIO_PIN_SET) key = '5';
    else if (HAL_GPIO_ReadPin(portCol, colPins[2]) == GPIO_PIN_SET) key = '6';
    else if (HAL_GPIO_ReadPin(portCol, colPins[3]) == GPIO_PIN_SET) key = 'B';

    // -------- ROW 3 --------
    HAL_GPIO_WritePin(portRow, rowPins[1], GPIO_PIN_RESET);
    HAL_GPIO_WritePin(portRow, rowPins[2], GPIO_PIN_SET);

    if (HAL_GPIO_ReadPin(portCol, colPins[0]) == GPIO_PIN_SET) key = '7';
    else if (HAL_GPIO_ReadPin(portCol, colPins[1]) == GPIO_PIN_SET) key = '8';
    else if (HAL_GPIO_ReadPin(portCol, colPins[2]) == GPIO_PIN_SET) key = '9';
    else if (HAL_GPIO_ReadPin(portCol, colPins[3]) == GPIO_PIN_SET) key = 'C';

    // -------- ROW 4 --------
    HAL_GPIO_WritePin(portRow, rowPins[2], GPIO_PIN_RESET);
    HAL_GPIO_WritePin(portRow, rowPins[3], GPIO_PIN_SET);

    if (HAL_GPIO_ReadPin(portCol, colPins[0]) == GPIO_PIN_SET) key = '*';
    else if (HAL_GPIO_ReadPin(portCol, colPins[1]) == GPIO_PIN_SET) key = '0';
    else if (HAL_GPIO_ReadPin(portCol, colPins[2]) == GPIO_PIN_SET) key = '#';
    else if (HAL_GPIO_ReadPin(portCol, colPins[3]) == GPIO_PIN_SET) key = 'D';

    // -------- DEBOUNCE --------
    if (key != 0)
    {
        HAL_Delay(20);
        return key;
    }

    return 0;  // No key pressed
}