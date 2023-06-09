#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "main.h"
#include "stdio.h"
#include "usart.h"
#include "stdarg.h"
#include "string.h"

#define SIZE	50

void Debug(UART_HandleTypeDef huatx, const char *format, ...);


#endif
