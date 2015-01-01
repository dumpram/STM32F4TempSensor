#ifndef _USART_H_
#define _USART_H_

#include <stm32f4xx_usart.h>

#define BLUETOOTH_TERMINAL (USART_TypeDef*) USART1

void init_USART1(int baudrate);
void USART_puts(USART_TypeDef* USARTx, char* bytes);


#endif
