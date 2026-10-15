#ifndef STM32F446XX_H
#define STM32F446XX_H

#include <stdint.h>

// Peripheral Base Addresses
#define PERIPH_BASE           ((uint32_t)0x40000000)
#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000)
#define APB1PERIPH_BASE       (PERIPH_BASE)

#define GPIOA_BASE            (AHB1PERIPH_BASE + 0x0000)
#define RCC_BASE              (AHB1PERIPH_BASE + 0x3800)
#define USART2_BASE           (APB1PERIPH_BASE + 0x4400)

// GPIO Register Map
typedef struct {
    volatile uint32_t MODER;    // Mode register
    volatile uint32_t OTYPER;   // Output type register
    volatile uint32_t OSPEEDR;  // Output speed register
    volatile uint32_t PUPDR;    // Pull-up/pull-down register
    volatile uint32_t IDR;      // Input data register
    volatile uint32_t ODR;      // Output data register
    volatile uint32_t BSRR;     // Bit set/reset register
    volatile uint32_t LCKR;     // Lock register
    volatile uint32_t AFR[2];   // Alternate function registers (AFR[0]=AFRL, AFR[1]=AFRH)
} GPIO_TypeDef;

// RCC Register Map
typedef struct {
    volatile uint32_t CR;
    volatile uint32_t PLLCFGR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t AHB1RSTR;
    volatile uint32_t AHB2RSTR;
    volatile uint32_t AHB3RSTR;
    uint32_t RESERVED0;
    volatile uint32_t APB1RSTR;
    volatile uint32_t APB2RSTR;
    uint32_t RESERVED1[2];
    volatile uint32_t AHB1ENR;
    volatile uint32_t AHB2ENR;
    volatile uint32_t AHB3ENR;
    uint32_t RESERVED2;
    volatile uint32_t APB1ENR;
    volatile uint32_t APB2ENR;
} RCC_TypeDef;

// USART Register Map
typedef struct {
    volatile uint32_t SR;   // Status register
    volatile uint32_t DR;   // Data register
    volatile uint32_t BRR;  // Baud rate register
    volatile uint32_t CR1;  // Control register 1
    volatile uint32_t CR2;  // Control register 2
    volatile uint32_t CR3;  // Control register 3
    volatile uint32_t GTPR; // Guard time and prescaler register
} USART_TypeDef;

#define GPIOA               ((GPIO_TypeDef *) GPIOA_BASE)
#define RCC                 ((RCC_TypeDef *) RCC_BASE)
#define USART2              ((USART_TypeDef *) USART2_BASE)

// Bit Definitions
#define RCC_AHB1ENR_GPIOAEN (1U << 0)
#define RCC_APB1ENR_USART2EN (1U << 17)

#define GPIO_MODER_MODE2_1  (1U << 5)
#define GPIO_MODER_MODE3_1  (1U << 7)
#define GPIO_MODER_MODE2    (3U << 4)
#define GPIO_MODER_MODE3    (3U << 6)

#define USART_SR_TXE        (1U << 7)
#define USART_SR_RXNE       (1U << 5)

#define USART_CR1_UE        (1U << 13)
#define USART_CR1_TE        (1U << 3)
#define USART_CR1_RE        (1U << 2)
#define USART_CR1_RXNEIE    (1U << 5)

#define USART2_IRQn         38

// NVIC Enable / Priority Functions
static inline void NVIC_EnableIRQ(int32_t IRQn) {
    volatile uint32_t *NVIC_ISER = (volatile uint32_t *)(0xE000E100);
    NVIC_ISER[IRQn >> 5] = (1U << (IRQn & 0x1F));
}

static inline void NVIC_SetPriority(int32_t IRQn, uint32_t priority) {
    volatile uint8_t *NVIC_IPR = (volatile uint8_t *)(0xE000E400);
    NVIC_IPR[IRQn] = (uint8_t)((priority << 4) & 0xFF);
}

#endif // STM32F446XX_H
