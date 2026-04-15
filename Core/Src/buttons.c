#include "buttons.h"

// =============================================
// Volatile flags — set by ISR, consumed by FSM
// =============================================
volatile uint8_t flag_doorbell       = 0;
volatile uint8_t flag_emergency_reset = 0;

// =============================================
// EXTI_Buttons_Init
// Configures PD0 and PD1 as input pins,
// maps them to EXTI lines 0 and 1,
// sets edge trigger, unmasks interrupts,
// and configures NVIC priorities.
// =============================================
void Buttons_Init(void)
{
    // 1. Enable clocks
    //    GPIOD → AHB1ENR bit 3
    //    SYSCFG → APB2ENR bit 14
    RCC_AHB1ENR |= (1U << 3);
    RCC_APB2ENR |= (1U << 14);

    // 2. Configure PD0 and PD1 as input
    GPIOD_MODER &= ~(0xFU);            // 00 00 for PD1 and PD0

    // 3. Enable pull-up on PD0 and PD1
    GPIOD_PUPDR &= ~(0xFU);            // clear bits [3:0]
    GPIOD_PUPDR |=  (0x5U);            // 01 01 → pull-up on PD1 and PD0

    // 4. Map EXTI lines to GPIOD via SYSCFG
    //    EXTI0 → bits [3:0]  → 0011 = GPIOD
    //    EXTI1 → bits [7:4]  → 0011 = GPIOD
    SYSCFG_EXTICR1 &= ~(0xFFU);        // clear EXTI0 and EXTI1 fields
    SYSCFG_EXTICR1 |=  (0x33U);        // 0011 0011 → both mapped to GPIOD

    // 5. Configure edge trigger
    //    FTSR bit 0 → EXTI0 (Doorbell)
    //    FTSR bit 1 → EXTI1 (Emergency Reset)
    EXTI_FTSR |= (1U << 0);            // EXTI0 falling edge
    EXTI_FTSR |= (1U << 1);            // EXTI1 falling edge

    // Make sure rising edge is NOT set for these lines
    EXTI_RTSR &= ~(1U << 0);
    EXTI_RTSR &= ~(1U << 1);

    // 6. Unmask EXTI lines (enable interrupt)
    EXTI_IMR |= (1U << 0);
    EXTI_IMR |= (1U << 1);

    // ------------------------------------------
    // 7. NVIC — Enable and set priorities
    //
    //    EXTI0 IRQ number = 6  → ISER0 bit 6
    //    EXTI1 IRQ number = 7  → ISER0 bit 7
    //
    //    Priority register: IPR is byte-accessible
    //    Each IRQ gets 8 bits in IPRx registers
    //    IPR1 covers IRQ4-IRQ7
    //    IRQ6 (EXTI0) → byte 2 of IPR1 → bits [23:16]
    //    IRQ7 (EXTI1) → byte 3 of IPR1 → bits [31:24]
    //
    //    Lower value = higher priority
    //    Emergency Reset (EXTI1) → priority 0x10 (high)
    //    Doorbell        (EXTI0) → priority 0x20 (normal)
    //
    //    STM32 uses only top 4 bits of priority byte
    //    so 0x10 = priority 1, 0x20 = priority 2
    // ------------------------------------------

    // Enable EXTI0 and EXTI1 in NVIC ISER0
    NVIC_ISER0 |= (1U << 6);           // EXTI0 IRQ6
    NVIC_ISER0 |= (1U << 7);           // EXTI1 IRQ7

    // Set priorities in NVIC_IPR1
    // Clear then set byte 2 (IRQ6 = Doorbell)
    NVIC_IPR1 &= ~(0xFFU << 16);
    NVIC_IPR1 |=  (0x20U << 16);       // priority 2 → Doorbell (normal)

    // Clear then set byte 3 (IRQ7 = Emergency Reset)
    NVIC_IPR1 &= ~(0xFFU << 24);
    NVIC_IPR1 |=  (0x10U << 24);       // priority 1 → Emergency Reset (high)
}

// =============================================
// EXTI0_IRQHandler — Door Bell (PD0)
// Does NOT disturb any lock state
// =============================================
void EXTI0_IRQHandler(void)
{
    // Check pending bit for line 0
    if (EXTI_PR & (1U << 0))
    {
        flag_doorbell = 1;

        // Clear pending bit by writing 1 to it
        EXTI_PR = (1U << 0);
    }
}

// =============================================
// EXTI1_IRQHandler — Emergency Reset (PD1)
// Sets flag with highest priority
// =============================================
void EXTI1_IRQHandler(void)
{
    // Check pending bit for line 1
    if (EXTI_PR & (1U << 1))
    {
        flag_emergency_reset = 1;

        // Clear pending bit by writing 1 to it
        EXTI_PR = (1U << 1);
    }
}