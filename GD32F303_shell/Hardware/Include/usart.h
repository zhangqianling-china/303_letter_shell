#ifndef USART_H
#define USART_H

#ifdef cplusplus
 extern "C" {
#endif

#include "gd32f30x.h"


#define COMn                             3U

#define GW_COM1                        USART0
#define GW_COM1_CLK                    RCU_USART0
#define GW_COM1_TX_PIN                 GPIO_PIN_9
#define GW_COM1_RX_PIN                 GPIO_PIN_10
#define GW_COM1_GPIO_PORT              GPIOA
#define GW_COM1_GPIO_CLK               RCU_GPIOA

#define GW_COM2                        USART1
#define GW_COM2_CLK                    RCU_USART1
#define GW_COM2_TX_PIN                 GPIO_PIN_2
#define GW_COM2_RX_PIN                 GPIO_PIN_3
#define GW_COM2_GPIO_PORT              GPIOA
#define GW_COM2_GPIO_CLK               RCU_GPIOA

#define GW_COM3                        USART2
#define GW_COM3_CLK                    RCU_USART2
#define GW_COM3_TX_PIN                 GPIO_PIN_10
#define GW_COM3_RX_PIN                 GPIO_PIN_11
#define GW_COM3_GPIO_PORT              GPIOB
#define GW_COM3_GPIO_CLK               RCU_GPIOB

//DEBUGµ÷ÊÔ¿ª¹Ø
#define __DEBUG
#ifdef __DEBUG
#define DEBUG_PRINTF(format,...) printf("[%s]"format"\r\n", __func__, ##__VA_ARGS__)
#define DEBUG_PRINTF_ERROR(format,...) printf("FILE: "__FILE__", LINE: %d:"format"\r\n", __LINE__, ##__VA_ARGS__)
#else
#define DEBUG_PRINTF(format,...)
#define DEBUG_PRINTF_ERROR(format,...)
#endif


void GW_ComInit(uint32_t com);

#ifdef cplusplus
}
#endif

#endif /* USART_H */
