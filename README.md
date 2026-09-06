# ARM-Based Smart Door Lock & Security System Using STM32F103C8T6 

## Project Overview

This project is an ARM-based smart door lock and security system developed using the STM32F103C8T6 microcontroller. It demonstrates the use of sensors, a keypad, OLED display, buzzer, and servo motor to create a practical embedded security system for controlled and secure door access.

The system provides secure door access using a 4×4 keypad and a servo motor. It also uses a PIR motion sensor to detect movement and an OLED display to show system status. A buzzer provides audio alerts for successful and unsuccessful access attempts.

## Hardware Components

- STM32F103C8T6
- 4×4 Matrix Keypad
- SG90 Servo Motor
- PIR Sensor
- SSD1306 OLED Display
- Buzzer


# Main Features
- Password-based door access
- Keypad-based PIN entry
- Servo-controlled door locking and unlocking
- PIR-based motion detection
- OLED status display
- Access granted/denied indication
- Wrong-attempt detection
- Temporary system lockout after multiple incorrect PIN attempts
- Automatic door locking after a timeout
- Buzzer alerts


# System Operation
1. The system starts in Idle Mode with the door locked.
2. The PIR sensor detects motion.
3. The user is prompted to enter the PIN using the keypad.
4. If the PIN is correct, the servo unlocks the door.
5. If the PIN is incorrect, an access-denied alert is generated.
6. After multiple incorrect attempts, the system enters Lockout Mode.
7. The door automatically locks again after the specified timeout.


# Software & Tools
- Keil MDK-ARM
- STM32CubeMX
- Embedded C
- ARM Cortex-M3
