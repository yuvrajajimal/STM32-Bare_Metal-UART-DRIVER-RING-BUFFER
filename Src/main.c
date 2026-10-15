#include "uart.h"

int main(void)
{
	LED_Init();

    while (1)
    {
    	LED_On();
    	for (volatile uint32_t i = 0; i < 500000; i ++){}
    	LED_Off();
    	for (volatile uint32_t i = 0; i < 500000; i ++){}
    }
}
