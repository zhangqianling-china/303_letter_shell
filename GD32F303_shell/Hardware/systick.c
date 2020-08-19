/*!
    \file  systick.c
    \brief the systick configuration file
*/

/*
    Copyright (C) 2017 GigaDevice

    2017-06-23, V1.0.0, demo for GD32F30x
*/

#include "gd32f30x.h"
#include "systick.h"


#define DELAY_US_FACTOR     120

void delay_init(void)
{
    systick_clksource_set(SYSTICK_CLKSOURCE_HCLK);
}


//xus < 139810
void delay_xus(uint32_t xus)
{
    uint32_t xtick = xus * DELAY_US_FACTOR;
    if(xus > 155000)
    {
        return;
    }
    SysTick->LOAD = xtick - 1;
    SysTick->VAL  = 0;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    while((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0x0);
}

//xms < 139
void delay_xms(uint32_t xms)
{
    uint32_t xtick = xms * DELAY_US_FACTOR * 1000;
    if(xms > 155)
    {
        return;
    }
    SysTick->LOAD = xtick - 1;
    SysTick->VAL  = 0;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    while((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0x0);
}


#if 0
volatile static uint32_t delay;

/*!
    \brief      configure systick
    \param[in]  none
    \param[out] none
    \retval     none
*/
void systick_config(void)
{
    /* setup systick timer for 1000Hz interrupts */
    if (SysTick_Config(SystemCoreClock / 1000U)){
        /* capture error */
        while (1){
        }
    }
    /* configure the systick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x00U);
}

/*!
    \brief      delay a time in milliseconds
    \param[in]  count: count in milliseconds
    \param[out] none
    \retval     none
*/
void delay_1ms(uint32_t count)
{
    delay = count;

    while(0U != delay){
    }
}

/*!
    \brief      delay decrement
    \param[in]  none
    \param[out] none
    \retval     none
*/
void delay_decrement(void)
{
    if (0U != delay){
        delay--;
    }
}
#endif
