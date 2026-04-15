#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stdint.h>

// =============================================
// System States
// =============================================
typedef enum {
    STATE_LOCKED   = 0,
    STATE_UNLOCKED = 1,
    STATE_ALARM    = 2
} SystemState;

// =============================================
// Events that the FSM can receive
// =============================================
typedef enum {
    EVENT_NONE          = 0,
    EVENT_VALID_DIGIT   = 1,   // correct next digit in sequence entered
    EVENT_INVALID_DIGIT = 2,   // wrong digit entered
    EVENT_SEQUENCE_DONE = 3,   // last correct digit → sequence complete
    EVENT_LOCK_COMMAND  = 4,   // re-lock command 
    EVENT_DOORBELL      = 5,   // doorbell button pressed
    EVENT_EMERGENCY     = 6    // emergency reset button pressed
} FSM_Event;


#define PASSWORD_LENGTH     4    // number of digits in the unlock sequence
#define LOCKOUT_THRESHOLD   3    // failed attempts before ALARM

void FSM_Init(void);
void FSM_Update(void);           // call this every main loop iteration

// Called by keypad driver to feed key presses into FSM
void FSM_FeedKey(char key);

// Read current state (useful for debug or display logic)
SystemState FSM_GetState(void);


#endif