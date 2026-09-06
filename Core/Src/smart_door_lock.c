#include "smart_door_lock.h"

/* -------------------------------------------------------------------------- */
/*  Private Variables                                                         */
/* -------------------------------------------------------------------------- */

/** Current state of the system */
static SystemState_t currentState = STATE_IDLE;

/** Entered PIN buffer */
static char enteredPin[MAX_PIN_LENGTH + 1] = {0};
static uint8_t pinIndex = 0;

/** Wrong-attempt counter */
static uint8_t wrongAttempts = 0;

/** Timestamps for timeout management */
static uint32_t lastKeyPressTime  = 0;
static uint32_t lockoutStartTime  = 0;
static uint32_t stateEnterTime    = 0;

// NEWWWW
static uint8_t pirReady = 0; // determines whether the pir is ready to detect  new motion

/** Sensor / actuator configuration instances */
static PIR_Config_t pirConfig = {
    .GPIOx    = GPIOA,
    .GPIO_Pin = GPIO_PIN_8
};

static ReedSwitch_Config_t reedConfig = {
    .GPIOx    = GPIOA,
    .GPIO_Pin = GPIO_PIN_9
};

static Buzzer_Config_t buzzerConfig = {
    .GPIOx      = GPIOA,
    .GPIO_Pin   = GPIO_PIN_10,
    .ActiveHigh = 1          /* HIGH = buzzer ON */
};

/* -------------------------------------------------------------------------- */
/*  Private Helper Functions                                                  */
/* -------------------------------------------------------------------------- */

/**
 * @brief  Turn the green LED on or off.
 */
static void GreenLED(uint8_t on)
{
    HAL_GPIO_WritePin(GREEN_LED_PORT, GREEN_LED_PIN,
                      on ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/**
 * @brief  Turn the red LED on or off.
 */
static void RedLED(uint8_t on)
{
    HAL_GPIO_WritePin(RED_LED_PORT, RED_LED_PIN,
                      on ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/**
 * @brief  Lock the door: servo to 0 degree, turn off green LED.
 */
static void LockDoor(void)
{
    Servo_GotoAngle(SERVO_ANGLE_LOCKED);
    GreenLED(0);
}

/**
 * @brief  Unlock the door: servo to 90 degree, green LED on.
 */
static void UnlockDoor(void)
{
    Servo_GotoAngle(SERVO_ANGLE_UNLOCKED);
    GreenLED(1);
}

/**
 * @brief  Clear the PIN entry buffer.
 */
static void ClearPinBuffer(void)
{
    memset(enteredPin, 0, sizeof(enteredPin));
    pinIndex = 0;
}

/**
 * @brief  Display a centered string on a specific OLED line.
 * @param  line  Y pixel position (0, 16, 32, 48 for Font_7x10 rows)
 * @param  str   Null-terminated string to display
 * @param  font  Pointer to font structure
 */
static void OLED_ShowLine(uint16_t y, const char *str, FontDef_t *font)
{
    /* Calculate X for rough centering (128px wide display) */
    uint16_t strWidth = strlen(str) * font->FontWidth;
    uint16_t x = (strWidth < 128) ? (128 - strWidth) / 2 : 0;
    SSD1306_GotoXY(x, y);
    SSD1306_Puts((char *)str, font, SSD1306_COLOR_WHITE);
}

/**
 * @brief  Show the PIN entry screen with asterisks for entered digits.
 */
static void DisplayPinEntryScreen(void)
{
    char maskBuf[MAX_PIN_LENGTH + 1] = {0};

    SSD1306_Fill(SSD1306_COLOR_BLACK);

    /* Title */
    OLED_ShowLine(0, "ENTER PIN:", &Font_7x10);

    /* Build asterisk mask */
    for (uint8_t i = 0; i < pinIndex && i < MAX_PIN_LENGTH; i++)
    {
        maskBuf[i] = '*';
    }
    maskBuf[pinIndex] = '\0';

    /* Show masked PIN on second line */
    OLED_ShowLine(20, maskBuf, &Font_11x18);

    /* Show remaining attempts */
    char attemptBuf[24];
    uint8_t remaining = MAX_WRONG_ATTEMPTS - wrongAttempts;
    sprintf(attemptBuf, "Attempts: %d/%d", remaining, MAX_WRONG_ATTEMPTS);
    OLED_ShowLine(50, attemptBuf, &Font_7x10);

    SSD1306_UpdateScreen();
}

/* -------------------------------------------------------------------------- */
/*  State Handler Functions                                                   */
/* -------------------------------------------------------------------------- */

/**
 * @brief  IDLE state: display standby, wait for PIR motion.
 */


static void Handle_Idle(void)
{
    /* On first entry, show standby screen */
    static uint8_t idleScreenShown = 0;

    if (!idleScreenShown)
    {
        SSD1306_Fill(SSD1306_COLOR_BLACK);
        OLED_ShowLine(10, "SMART DOOR", &Font_7x10);
        //OLED_ShowLine(35, "LOCK SYSTEM", &Font_11x18);
			  OLED_ShowLine(35, "LOCK SYSTEM", &Font_7x10);
        OLED_ShowLine(50, "Waiting...", &Font_7x10);// 55
        SSD1306_UpdateScreen();
        idleScreenShown = 1;

        /* Make sure door is locked and LEDs off */
        LockDoor();
        RedLED(0);
			
		
			
    }
// NEWW
		    /*
     * First wait for the PIR output to become LOW.
     * This ignores any HIGH signal that may already exist when the system starts.
     */
    if (!pirReady)
    {
        if (!PIR_IsMotionDetected(&pirConfig))
        {
            pirReady = 1;
        }

        return;
    }


		
		//^ NEWWW
		
    /* Check PIR sensor */
	
    if (PIR_IsMotionDetected(&pirConfig))
    {
        /* Transition to PIN entry */
        currentState     = STATE_ENTER_PIN;
        lastKeyPressTime = HAL_GetTick();
        stateEnterTime   = HAL_GetTick();
        idleScreenShown  = 0;
        ClearPinBuffer();

        /* Short beep to indicate wake-up */
        Buzzer_Beep(&buzzerConfig, 100);

        DisplayPinEntryScreen();
    }

}
/**
 * @brief  ENTER_PIN state: read keypad, validate password.
 */
static void Handle_EnterPin(void)
{
    /* Check inactivity timeout */
    if ((HAL_GetTick() - lastKeyPressTime) > (INACTIVITY_TIMEOUT_SEC * 1000U))
    {
        /* Return to IDLE */
        SSD1306_Fill(SSD1306_COLOR_BLACK);
        OLED_ShowLine(20, "TIMEOUT", &Font_11x18);
        SSD1306_UpdateScreen();
        HAL_Delay(1000);
        currentState = STATE_IDLE;
        ClearPinBuffer();
        return;
    }

    char key = getPressedKey();
    if (key == 0) return;  /* No key pressed */

    lastKeyPressTime = HAL_GetTick();

    /* Short click feedback */
    Buzzer_Beep(&buzzerConfig, 50);

    if (key == '#')
    {
        /* ---- CONFIRM / ENTER ---- */
        enteredPin[pinIndex] = '\0';

        if (strcmp(enteredPin, DOOR_PASSWORD) == 0)
        {
            /* Correct password */
            wrongAttempts = 0;
            currentState  = STATE_ACCESS_GRANTED;
            stateEnterTime = HAL_GetTick();
        }
        else
        {
            /* Wrong password */
            wrongAttempts++;
            currentState = STATE_ACCESS_DENIED;
            stateEnterTime = HAL_GetTick();
        }
        ClearPinBuffer();
    }
    else if (key == '*')
    {
        /* ---- BACKSPACE / CLEAR ---- */
        ClearPinBuffer();
        DisplayPinEntryScreen();
    }
    else if (key >= '0' && key <= '9')
    {
        /* ---- DIGIT ENTRY ---- */
        if (pinIndex < MAX_PIN_LENGTH)
        {
            enteredPin[pinIndex++] = key;
            enteredPin[pinIndex]   = '\0';
            DisplayPinEntryScreen();
        }
    }
    /* Keys A, B, C, D are ignored for PIN entry */

    /* Small delay to avoid double-reads */
    HAL_Delay(150);
}

/**
 * @brief  ACCESS_GRANTED state: unlock door, show success.
 */
static void Handle_AccessGranted(void)
{
    /* Show success screen */
    SSD1306_Fill(SSD1306_COLOR_BLACK);
    OLED_ShowLine(5,  "ACCESS", &Font_11x18);
    OLED_ShowLine(28, "GRANTED", &Font_11x18);
    OLED_ShowLine(52, "Door Unlocked!", &Font_7x10);
    SSD1306_UpdateScreen();

    /* Visual and audio feedback */
    GreenLED(1);
    RedLED(0);
    Buzzer_BeepPattern(&buzzerConfig, 2, 100, 100);

    /* Unlock the door */
    UnlockDoor();

    /* Transition to DOOR_OPEN state */
    HAL_Delay(1000);
    currentState   = STATE_DOOR_OPEN;
    stateEnterTime = HAL_GetTick();
}

/**
 * @brief  DOOR_OPEN state: wait for door to close (reed switch), then auto-lock.
 */
static void Handle_DoorOpen(void)
{
    /* Show waiting screen */
    static uint8_t doorOpenScreenShown = 0;

    if (!doorOpenScreenShown)
    {
        SSD1306_Fill(SSD1306_COLOR_BLACK);
        OLED_ShowLine(5,  "DOOR OPEN", &Font_11x18);
        OLED_ShowLine(30, "Close door to", &Font_7x10);
        OLED_ShowLine(42, "auto-lock...", &Font_7x10);
        SSD1306_UpdateScreen();
        doorOpenScreenShown = 1;
    }

    /* Check reed switch — is the door closed? */
    if (ReedSwitch_IsDoorClosed(&reedConfig))
    {
        /* Door has been closed — auto-lock */
        LockDoor();

        SSD1306_Fill(SSD1306_COLOR_BLACK);
        OLED_ShowLine(5,  "DOOR", &Font_11x18);
        OLED_ShowLine(28, "LOCKED", &Font_11x18);
        SSD1306_UpdateScreen();

        Buzzer_Beep(&buzzerConfig, 200);
        GreenLED(0);

        HAL_Delay(2000);

        doorOpenScreenShown = 0;
        currentState = STATE_IDLE;
    }

    /* Optional: auto-lock after 30 seconds even if door stays open */
    if ((HAL_GetTick() - stateEnterTime) > 30000U)
    {
        LockDoor();

        SSD1306_Fill(SSD1306_COLOR_BLACK);
        OLED_ShowLine(5,  "AUTO-LOCK", &Font_11x18);
        OLED_ShowLine(28, "TIMEOUT", &Font_11x18);
        SSD1306_UpdateScreen();

        Buzzer_BeepPattern(&buzzerConfig, 3, 150, 100);
        GreenLED(0);

        HAL_Delay(2000);

        doorOpenScreenShown = 0;
        currentState = STATE_IDLE;
    }
}

/**
 * @brief  ACCESS_DENIED state: flash red LED, beep, show error.
 */
static void Handle_AccessDenied(void)
{
    /* Show denied screen */
    SSD1306_Fill(SSD1306_COLOR_BLACK);
    OLED_ShowLine(5,  "ACCESS", &Font_11x18);
    OLED_ShowLine(28, "DENIED!", &Font_11x18);

    char buf[24];
    sprintf(buf, "Wrong! (%d/%d)", wrongAttempts, MAX_WRONG_ATTEMPTS);
    OLED_ShowLine(52, buf, &Font_7x10);
    SSD1306_UpdateScreen();

    /* Flash red LED and beep */
    GreenLED(0);
    for (uint8_t i = 0; i < 3; i++)
    {
        RedLED(1);
        Buzzer_On(&buzzerConfig);
        HAL_Delay(200);
        RedLED(0);
        Buzzer_Off(&buzzerConfig);
        HAL_Delay(200);
    }

    /* Check if we should lock the system */
    if (wrongAttempts >= MAX_WRONG_ATTEMPTS)
    {
        currentState    = STATE_SYSTEM_LOCKED;
        lockoutStartTime = HAL_GetTick();
    }
    else
    {
        /* Let user try again */
        currentState     = STATE_ENTER_PIN;
        lastKeyPressTime = HAL_GetTick();
        ClearPinBuffer();
        DisplayPinEntryScreen();
    }
}

/**
 * @brief  SYSTEM_LOCKED state: lockout for LOCKOUT_DURATION_SEC seconds.
 */
static void Handle_SystemLocked(void)
{
    uint32_t elapsed_ms = HAL_GetTick() - lockoutStartTime;
    uint32_t remaining_sec = 0;

    if (elapsed_ms < (LOCKOUT_DURATION_SEC * 1000U))
    {
        remaining_sec = LOCKOUT_DURATION_SEC - (elapsed_ms / 1000U);
    }

    /* Update display with countdown */
    SSD1306_Fill(SSD1306_COLOR_BLACK);
    OLED_ShowLine(0, "!! SYSTEM !!", &Font_11x18);
    OLED_ShowLine(22, "!! LOCKED !!", &Font_11x18);

    char buf[24];
    sprintf(buf, "Wait: %lu sec", (unsigned long)remaining_sec);
    OLED_ShowLine(48, buf, &Font_7x10);
    SSD1306_UpdateScreen();

    /* Red LED on, intermittent buzzer */
    RedLED(1);
    Buzzer_Beep(&buzzerConfig, 100);
    HAL_Delay(900);  /* ~1 second per loop iteration */

    /* Check if lockout has expired */
    if (elapsed_ms >= (LOCKOUT_DURATION_SEC * 1000U))
    {
        /* Reset and go back to IDLE */
        wrongAttempts = 0;
        RedLED(0);
        Buzzer_Off(&buzzerConfig);
        currentState = STATE_IDLE;

        SSD1306_Fill(SSD1306_COLOR_BLACK);
        OLED_ShowLine(15, "SYSTEM", &Font_11x18);
        OLED_ShowLine(38, "UNLOCKED", &Font_11x18);
        SSD1306_UpdateScreen();
        HAL_Delay(2000);
    }
}

/* -------------------------------------------------------------------------- */
/*  Public Functions                                                          */
/* -------------------------------------------------------------------------- */

/**
 * @brief  Initialize all peripherals for the Smart Door Lock system.
 *         Call once in main() after HAL_Init() and SystemClock_Config().
 */
void SmartDoorLock_Init(void)
{
    /* Initialize OLED display */
    SSD1306_Init();

    /* Initialize sensors */
    PIR_Init(&pirConfig);
    ReedSwitch_Init(&reedConfig);
    Buzzer_Init(&buzzerConfig);

    /* Ensure door starts locked */
    LockDoor();
    GreenLED(0);
    RedLED(0);

    /* Clear PIN buffer */
    ClearPinBuffer();

    /* Set initial state */
    currentState  = STATE_IDLE;
    wrongAttempts = 0;
	//NEWWWW
	//pirStartupTime = HAL_GetTick();
    pirReady = 0;

    /* Startup indication */
    SSD1306_Fill(SSD1306_COLOR_BLACK);
    //OLED_ShowLine(5,  "SMART DOOR", &Font_11x18);
		OLED_ShowLine(5,  "SMART DOOR", &Font_7x10);
    //OLED_ShowLine(28, "LOCK v1.0", &Font_11x18);
		OLED_ShowLine(28, "LOCK v1.0", &Font_7x10);
    OLED_ShowLine(52, "Initializing...", &Font_7x10);
    SSD1306_UpdateScreen();

    Buzzer_BeepPattern(&buzzerConfig, 3, 80, 80);
    HAL_Delay(2000);
}

/**
 * @brief  Run one iteration of the state machine.
 *         Call this in the main while(1) loop.
 *
 *         Example usage in main.c:
 *         @code
 *         #include "smart_door_lock.h"
 *
 *         int main(void)
 *         {
 *             HAL_Init();
 *             SystemClock_Config();
 *             MX_GPIO_Init();
 *             MX_I2C1_Init();
 *             MX_TIM4_Init();
 *
 *             SmartDoorLock_Init();
 *
 *             while (1)
 *             {
 *                 SmartDoorLock_Update();
 *             }
 *         }
 *         @endcode
 */
void SmartDoorLock_Update(void)
{
    switch (currentState)
    {
        case STATE_IDLE:
            Handle_Idle();
            break;

        case STATE_ENTER_PIN:
            Handle_EnterPin();
            break;

        case STATE_ACCESS_GRANTED:
            Handle_AccessGranted();
            break;

        case STATE_DOOR_OPEN:
            Handle_DoorOpen();
            break;

        case STATE_ACCESS_DENIED:
            Handle_AccessDenied();
            break;

        case STATE_SYSTEM_LOCKED:
            Handle_SystemLocked();
            break;

        default:
            currentState = STATE_IDLE;
            break;
    }

	
}
