#include "usart_shell.h"
#include <stdio.h>
#include <string.h>
#include "shell_port.h"

extern Shell shell;

uint16_t receive_len=0;
char Usart0_Rx_Buf[USART0_RX_BUFF_SIZE]={0};
#define USART0_DATA_ADDRESS    ((uint32_t)&USART_DATA(USART0))

void usart0_shell(void)
{
	nvic_irq_enable(USART0_IRQn, 0, 0);
	/* enable GPIO clock */
	rcu_periph_clock_enable(RCU_GPIOA);

	/* enable USART clock */
	rcu_periph_clock_enable(RCU_USART0);

	/* connect port to USARTx_Tx */
	gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_9);

	/* connect port to USARTx_Rx */
	gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

	/* USART configure */
	usart_deinit(USART0);
	usart_baudrate_set(USART0, 115200U);
	usart_receive_config(USART0, USART_RECEIVE_ENABLE);
	usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);

	usart_interrupt_enable(USART0, USART_INT_RBNE);

	usart_enable(USART0);
}
void USART0_IRQHandler(void)
{
	char ch=0;
	
    if(usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE) != RESET)
    {
			ch = usart_data_receive(USART0);
			shellHandler(&shell,ch);
    }
	
}
#if 0
void usart0_shell(void)
{
	dma_parameter_struct dma_init_struct;
	
	 nvic_irq_enable(USART0_IRQn, 0, 0);
   /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);

    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART0);
	rcu_periph_clock_enable(RCU_DMA0);

    /* connect port to USARTx_Tx */
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_9);

    /* connect port to USARTx_Rx */
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

    /* USART configure */
    usart_deinit(USART0);
    usart_baudrate_set(USART0, 115200U);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    
	
	usart_flag_clear(USART0,USART_FLAG_TC);
	usart_interrupt_disable(USART0,USART_INT_TC);
	usart_interrupt_disable(USART0, USART_INT_RBNE);

	usart_interrupt_enable(USART0,USART_INT_IDLE);
	
	{
        /* deinitialize DMA channel4 (USART0 rx) */
        dma_deinit(DMA0, DMA_CH4);
        dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
        dma_init_struct.memory_addr = (uint32_t)Usart0_Rx_Buf;
        dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
        dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
        dma_init_struct.number = USART0_RX_BUFF_SIZE;
        dma_init_struct.periph_addr = USART0_DATA_ADDRESS;
        dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
        dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
        dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
        dma_init(DMA0, DMA_CH4, dma_init_struct);
        /* configure DMA mode */
        dma_circulation_disable(DMA0, DMA_CH4);        
        /* enable DMA channel4 */
        dma_channel_enable(DMA0, DMA_CH4);	
	}
	usart_dma_receive_config(USART0, USART_DENR_ENABLE);
	usart_enable(USART0);
}
void USART0_IRQHandler(void)
{
    

    if(usart_interrupt_flag_get(USART0, USART_INT_FLAG_IDLE) != RESET)
    {
        dma_channel_disable(DMA0, DMA_CH4);
		USART_STAT0(USART0);
		USART_DATA(USART0);												                //读SR后读DR 清USART_IT_IDLE标志
        dma_flag_clear(DMA0, DMA_CH4,DMA_FLAG_FTF);
		dma_flag_clear(DMA0, DMA_CH4,DMA_FLAG_ERR);
		receive_len=USART0_RX_BUFF_SIZE - dma_transfer_number_get(DMA0, DMA_CH4);
		dma_transfer_number_config(DMA0, DMA_CH4,USART0_RX_BUFF_SIZE);
		dma_channel_enable(DMA0, DMA_CH4);
    }
	
}
#endif
int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART0, (uint8_t)ch);
    while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
    return ch;
}
/**
  * @brief  串口0发送数据
  * @param  pData 数据指针
  * @param  len 数据长度
  */
void USART0_SendData(char *pData, uint32_t len)
{
    int i;
    for( i = 0; i<len ; i++)
    {
		usart_data_transmit(USART0,pData[i]);
		while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
    }
}




