#include "Debug.h"

void Debug(UART_HandleTypeDef huatx, const char *format, ...)
{
	va_list args;
	uint32_t length;
	uint8_t tx_buff[SIZE] = {0};
	
	va_start(args, format);
	length = vsnprintf((char *)tx_buff, sizeof(tx_buff), (char *)format, args);
	va_end(args);
	HAL_UART_Transmit(&huatx, (uint8_t *)tx_buff, length, 100);
	memset(tx_buff, 0, SIZE);	
}
