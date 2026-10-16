#ifndef UART_H
#define UART_H

#include "stm32f446xx.h"
#include <stdbool.h>
#include <stdint.h>

// Onboard LED (LD2), PA5
void LED_Init(void); // Clock + PA5 to output
void LED_On(void);
void LED_Off(void);

// USART2 PA2 (TX) and PA3 (RX)
void UART2_Init(void);
void UART2_SendChar(char c);
void UART2_SendString(const char *s);
void UART2_SendUInt(uint32_t n);

#endif
