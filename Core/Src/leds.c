#include "leds.h"
#include "stm32f401ve.h"

// LED Bar (Progress): PB0-PB3
// Success LED: PB4
// Alarm LED: PB5
// Bell LED: PB6

void LED_Init(void)
{
    // 1. Enable GPIOB clock 
    RCC_AHB1ENR |= (1U << 1);

    // 2. Set PB0-PB6 as output (MODER = 01 for each pin)
    GPIOB_MODER &= ~(0x3FFF);          // clear bits [13:0] 
    GPIOB_MODER |=  (0x1555);          // 01 01 01 01 01 01 01 

    // 3. Push-pull output (OTYPER bit = 0 for each, default but explicit)
    GPIOB_OTYPER &= ~(0x7F);           

    // 4. Low speed (OSPEEDR = 00 for each pin)
    GPIOB_OSPEEDR &= ~(0x3FFF);        

    // 5. No pulln (PUPDR = 00 for each pin)
    GPIOB_PUPDR &= ~(0x3FFF);          

    // 6. Start with all LEDs OFF
    LED_ClearAll();
}


// LED_SetProgress
// count = 0 , all off
// count = 1 , PB0 on
// count = 2 , PB0,PB1 on
// count = 3 , PB0,PB1,PB2 on
// count = 4 , all 4 on 
void LED_SetProgress(uint8_t count)
{
    // Clamp to max 4
    if (count > 4) count = 4;

    // Clear PB0-PB3 first
    GPIOB_BSRR = (0xFU << 16);      

    // Build a mask of 'count' bits starting at bit 0
    if (count > 0)
    {
        uint32_t mask = (1U << count) - 1U; // e.g. count=3 , 0b0111
        GPIOB_BSRR = mask;                  
    }
}

// LED_SetSuccess
// state = 1 → turn ON  PB4
// state = 0 → turn OFF PB4
void LED_SetSuccess(uint8_t state)
{
    if (state)
        LED_SET(4);
    else
        LED_RESET(4);
}

void LED_SetAlarm(uint8_t state)
{
    if (state)
        LED_SET(5);
    else
        LED_RESET(5);
}

void LED_SetBell(uint8_t state)
{
    if (state)
        LED_SET(6);
    else
        LED_RESET(6);
}

// LED_ClearAll
void LED_ClearAll(void)
{
    // Writing 1 to bits [22:16] resets PB0-PB6
    GPIOB_BSRR = (0x7FU << 16);
}