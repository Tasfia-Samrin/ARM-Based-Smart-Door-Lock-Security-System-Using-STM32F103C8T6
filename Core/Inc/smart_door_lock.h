#ifndef SMART_DOOR_LOCK_H
#define SMART_DOOR_LOCK_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ----------------------------------------------------------------- */
#include "stm32f1xx_hal.h"
#include "ssd1306.h"
#include "fonts.h"
#include "keypad.h"
#include "servo.h"
#include "pir_sensor.h"
#include "reed_switch.h"
#include "buzzer.h"
#include <string.h>
#include <stdio.h>

/* Configuration Defines ---------------------------------------------------- */

/** @brief  The correct PIN/password to unlock the door.
 *          '#' acts as the ENTER/confirm key. */
#define DOOR_PASSWORD           "2580"

/** @brief  Maximum number of digits the user can enter (excluding '#'). */
#define MAX_PIN_LENGTH          10

/** @brief  Maximum consecutive wrong attempts before lockout. */
#define MAX_WRONG_ATTEMPTS      3

/** @brief  Lockout duration in seconds after max wrong attempts. */
#define LOCKOUT_DURATION_SEC    15

/** @brief  Inactivity timeout in seconds: return to IDLE if no key pressed. */
#define INACTIVITY_TIMEOUT_SEC  15

/** @brief  Servo angle for LOCKED position (degrees). */
#define SERVO_ANGLE_LOCKED      0

/** @brief  Servo angle for UNLOCKED position (degrees). */
#define SERVO_ANGLE_UNLOCKED    90

/* ---- LED Pin Definitions ---- */
#define GREEN_LED_PORT          GPIOB
#define GREEN_LED_PIN           GPIO_PIN_12

#define RED_LED_PORT            GPIOB
#define RED_LED_PIN             GPIO_PIN_13

/* State Machine ------------------------------------------------------------ */

/**
 * @brief  System state enumeration
 */
typedef enum {
    STATE_IDLE = 0,        /*!< Waiting for PIR motion detection          */
    STATE_ENTER_PIN,       /*!< OLED prompts for PIN, keypad active       */
    STATE_ACCESS_GRANTED,  /*!< Correct password — unlocking door         */
    STATE_DOOR_OPEN,       /*!< Door is physically open (reed switch)     */
    STATE_ACCESS_DENIED,   /*!< Wrong password — beep + flash             */
    STATE_SYSTEM_LOCKED    /*!< Too many wrong attempts — timed lockout   */
} SystemState_t;

/* Public Function Prototypes ----------------------------------------------- */

/**
 * @brief  Initialize all peripherals and subsystems for the door lock.
 * @note   Call this once in main() before the super-loop.
 * @retval None
 */
void SmartDoorLock_Init(void);

/**
 * @brief  Run one iteration of the door lock state machine.
 * @note   Call this repeatedly inside the main while(1) loop.
 * @retval None
 */
void SmartDoorLock_Update(void);

#ifdef __cplusplus
}
#endif

#endif /* SMART_DOOR_LOCK_H */
