#include "timer.h"
//#include "led.h"
#include <stdio.h>

/*静态变量----------------------------------------------------------------*/
static uint32_t SYSTEM_REFERENCE_TIMER;
static SYSTIMERMSG	Sys_Timer_Msg[TIMERMSGNUM];

/**
  * @brief  定时器初始化
  *			定义10ms的基准定时器
  * @param  None
  * @retval None
  */
void GW_TimerInit(void)
{
    timer_parameter_struct timer_initpara;
	
	nvic_irq_enable(GW_TIMER_IRQ, 0,1);
	
    rcu_periph_clock_enable(GW_TIMER_CLK);
    timer_deinit(GW_TIMER);
	
    timer_initpara.prescaler         = 119;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 9999;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(GW_TIMER,&timer_initpara);

    timer_update_event_enable(GW_TIMER);
    timer_interrupt_enable(GW_TIMER,TIMER_INT_UP);
    timer_flag_clear(GW_TIMER, TIMER_FLAG_UP);
    timer_update_source_config(GW_TIMER, TIMER_UPDATE_SRC_GLOBAL);
	
    timer_enable(GW_TIMER);
		
		SYSTEM_REFERENCE_TIMER = 0;
}
/**
  * @brief  10ms定时器中断处理函数
  * @param  None
  * @retval None
  */
void GW_TIMER_IRQHandler(void)
{
    if(SET == timer_interrupt_flag_get(GW_TIMER, TIMER_FLAG_UP)){
        /* clear channel 0 interrupt bit */
        timer_interrupt_flag_clear(GW_TIMER, TIMER_FLAG_UP);
			
				SYSTEM_REFERENCE_TIMER++;
        /* toggle selected led */
//        GW_LedToggle(LED2);
    }
}
/**
  * @brief  添加一个定时器
  * @param  msg：定时器ID
  *			timer：时间长度。10ms*timer=实际时间(ms  )
  *			pCB：到达时间后的，回调处理函数
  * @retval None
  */
void Set_Timer_Msg(TIMERMSGSTYLE msg,  uint32_t timer, void (*pCB)())
{
	Sys_Timer_Msg[msg].flag = timer + SYSTEM_REFERENCE_TIMER;
	Sys_Timer_Msg[msg].pCb = pCB;
}
/**
  * @brief  关闭一个定时器
  * @param  msg：定时器ID
  * @retval None
  */
void Clear_Timer_Msg(TIMERMSGSTYLE msg)
{
	Sys_Timer_Msg[msg].flag = 0;
}
/**
  * @brief  定时器处理函数
  * @param  None
  * @retval None
  */
void Timer_Msg(void)
{
	uint8_t i;
	
	for(i=0; i<TIMERMSGNUM; i++)
	{

		if(Sys_Timer_Msg[i].flag != 0)
		{
			if(SYSTEM_REFERENCE_TIMER > Sys_Timer_Msg[i].flag)
			{
				Sys_Timer_Msg[i].flag = 0;
				
				if(Sys_Timer_Msg[i].pCb != NULL)				
				{
					Sys_Timer_Msg[i].pCb();
				}
			}
		}
	}
}


