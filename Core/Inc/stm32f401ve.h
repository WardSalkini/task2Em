#ifndef STM32F401VE_H
#define STM32F401VE_H

#include <stdint.h>


// RCC Base: 0x40023800
// GPIOA: 0x40020000
// GPIOB: 0x40020400
// GPIOC: 0x40020800
// GPIOD: 0x40020C00
// EXTI:  0x40013C00
// SYSCFG:0x40013800
// NVIC:  0xE000E100

// =============================================
// RCC Registers (offset from 0x40023800)
// =============================================
#define RCC_AHB1ENR   (*((volatile uint32_t*)(0x40023800 + 0x30)))
#define RCC_APB2ENR   (*((volatile uint32_t*)(0x40023800 + 0x44)))

// =============================================
// GPIOA Registers (offset from 0x40020000)
// =============================================
#define GPIOA_MODER   (*((volatile uint32_t*)(0x40020000 + 0x00)))
#define GPIOA_OTYPER  (*((volatile uint32_t*)(0x40020000 + 0x04)))
#define GPIOA_OSPEEDR (*((volatile uint32_t*)(0x40020000 + 0x08)))
#define GPIOA_PUPDR   (*((volatile uint32_t*)(0x40020000 + 0x0C)))
#define GPIOA_IDR     (*((volatile uint32_t*)(0x40020000 + 0x10)))
#define GPIOA_ODR     (*((volatile uint32_t*)(0x40020000 + 0x14)))
#define GPIOA_BSRR    (*((volatile uint32_t*)(0x40020000 + 0x18)))

// =============================================
// GPIOB Registers (offset from 0x40020400)
// =============================================
#define GPIOB_MODER   (*((volatile uint32_t*)(0x40020400 + 0x00)))
#define GPIOB_OTYPER  (*((volatile uint32_t*)(0x40020400 + 0x04)))
#define GPIOB_OSPEEDR (*((volatile uint32_t*)(0x40020400 + 0x08)))
#define GPIOB_PUPDR   (*((volatile uint32_t*)(0x40020400 + 0x0C)))
#define GPIOB_IDR     (*((volatile uint32_t*)(0x40020400 + 0x10)))
#define GPIOB_ODR     (*((volatile uint32_t*)(0x40020400 + 0x14)))
#define GPIOB_BSRR    (*((volatile uint32_t*)(0x40020400 + 0x18)))

// =============================================
// GPIOC Registers (offset from 0x40020800)
// =============================================
#define GPIOC_MODER   (*((volatile uint32_t*)(0x40020800 + 0x00)))
#define GPIOC_OTYPER  (*((volatile uint32_t*)(0x40020800 + 0x04)))
#define GPIOC_OSPEEDR (*((volatile uint32_t*)(0x40020800 + 0x08)))
#define GPIOC_PUPDR   (*((volatile uint32_t*)(0x40020800 + 0x0C)))
#define GPIOC_IDR     (*((volatile uint32_t*)(0x40020800 + 0x10)))
#define GPIOC_ODR     (*((volatile uint32_t*)(0x40020800 + 0x14)))
#define GPIOC_BSRR    (*((volatile uint32_t*)(0x40020800 + 0x18)))

// =============================================
// GPIOD Registers (offset from 0x40020C00)
// =============================================
#define GPIOD_MODER   (*((volatile uint32_t*)(0x40020C00 + 0x00)))
#define GPIOD_OTYPER  (*((volatile uint32_t*)(0x40020C00 + 0x04)))
#define GPIOD_OSPEEDR (*((volatile uint32_t*)(0x40020C00 + 0x08)))
#define GPIOD_PUPDR   (*((volatile uint32_t*)(0x40020C00 + 0x0C)))
#define GPIOD_IDR     (*((volatile uint32_t*)(0x40020C00 + 0x10)))
#define GPIOD_ODR     (*((volatile uint32_t*)(0x40020C00 + 0x14)))
#define GPIOD_BSRR    (*((volatile uint32_t*)(0x40020C00 + 0x18)))

// =============================================
// EXTI Registers (offset from 0x40013C00)
// =============================================
#define EXTI_IMR      (*((volatile uint32_t*)(0x40013C00 + 0x00))) //Interrupt Mask Register
#define EXTI_EMR      (*((volatile uint32_t*)(0x40013C00 + 0x04))) //Event Mask Register
#define EXTI_RTSR     (*((volatile uint32_t*)(0x40013C00 + 0x08))) //Rising Trigger Selection Register
#define EXTI_FTSR     (*((volatile uint32_t*)(0x40013C00 + 0x0C))) //Falling Trigger Selection Register
#define EXTI_PR       (*((volatile uint32_t*)(0x40013C00 + 0x14))) //Pending Register

// =============================================
// SYSCFG Registers (offset from 0x40013800)
// =============================================
#define SYSCFG_EXTICR1 (*((volatile uint32_t*)(0x40013800 + 0x08)))
#define SYSCFG_EXTICR2 (*((volatile uint32_t*)(0x40013800 + 0x0C)))

// =============================================
// NVIC Registers (offset from 0xE000E100)
// =============================================
#define NVIC_ISER0    (*((volatile uint32_t*)(0xE000E100)))
#define NVIC_IPR1     (*((volatile uint32_t*)(0xE000E400 + 0x04)))

#endif