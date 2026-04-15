#include "stm32f401ve.h"
#include "keypad.h"
#include "leds.h"
#include "display.h"
#include "buttons.h"
#include "state_machine.h"

// =============================================
// SystemInit_Custom
// Initializes all peripherals in the correct
// order before the main loop starts
// =============================================
void SystemInit_Custom(void)
{
    LED_Init();            // PB0-PB6 outputs
    Display_Init();        // PC0-PC6 outputs
    Buttons_Init();   // PD0 doorbell, PD1 emergency (EXTI + NVIC)
    Keypad_Init();         // PA0-PA3 rows out, PA4-PA6 cols in pull-up
    FSM_Init();            // state = LOCKED, clear all outputs
}

// =============================================
// main
// =============================================
int main(void)
{
    SystemInit_Custom();

    while (1)
    {
        // 1. Scan keypad — blocking until release is
        //    handled inside Keypad_GetKey()
        char key = Keypad_GetKey();

        // 2. Feed key into FSM only if a key was pressed
        if (key != 0)
        {
            FSM_FeedKey(key);
        }

        // 3. Run FSM tick — checks interrupt flags
        //    (doorbell, emergency) and processes
        //    any pending key event
        FSM_Update();
    }
}