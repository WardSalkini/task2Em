#include "display.h"

// =============================================
// Common Cathode 7-segment encoding
// Bit position: gfedcba → maps to PC6..PC0
//
//   Segment layout:
//       a
//      ___
//   f |   | b
//      _g_
//   e |   | c
//      ___
//       d
//
// =============================================
static const uint8_t SEG_TABLE[11] = {
    //gfedcba
    0b0111111,   // 0 → a b c d e f     ON  
    0b0000110,   // 1 → b c             ON  
    0b1011011,   // 2 → a b d e g       ON
    0b1001111,   // 3 → a b c d g       ON
    0b1100110,   // 4 → b c f g         ON
    0b1101101,   // 5 → a c d f g       ON
    0b1111101,   // 6 → a c d e f g     ON
    0b0000111,   // 7 → a b c           ON
    0b1111111,   // 8 → all ON
    0b1101111,   // 9 → a b c d f g     ON
};



// =============================================
// Display_Init
// =============================================
void Display_Init(void)
{
    // 1. Enable GPIOC clock 
    RCC_AHB1ENR |= (1U << 2);

    // 2. Set PC0-PC6 as output (MODER = 01 for each)
    GPIOC_MODER &= ~(0x3FFF);          // clear bits [13:0]
    GPIOC_MODER |=  (0x1555);          // 01 for each of PC0-PC6

    // 3. Push-pull (OTYPER bits [6:0] = 0)
    GPIOC_OTYPER &= ~(0x7F);

    // 4. Low speed (OSPEEDR bits [13:0] = 00)
    GPIOC_OSPEEDR &= ~(0x3FFF);

    // 5. No pull-up/pull-down (PUPDR bits [13:0] = 00)
    GPIOC_PUPDR &= ~(0x3FFF);

    // 6. Start with display cleared
    Display_Clear();
}

// =============================================
// Display_ShowNumber
// Writes the correct segment pattern for
// =============================================
void Display_ShowNumber(uint8_t num)
{
    // Clamp to valid range
    if (num > 9) num = 9;
    if (num < 0) num = 0;

    // Clear PC0-PC6 first 
    Display_Clear();

    // SEG_TABLE[num] bits [6:0] map directly to PC6..PC0
    GPIOC_ODR |= (SEG_TABLE[num] & 0x7F);
}

void Display_Clear(void)
{
    GPIOC_ODR &= ~(0x7F);
}