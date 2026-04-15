#ifndef KEYPAD_H
#define KEYPAD_H

#include "stm32f401ve.h"
// Rows: PA0-PA3 (Output)
// Cols: PA4-PA7 (Input Pull-up)

void Keypad_Init(void);
char Keypad_GetKey(void);

#endif