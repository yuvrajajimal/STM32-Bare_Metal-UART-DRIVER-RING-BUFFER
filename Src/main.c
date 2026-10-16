#include "uart.h"

int main(void)
{
	LED_Init();
	UART2_Init();

	uint32_t count = 0;

    while (1)
    {
    	// For Testing
    	UART2_SendString("count: ");
    	UART2_SendUInt(count++);
    	UART2_SendString("\r\n");
    	for (volatile uint32_t i = 0; i < 500000; i ++){} // Crude delay
    }
}
