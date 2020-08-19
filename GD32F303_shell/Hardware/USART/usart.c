#include "usart.h"
#include <stdio.h>

static rcu_periph_enum COM_CLK[COMn] = {GW_COM1_CLK, GW_COM2_CLK, GW_COM3_CLK};
static uint32_t COM_TX_PIN[COMn] = {GW_COM1_TX_PIN, GW_COM2_TX_PIN, GW_COM3_TX_PIN};
static uint32_t COM_RX_PIN[COMn] = {GW_COM1_RX_PIN, GW_COM2_RX_PIN, GW_COM3_RX_PIN};
static uint32_t COM_GPIO_PORT[COMn] = {GW_COM1_GPIO_PORT, GW_COM2_GPIO_PORT, GW_COM3_GPIO_PORT};
static rcu_periph_enum COM_GPIO_CLK[COMn] = {GW_COM1_GPIO_CLK, GW_COM2_GPIO_CLK, GW_COM3_GPIO_CLK};


/*!
    \brief      configure COM port
    \param[in]  com: COM on the board
      \arg        EVAL_COM1: COM1 on the board
      \arg        EVAL_COM2: COM2 on the board
    \param[out] none
    \retval     none
*/
void GW_ComInit(uint32_t com)
{
    uint32_t com_id = 0U;
    if(GW_COM1 == com){
        com_id = 0U;
				nvic_irq_enable(USART0_IRQn, 0, 2);
    }else if(GW_COM2 == com){
        com_id = 1U;
				nvic_irq_enable(USART1_IRQn, 0, 3);
    }else if(GW_COM3 == com){
				com_id = 2U;
		}
    
    /* enable GPIO clock */
    rcu_periph_clock_enable(COM_GPIO_CLK[com_id]);

    /* enable USART clock */
    rcu_periph_clock_enable(COM_CLK[com_id]);

    /* connect port to USARTx_Tx */
    gpio_init(COM_GPIO_PORT[com_id], GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, COM_TX_PIN[com_id]);

    /* connect port to USARTx_Rx */
    gpio_init(COM_GPIO_PORT[com_id], GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, COM_RX_PIN[com_id]);

    /* USART configure */
    usart_deinit(com);
    usart_baudrate_set(com, 115200U);
    usart_receive_config(com, USART_RECEIVE_ENABLE);
    usart_transmit_config(com, USART_TRANSMIT_ENABLE);
		if(GW_COM3 != com){	
				usart_interrupt_enable(com, USART_INT_RBNE);
				usart_interrupt_enable(com, USART_INT_IDLE);
		}
    usart_enable(com);
}
/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(GW_COM3, (uint8_t)ch);
    while(RESET == usart_flag_get(GW_COM3, USART_FLAG_TBE));
    return ch;
}
uint8_t buf[20] = {0};
uint8_t i=0;
void USART0_IRQHandler(void)
{
	
    if(RESET != usart_interrupt_flag_get(GW_COM1, USART_INT_FLAG_RBNE)){

				buf[i++] = usart_data_receive(GW_COM1);
				
    }
    if(RESET != usart_interrupt_flag_get(GW_COM1, USART_INT_FLAG_IDLE)){
				usart_data_receive(GW_COM1);
				i=0;

    }
}
void USART1_IRQHandler(void)
{
    if(RESET != usart_interrupt_flag_get(GW_COM2, USART_INT_FLAG_RBNE)){

			buf[i++] = usart_data_receive(GW_COM2);
    }
    if(RESET != usart_interrupt_flag_get(GW_COM2, USART_INT_FLAG_IDLE)){
				usart_data_receive(GW_COM2);
				i=0;
    }
}

