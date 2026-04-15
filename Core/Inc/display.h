#ifndef DISPLAY_H
#define DISPLAY_H

#include "stm32f401ve.h"
#include <stdint.h>

// 7-Segment Display on GPIOC
// PC0 → segment a
// PC1 → segment b
// PC2 → segment c
// PC3 → segment d 
// PC4 → segment e
// PC5 → segment f
// PC6 → segment g
// Common Cathode: HIGH = segment ON

void Display_Init(void);
void Display_ShowNumber(uint8_t num);   
void Display_Clear(void);

#endif