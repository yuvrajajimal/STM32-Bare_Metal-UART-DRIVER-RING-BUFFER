#include "uart.h"
#include <string.h>

#define LINE_MAX 64 // 3 characters + '\0' terminator

static void run_command(const char *cmd)
{
	if (strcmp(cmd, "LED ON") == 0)
	{
		LED_On();
		UART2_SendString("LED is ON\r\n");
	}
	else if (strcmp(cmd, "LED OFF") == 0)
	{
		LED_Off();
		UART2_SendString("LED is OFF\r\n");
	}
	else if (strcmp(cmd, "STATUS") == 0)
	{
		UART2_SendString("dropped: ");
		UART2_SendUInt(UART2_Dropped());
		UART2_SendString("  overruns: ");
		UART2_SendUInt(UART2_Overruns());
		UART2_SendString("\r\n");
	}
	else if (strcmp(cmd, "HELP") == 0)
	{
		UART2_SendString("Commands: LED ON, LED OFF, STATUS, HELP\r\n");
	}
	else
	{
		UART2_SendString("unknown command\r\n");
	}
}

int main(void)
{
	char line[LINE_MAX];
	uint16_t len = 0; // Chars typed so far
	uint8_t c;

	LED_Init();
	UART2_Init();
	UART2_SendString("\r\nSTM32 CLI ready\r\n");

	while (1)
	{
		if (!UART2_ReadByte(&c)) // Check for byte
			continue;

		if (c == '\r') // Enter
		{
			UART2_SendString("\r\n");
			if (len > 0) // If empty, fresh prompt
			{
				line[len] = '\0'; // Terminator
				run_command(line);
				len = 0; // New line
			}
			UART2_SendString("> ");
		}
		else if (c == '\n')
		{
			// For terminals that send \r\n for enter
		}
		else if (c == 0x08 || c == 0x7F) // Backspace/Delete
		{
			if (len > 0)
			{
				len--;
				UART2_SendString("\b \b"); // Back, space over character, erase on screen
			}
		}
		else if (c >= 0x20 && c < 0x7F && len < LINE_MAX - 1)
		{
			line[len++] = (char)c;
			UART2_SendChar((char)c); // echo to see typed text
		}
	}
}
