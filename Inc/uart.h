#ifndef UART_H
#define UART_H

#include "stm32f446xx.h"
#include <stdbool.h>
#include <stdint.h>

// Ring buffer : Power of 2 to wrap index while avoiding % operator
#define RX_SIZE 128
#define RX_MASK (RX_SIZE - 1)

bool UART2_ReadByte(uint8_t *out); // False if nothing waiting

// Onboard LED (LD2), PA5
void LED_Init(void); // Clock + PA5 to output
void LED_On(void);
void LED_Off(void);

// USART2 PA2 (TX) and PA3 (RX)
void UART2_Init(void);
void UART2_SendChar(char c);
void UART2_SendString(const char *s); // number as text
void UART2_SendUInt(uint32_t n);

bool UART2_TakeByte(uint8_t *out);

// Error counters
uint32_t UART2_Dropped(void); // bytes lost due to full buffer, main too slow
uint32_t UART2_Overruns(void); // bytes lost in hardware, ISR too slow

#endif
