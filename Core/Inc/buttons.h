#ifndef BUTTONS_H
#define BUTTONS_H

#include "stm32f401ve.h"
#include <stdint.h>

// Door Bell Button:      PD0 → EXTI0
// Emergency Reset Button: PD1 → EXTI1

void Buttons_Init(void);

// Flags set inside ISRs, read & cleared in main loop / FSM
extern volatile uint8_t flag_doorbell;
extern volatile uint8_t flag_emergency_reset;

#endif