#include "uart.h"

#define APB1_CLK 16000000UL // Default HSI clock
#define BAUD     115200UL

static volatile uint8_t last_byte;
static volatile bool got_byte;

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
	if (USART2->SR & USART_SR_RXNE)
	{
		last_byte = (uint8_t)USART2->DR;
		got_byte = true;
	}
}

bool UART2_TakeByte(uint8_t *out)
{
	if (!got_byte)
		return false;
	*out = last_byte;
	got_byte = false;
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
