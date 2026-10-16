#include "uart.h"

int main(void)
{
	uint8_t c;

	LED_Init();
	UART2_Init();
	UART2_SendString("Type Something\r\n");

    while (1)
    {
    	if (UART2_ReadByte(&c)) // Check for byte, let ISE receive
    		UART2_SendChar((char)c);
    }
}
