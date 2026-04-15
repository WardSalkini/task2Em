#ifndef LEDS_H
#define LEDS_H

#include "stm32f401ve.h"
#include <stdint.h>

// LED Bar (Progress): PB0-PB3
// Success LED: PB4
// Alarm LED: PB5
// Bell LED: PB6

// =============================================
// Helper Macros using BSRR
// BSRR low 16 bits  = SET   (write 1 to set pin HIGH)
// BSRR high 16 bits = RESET (write 1 to set pin LOW)
// =============================================
#define LED_SET(pin)    (GPIOB_BSRR = (1U << (pin)))
#define LED_RESET(pin)  (GPIOB_BSRR = (1U << ((pin) + 16)))

void LED_Init(void);
void LED_SetProgress(uint8_t count);
void LED_SetSuccess(uint8_t state);
void LED_SetAlarm(uint8_t state);
void LED_SetBell(uint8_t state);
void LED_ClearAll(void);

#endif