#ifndef LED_H
#define LED_H

#ifdef cplusplus
 extern "C" {
#endif

#include "gd32f30x.h"
     
		 
/* exported types */
typedef enum 
{
    LED1 = 0,
    LED2 = 1,
    LED3 = 2,
    LED4 = 3
} led_typedef_enum;


#define LEDn                             4U

#if 1
#define LED1_PIN                         GPIO_PIN_0
#define LED1_GPIO_PORT                   GPIOC
#define LED1_GPIO_CLK                    RCU_GPIOC
  
#define LED2_PIN                         GPIO_PIN_2
#define LED2_GPIO_PORT                   GPIOC
#define LED2_GPIO_CLK                    RCU_GPIOC
  
#define LED3_PIN                         GPIO_PIN_0
#define LED3_GPIO_PORT                   GPIOE
#define LED3_GPIO_CLK                    RCU_GPIOE
  
#define LED4_PIN                         GPIO_PIN_1
#define LED4_GPIO_PORT                   GPIOE
#define LED4_GPIO_CLK                    RCU_GPIOE

#else

#define LED1_PIN                         GPIO_PIN_3
#define LED1_GPIO_PORT                   GPIOB
#define LED1_GPIO_CLK                    RCU_GPIOB
  
#define LED2_PIN                         GPIO_PIN_4
#define LED2_GPIO_PORT                   GPIOB
#define LED2_GPIO_CLK                    RCU_GPIOB
  
#define LED3_PIN                         GPIO_PIN_5
#define LED3_GPIO_PORT                   GPIOB
#define LED3_GPIO_CLK                    RCU_GPIOB
  
#define LED4_PIN                         GPIO_PIN_6
#define LED4_GPIO_PORT                   GPIOB
#define LED4_GPIO_CLK                    RCU_GPIOB

#endif

/* configure led GPIO */
void GW_LedInit(led_typedef_enum lednum);
/* turn on selected led */
void GW_LedOn(led_typedef_enum lednum);
/* turn off selected led */
void GW_LedOff(led_typedef_enum lednum);
/* toggle the selected led */
void GW_LedToggle(led_typedef_enum lednum);


#ifdef cplusplus
}
#endif

#endif /* LED_H */
