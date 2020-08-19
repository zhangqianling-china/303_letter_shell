#ifndef USART_SHELL_H
#define USART_SHELL_H

#ifdef cplusplus
 extern "C" {
#endif

#include "gd32f30x.h"
     
#define USART0_RX_BUFF_SIZE 64

void usart0_shell(void);
void USART0_SendData(const uint8_t* const pData, uint32_t len);

#ifdef cplusplus
}
#endif

#endif /* USART_SHELL_H */
