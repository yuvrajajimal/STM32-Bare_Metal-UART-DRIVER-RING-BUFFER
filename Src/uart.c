#include "uart.h"

#define APB1_CLK 16000000UL // Default HSI clock
#define BAUD     115200UL

#ifndef USART_SR_ORE
#define USART_SR_ORE (1U << 3)
#endif

static volatile uint32_t dropped;
static volatile uint32_t overruns;

static volatile uint8_t rx_buf[RX_SIZE];
static volatile uint8_t rx_head; // Where ISR writes next
static volatile uint8_t rx_tail; // Where mai reads next

void LED_Init(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // Peripheral Clock
	GPIOA->MODER &= ~(3U << (5 * 2));
	GPIOA->MODER |= (1U << (5 * 2)); // Output
}

void LED_On(void)
{
	GPIOA->ODR |= (1U << 5);
}

void LED_Off(void)
{
	GPIOA->ODR &= ~(1U << 5);
}

void USART2_IRQHandler(void)
{
	uint32_t sr = USART2->SR; // Single snapshot

	if (!(sr & (USART_SR_RXNE | USART_SR_ORE))) // Byte received or overrun
		return;

	uint8_t b = (uint8_t)USART2->DR; // Read once

	if (sr & USART_SR_ORE) // Overrun
		overruns++;

	if (sr & USART_SR_RXNE)
	{
		uint16_t next = (rx_head + 1) & RX_MASK; // Wrap 127 to 0

		if (next == rx_tail) // full buffer
		{
			dropped++;
		}
		else
		{
			// Store byte, advance head
			rx_buf[rx_head] = b;
			rx_head = next;
		}
	}
}

uint32_t UART2_Dropped(void){ return dropped;	}
uint32_t UART2_Overruns(void) { return overruns;  }

bool UART2_ReadByte(uint8_t *out)
{
	if (rx_head == rx_tail) // Empty buffer
		return false;
	// Copy byte, advance
	*out = rx_buf[rx_tail];
	rx_tail = (rx_tail + 1) & RX_MASK;
	return true;
}

void UART2_Init(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // pins
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN; // USART2

	// PA2, PA3 alternate function mode
	GPIOA->MODER &= ~(GPIO_MODER_MODE2 | GPIO_MODER_MODE3);
	GPIOA->MODER |= (GPIO_MODER_MODE2_1 | GPIO_MODER_MODE3_1);
	GPIOA->AFR[0] &= ~((0xFUL << 8) | (0xFUL << 12));
	GPIOA->AFR[0] |= ((7UL << 8) | (7UL << 12));

	USART2->BRR = (APB1_CLK + BAUD / 2) / BAUD; // 16MHz / 115200 = 138.9 -> 139 due to BAUD/2
	USART2->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE | USART_CR1_UE;

	NVIC_SetPriority(USART2_IRQn, 2);
	NVIC_EnableIRQ(USART2_IRQn);
}

void UART2_SendChar(char c)
{
	while (!(USART2->SR & USART_SR_TXE)) // Wait for register to be empty
		;
	USART2->DR = (uint8_t)c;
}

void UART2_SendString(const char *s)
{
	while (*s)
		UART2_SendChar(*s++);
}

void UART2_SendUInt(uint32_t n)
{
	char tmp[10]; // ten digits, 32-bit number
	int i = 0;

	if (n == 0) // For 0
	{
		UART2_SendChar('0');
		return;
	}
	while (n)
	{
		tmp[i++] = '0' + (n % 10); // digit -> character
		n /= 10;
	}
	while (i)
		UART2_SendChar(tmp[--i]); // Reverse
}
