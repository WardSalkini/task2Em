#include "state_machine.h"
#include "leds.h"
#include "display.h"
#include "buttons.h"

// =============================================
// Password
// =============================================
static const char PASSWORD[PASSWORD_LENGTH] = {'1', '2', '3', '4'};

// =============================================
// Internal variables
// =============================================
static SystemState current_state = STATE_LOCKED;
static uint8_t     progress      = 0;
static uint8_t     fail_count    = 0;
static FSM_Event   pending_event = EVENT_NONE;

// =============================================
// The Mealy Transition Function
//
// Both happen together as ONE atomic decision:
//   f(current_state, event) → (output, next_state)
//
// All outputs are tied to (state + input) pairs
// =============================================
static void FSM_Transition(SystemState state, FSM_Event event)
{
    switch (state)
    {
        case STATE_LOCKED:
            switch (event)
            {
                case EVENT_VALID_DIGIT:
                    progress++;
                    LED_SetProgress(progress);
                    current_state = STATE_LOCKED;
                    break;

                case EVENT_SEQUENCE_DONE:
                    progress = 0;
                    LED_SetProgress(0);
                    LED_SetSuccess(1);
                    current_state = STATE_UNLOCKED;
                    break;

                case EVENT_INVALID_DIGIT:
                    progress = 0;
                    LED_SetProgress(0);
                    fail_count++;
                    Display_ShowNumber(fail_count);

                    if (fail_count >= LOCKOUT_THRESHOLD)
                    {
                        LED_SetAlarm(1);
                        current_state = STATE_ALARM;
                    }
                    else
                    {
                        current_state = STATE_LOCKED;
                    }
                    break;

                case EVENT_DOORBELL:
                    // OUTPUT: pulse bell LED
                    LED_SetBell(1);
                    for (volatile uint32_t i = 0; i < 200000; i++);
                    LED_SetBell(0);
                    current_state = STATE_LOCKED;
                    break;

                default:
                    // No output, no transition
                    break;
            }
            break;

        case STATE_UNLOCKED:
            switch (event)
            {
                case EVENT_LOCK_COMMAND:
                    LED_SetSuccess(0);
                    current_state = STATE_LOCKED;
                    break;

                // All other inputs while unlocked:
                // OUTPUT: none   NEXT STATE: remain UNLOCKED
                default:
                    current_state = STATE_UNLOCKED;
                    break;
            }
            break;

        case STATE_ALARM:
            switch (event)
            {
                case EVENT_EMERGENCY:
                    // OUTPUT: clear everything, reset all counters
                    LED_SetAlarm(0);
                    LED_SetProgress(0);
                    LED_SetSuccess(0);
                    LED_SetBell(0);
                    progress   = 0;
                    fail_count = 0;
                    Display_ShowNumber(0);
                    current_state = STATE_LOCKED;
                    break;

                // All other inputs while in alarm:
                // OUTPUT: none   NEXT STATE: remain ALARM
                default:
                    current_state = STATE_ALARM;
                    break;
            }
            break;
    }
}

// =============================================
// FSM_Init
// =============================================
void FSM_Init(void)
{
    current_state = STATE_LOCKED;
    progress      = 0;
    fail_count    = 0;
    pending_event = EVENT_NONE;

    LED_ClearAll();
    Display_ShowNumber(0);
}

// =============================================
// FSM_GetState
// =============================================
SystemState FSM_GetState(void)
{
    return current_state;
}

// =============================================
// FSM_FeedKey
// Translates raw keypad char → FSM_Event
// =============================================
void FSM_FeedKey(char key)
{
    if (current_state == STATE_ALARM)
    {
        pending_event = EVENT_NONE;
        return;
    }

    if (current_state == STATE_UNLOCKED)
    {
        pending_event = (key == '*') ? EVENT_LOCK_COMMAND : EVENT_NONE;
        return;
    }

    // STATE_LOCKED: check key against password sequence
    if (key == '*')
    {
        // '*' cancels current attempt
        progress = 0;
        LED_SetProgress(0);
        pending_event = EVENT_NONE;
        return;
    }

    if (key == PASSWORD[progress])
    {
        pending_event = (progress + 1 == PASSWORD_LENGTH) //check if we entered the last password number or no
                        ? EVENT_SEQUENCE_DONE
                        : EVENT_VALID_DIGIT;
    }
    else
    {
        pending_event = EVENT_INVALID_DIGIT;
    }
}

// =============================================
// FSM_Update
// Called every main loop tick.
// Checks interrupt flags then fires FSM_Transition
// with (current_state, event) — pure Mealy call.
// =============================================
void FSM_Update(void)
{
    if (flag_emergency_reset)
    {
        flag_emergency_reset = 0;
        FSM_Transition(current_state, EVENT_EMERGENCY);
        return;
    }

    if (flag_doorbell)
    {
        flag_doorbell = 0;
        FSM_Transition(current_state, EVENT_DOORBELL);
    }

    if (pending_event == EVENT_NONE)
        return;

    FSM_Event event = pending_event;
    pending_event   = EVENT_NONE;

    FSM_Transition(current_state, event);
}