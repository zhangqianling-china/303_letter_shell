#ifndef GPIO_H
#define GPIO_H

#ifdef cplusplus
 extern "C" {
#endif

#include "gd32f30x.h"
     
#define WIFI_RST_ON						GPIO_BC(WIFI_RST_PORT) = WIFI_RST_PIN
#define WIFI_RST_OFF					GPIO_BOP(WIFI_RST_PORT) = WIFI_RST_PIN
#define WIFI_LED_TOGGLE					GPIO_OCTL(WIFI_LED_PORT) ^= WIFI_LED_PIN

#define SI4463_PDN_SET					GPIO_BOP(SI4463_PDN_PORT) = SI4463_PDN_PIN
#define SI4463_PDN_RESET				GPIO_BC(SI4463_PDN_PORT) = SI4463_PDN_PIN

#define SI4463_CSN_SET					GPIO_BOP(SI4463_CSN_PORT) = SI4463_CSN_PIN
#define SI4463_CSN_RESET				GPIO_BC(SI4463_CSN_PORT) = SI4463_CSN_PIN

#define SI4463_IRQ_IO_READ				gpio_input_bit_get(SI4463_IRO_PORT,SI4463_IRO_PIN)

#define WIFI_RST_PIN                    GPIO_PIN_8
#define WIFI_RST_PORT                   GPIOA
#define WIFI_RST_CLK                    RCU_GPIOA

#define WIFI_LED_PIN                    GPIO_PIN_3
#define WIFI_LED_PORT                   GPIOB
#define WIFI_LED_CLK                    RCU_GPIOB

#define SI4463_PDN_PIN					GPIO_PIN_12
#define SI4463_PDN_PORT					GPIOA
#define SI4463_PDN_CLK					RCU_GPIOA

#define SI4463_CSN_PIN					GPIO_PIN_12
#define SI4463_CSN_PORT					GPIOB
#define SI4463_CSN_CLK					RCU_GPIOB

#define SI4463_IRO_PIN					GPIO_PIN_1
#define SI4463_IRO_PORT					GPIOA
#define SI4463_IRO_CLK					RCU_GPIOA
#define SI4463_IRO_EXTI_LINE            EXTI_1
#define SI4463_IRO_EXTI_IRQn            EXTI1_IRQn
#define SI4463_IRO_PORT_SOURCE      	GPIO_PORT_SOURCE_GPIOA
#define SI4463_IRO_PIN_SOURCE       	GPIO_PIN_SOURCE_1

#define SI4463_IRO_IRQHandler			EXTI1_IRQHandler

/* configure GPIO */
void GW_GpioInit(void);



#ifdef cplusplus
}
#endif

#endif /* GPIO_H */
