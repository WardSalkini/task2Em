#include "keypad.h"
#include <stdint.h>
#include "stm32f401ve.h"

static char keys[4][3] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
};

void Keypad_Init(void) {
    // Enable GPIOA clock
    RCC_AHB1ENR  |= (1U << 0);
RCC_AHB1ENR  |= (1U << 0);
GPIOA_MODER  |= (1U << 0);
    // Set PA0-PA3 as OUTPUT (Rows)
    GPIOA_MODER |= (1 << 0);   // PA0
    GPIOA_MODER |= (1 << 2);   // PA1
    GPIOA_MODER |= (1 << 4);   // PA2
    GPIOA_MODER |= (1 << 6);   // PA3

    // Set PA4-PA6 as INPUT (Cols) - only 3 columns now
    GPIOA_MODER &= ~(3 << 8);  // PA4
    GPIOA_MODER &= ~(3 << 10); // PA5
    GPIOA_MODER &= ~(3 << 12); // PA6

    // Enable Pull-up on PA4-PA6
    GPIOA_MODER |= (1 << 8);   // PA4
    GPIOA_MODER |= (1 << 10);  // PA5
    GPIOA_MODER |= (1 << 12);  // PA6
}

char Keypad_GetKey(void) {
    uint8_t row, col;

    for (row = 0; row < 4; row++) {
        // Set all rows HIGH
        GPIOA_ODR |= (0xF << 0);

        // Set current row LOW
        GPIOA_ODR &= ~(1 << row);

        // Small delay
        for (volatile uint32_t i = 0; i < 1000; i++);

        // Read columns - only 3 columns now
        for (col = 0; col < 3; col++) {
            if (!(GPIOA_IDR & (1 << (col + 4)))) {
                // Wait for key release
                while (!(GPIOA_IDR & (1 << (col + 4))));
                return keys[row][col];
            }
        }
    }
    return 0;
}