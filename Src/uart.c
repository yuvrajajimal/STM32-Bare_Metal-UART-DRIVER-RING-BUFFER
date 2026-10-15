#include "uart.h"

void LED_Init(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER &= ~(3U << (5 * 2));
	GPIOA->MODER |= (1U << (5 * 2));
}

void LED_On(void)
{
	GPIOA->ODR |= (1U << 5);
}

void LED_Off(void)
{
	GPIOA->ODR &= ~(1U << 5);
}
