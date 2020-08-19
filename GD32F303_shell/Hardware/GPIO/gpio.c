#include "gpio.h"

volatile uint8_t   irq_flag=0;

void GW_GpioInit(void)
{
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_GPIOB);
	
	rcu_periph_clock_enable(RCU_AF);
	nvic_irq_enable(SI4463_IRO_EXTI_IRQn, 0U, 5U);
	
	/*Init wifi rst pin*/	
    gpio_init(WIFI_RST_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, WIFI_RST_PIN);
    GPIO_BOP(WIFI_RST_PORT) = WIFI_RST_PIN;
	
	/*Init wifi run led*/
    gpio_init(WIFI_LED_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, WIFI_LED_PIN);
    GPIO_BOP(WIFI_LED_PORT) = WIFI_LED_PIN;
	
	/*Init SI4463 PDN PIN*/
    gpio_init(SI4463_PDN_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, SI4463_PDN_PIN);
    GPIO_BOP(SI4463_PDN_PORT) = SI4463_PDN_PIN;

	/*Init SI4463 CSN PIN*/
    gpio_init(SI4463_CSN_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, SI4463_CSN_PIN);
    GPIO_BOP(SI4463_CSN_PORT) = SI4463_CSN_PIN;
	
	/*Init SI4463 IRQ PIN*/
	gpio_init(SI4463_IRO_PORT, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, SI4463_IRO_PIN);
    gpio_exti_source_select(SI4463_IRO_PORT_SOURCE, SI4463_IRO_PIN_SOURCE);
	exti_init(SI4463_IRO_EXTI_LINE, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
	exti_interrupt_flag_clear(SI4463_IRO_EXTI_LINE);

}

void SI4463_IRO_IRQHandler(void)
{
	if(RESET != exti_interrupt_flag_get(SI4463_IRO_EXTI_LINE))
	{
		irq_flag =1;
		exti_interrupt_flag_clear(SI4463_IRO_EXTI_LINE);
	}

}




















