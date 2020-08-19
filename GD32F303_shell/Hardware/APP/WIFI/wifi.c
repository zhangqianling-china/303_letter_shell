/**
  ******************************************************************************
  * @file    wifi.c
  * @author  HK_698
  * @version V1.0.0
  * @date    2019/10/5
  * @brief   
  ******************************************************************************
  * @attention
  *
  *
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/  
#include "systick.h"
#include "wifi.h"
#include "timer.h"
#include "usart.h"
//#include "task.h"
#include "string.h"
#include <stdio.h>

#include "systick.h"
#include "gpio.h"

/*静态变量----------------------------------------------------------------*/
static uint8_t Wifi_Buf[WIFIBUFFLEN];
static WIFIWORKMODE	WIFI_WORK_MODE;
static WIFICMD 	Wifi_Cmd;
static WIFIBUFF	Wifi_Passthough_Data;
static WIFIBUFF	Wifi_Send_Data[3];
static WIFIBUFF	Wifi_Recive_Data;
static uint8_t		Wifi_Error_Num;
static uint8_t		*p_Wifi_AP_SSID;
static uint8_t		*p_Wifi_AP_PASSWORD;
static uint8_t		*p_Wifi_SERVER_IP;
static uint8_t		*p_Wifi_SERVER_PORT;
static uint8_t		*p_Wifi_Heartbeat_Data;
static void		(*pf_Hal_Wifi_Send_Data)(uint8_t *pbuf, uint8_t len);
static void		(*pf_Hal_Wifi_Passthough_Data_Procss)(uint8_t *pData, uint8_t len);

uint8_t wifi_rx_len=0;
uint8_t wifi_rx_flag=0;

/**
  * @brief  进入到透传数据状态
  * @param  None
  * @retval None
  */
void Wifi_Cmd_PASSTHROUGH()
{
//	uint8_t i;
	if(Wifi_Passthough_Data.len != 0)
	{
		
		pf_Hal_Wifi_Send_Data(Wifi_Passthough_Data.data, Wifi_Passthough_Data.len);

		Wifi_Passthough_Data.len = 0;
		
		WIFI_WORK_MODE = WIFI_WORK_PASSTHROUGH;
	}
}

/**
  * @brief  设定透传数据数量
  * @param  None
  * @retval None
  */
void Wifi_Cmd_CIPSEND(void)
{
	sprintf((char*)Wifi_Buf, "AT+CIPSEND");
	
	Wifi_SendCMD(Wifi_Buf, 2000, 5);

	WIFI_WORK_MODE = WIFI_CMD_CIPSEND;
}

/**
  * @brief  设定透传数据数量
  * @param  None
  * @retval None
  */
void Wifi_Cmd_CIPMODE(void)
{	
	sprintf((char*)Wifi_Buf, "AT+CIPMODE=1");
	
	Wifi_SendCMD(Wifi_Buf, 2000, 5);

	WIFI_WORK_MODE = WIFI_CMD_CIPMODE;
}


/**
  * @brief  连接服务器
  * @param  None
  * @retval None
  */
void Wifi_Cmd_CIPSTART(void)
{	
	sprintf((char*)Wifi_Buf, "AT+CIPSTART=\"TCP\",\"%s\",%s", 
				p_Wifi_SERVER_IP, p_Wifi_SERVER_PORT);
	
	Wifi_SendCMD(Wifi_Buf, 2000, 5);	

	WIFI_WORK_MODE = WIFI_CMD_CIPSTART;
}

/**
  * @brief  连接AP
  * @param  None
  * @retval None
  */
void Wifi_Cmd_CWJAP(void)
{
	sprintf((char*)Wifi_Buf, "AT+CWJAP=\"%s\",\"%s\"", 
				p_Wifi_AP_SSID, p_Wifi_AP_PASSWORD);    

	Wifi_SendCMD(Wifi_Buf, 1000, 5);

	WIFI_WORK_MODE = WIFI_CMD_CWJAP;
}

/**
  * @brief  设定wifi模块的工作模式
  * @param  None
  * @retval None
  */
void Wifi_Cmd_CWMODE(void)
{	
	Wifi_SendCMD("AT+CWMODE=1", 50, 1);

	WIFI_WORK_MODE = WIFI_CMD_CWMODE;	
}
/**
  * @brief  关闭模块的回显功能
  * @param  None
  * @retval None
  */
void Wifi_Cmd_ATE0(void)
{
	Wifi_SendCMD("ATE0", 50, 1);
	
	WIFI_WORK_MODE = WIFI_CMD_ATE;
}

/**
  * @brief  模块连接测试指令
  * @param  None
  * @retval None
  */
void Wifi_Cmd_AT(void)
{
	Wifi_SendCMD("AT", 50, 5);
	
	WIFI_WORK_MODE = WIFI_CMD_AT;
}

/**
  * @brief  复位
  * @param  None
  * @retval None
  */
void Wifi_Cmd_RST(void)
{
//	sprintf((char*)Wifi_Buf, "AT+RST\n");
//	delay_ms(500);
//	delay_ms(500);
	wifi_rx_len = 0;
	wifi_rx_flag =0;
	memset(Wifi_Recive_Data.data, 0, WIFIBUFFLEN);
}


void Wifi_Send_Data_Pack(uint8_t Order,uint16_t len)
{
}

/**
   *@brief 
   *
   *@param 
   *@retval 
   */
void Led_flah_info(void)
{
   if(WIFI_WORK_MODE != WIFI_WORK_PASSTHROUGH)
	 {
		WIFI_LED_TOGGLE;
	   Set_Timer_Msg(TIMER_MSG_ZIGBEE_LED_FLASHED, 50, Led_flah_info);
	 }
}
/**
  * @brief  心跳处理函数;
  * @param  None
  * @retval None
  */
void Wifi_Heartbeat_Process(void)
{
	//--发送心跳
	Set_Timer_Msg(TIMER_MSG_HEARTBEAT_PACKET, 6000, Wifi_Heartbeat_Process);
}

/**
  * @brief  wifi 模块 AT指令初始化结束函数
  * @param  None
  * @retval None
  */
void Wifi_Cmd_End(void)
{
	DEBUG_PRINTF("WIFI init end");

	Clear_Timer_Msg(TIMER_MSG_WIFIREC);
	
	Wifi_Passthough_Data.len = 0;
	WIFI_WORK_MODE = WIFI_WORK_PASSTHROUGH;
  
	Set_Timer_Msg(TIMER_MSG_HEARTBEAT_PACKET, 500, Wifi_Heartbeat_Process);
}

/**
  * @brief  串口接收到数据后的处理流程
  * @param  None
  * @retval None
  */
  void HAL_Wifi_Process(void)
{
	char *strx=0;
	uint8_t len,i;

	if(Wifi_Recive_Data.len == 0)
	{
		return;		
	}	

	DEBUG_PRINTF("rec = %s", Wifi_Recive_Data.data);

	//TCP 断开连接
	strx = strstr((const char*)Wifi_Recive_Data.data, "CLOSED");		
	if(strx != NULL)
	{
		DEBUG_PRINTF("rec CLOSED");
		Clear_Timer_Msg(TIMER_MSG_WIFIREC);

		Wifi_Error_Num++;
		if(Wifi_Error_Num > 10)
		{
			goto _hal_wifi_process_fun_end;
		}

		delay_1ms(1000);
		Wifi_Cmd_CIPSTART();
		
		goto _hal_wifi_process_fun_end;
	}

	//TCP 连接错误
	strx = strstr((const char*)Wifi_Recive_Data.data, "link is not valid");		
	if(strx != NULL)
	{
		DEBUG_PRINTF("rec link error");
		Clear_Timer_Msg(TIMER_MSG_WIFIREC);

		Wifi_Error_Num++;
		if(Wifi_Error_Num > 10)
		{
			goto _hal_wifi_process_fun_end;
		}

		delay_1ms(1000);
		Wifi_Cmd_CIPSTART();

		Wifi_Error_Num++;

		goto _hal_wifi_process_fun_end;
	}

	//接收到服务器发来的数据
	if(WIFI_WORK_MODE == WIFI_WORK_PASSTHROUGH)
	{
		
		len = Wifi_Recive_Data.data[3]*256+Wifi_Recive_Data.data[4];		
		
		DEBUG_PRINTF("strlen = %d", len);
    for(i=0;i<len+10;i++)
		{
		  printf("0x%x\t",Wifi_Recive_Data.data[i]);
		}
		printf("\n");
		pf_Hal_Wifi_Passthough_Data_Procss(Wifi_Recive_Data.data, len+5);

		goto _hal_wifi_process_fun_end;
	}
	
	//透传发送数据
	if(WIFI_WORK_MODE == WIFI_CMD_CIPSEND)
	{
		strx = strstr((const char*)Wifi_Recive_Data.data, ">");
		if(strx != NULL)
		{
			DEBUG_PRINTF("PASSTHROUGH");
			Clear_Timer_Msg(TIMER_MSG_WIFIREC);
			Wifi_Cmd_End();
			
			//Wifi_Cmd_PASSTHROUGH();						
		}			

		goto _hal_wifi_process_fun_end;
	}

	if(WIFI_WORK_MODE == WIFI_CMD_AT)
	{
		strx = strstr((const char*)Wifi_Recive_Data.data, "OK");
		if(strx != NULL)
		{
			DEBUG_PRINTF("WIFI_CMD_AT");			
			
			Clear_Timer_Msg(TIMER_MSG_WIFIREC);
			Wifi_Cmd_ATE0();
		}
		goto _hal_wifi_process_fun_end;
	}
		
	if(WIFI_WORK_MODE == WIFI_CMD_ATE)
	{
		strx = strstr((const char*)Wifi_Recive_Data.data, "OK");
		if(strx != NULL)
		{
			DEBUG_PRINTF("WIFI_CMD_ATE");
			
			Clear_Timer_Msg(TIMER_MSG_WIFIREC);
			Wifi_Cmd_CWMODE();
		}

		goto _hal_wifi_process_fun_end;
	}

	if(WIFI_WORK_MODE == WIFI_CMD_CWMODE)
	{
		strx = strstr((const char*)Wifi_Recive_Data.data, "OK");
		if(strx != NULL)
		{
			DEBUG_PRINTF("WIFI_CMD_CWMODE");
			
			Clear_Timer_Msg(TIMER_MSG_WIFIREC);
			Wifi_Cmd_CIPMODE();
		}

		goto _hal_wifi_process_fun_end;
	}

	if(WIFI_WORK_MODE == WIFI_CMD_CIPSTART)
	{
		strx = strstr((const char*)Wifi_Recive_Data.data, "OK");
		if(strx != NULL)
		{
			DEBUG_PRINTF("WIFI_CMD_CIPSTART");
			
			Clear_Timer_Msg(TIMER_MSG_WIFIREC);
			Wifi_Cmd_CIPSEND();
			
		}

		goto _hal_wifi_process_fun_end;
	}

	if(WIFI_WORK_MODE == WIFI_CMD_CIPMODE)
	{
		strx = strstr((const char*)Wifi_Recive_Data.data, "OK");
		if(strx != NULL)
		{
			DEBUG_PRINTF("WIFI_CMD_CIPMODE");
		
			Clear_Timer_Msg(TIMER_MSG_WIFIREC);
			Wifi_Cmd_CWJAP();
		}

		goto _hal_wifi_process_fun_end;
	}
	
	if(WIFI_WORK_MODE == WIFI_CMD_CWJAP)
	{
		strx = strstr((const char*)Wifi_Recive_Data.data, "OK");
		if(strx != NULL)
		{
			DEBUG_PRINTF("WIFI_CMD_CWJAP");
		
			Clear_Timer_Msg(TIMER_MSG_WIFIREC);
			Wifi_Cmd_CIPSTART();
		}

		goto _hal_wifi_process_fun_end;
	}

_hal_wifi_process_fun_end:	

	if(Wifi_Error_Num > 10)
	{
		DEBUG_PRINTF("Wifi_Error_Num: %d", Wifi_Error_Num);

		WIFI_WORK_MODE = WIFI_WORK_NULL;
		Wifi_Error_Num = 0;
        
		Clear_Timer_Msg(TIMER_MSG_WIFIREC);
		
		Set_Timer_Msg(TIMER_MSG_OVERTIME_RESET, 6000, HAL_Reset_Wifi);

	}

	Wifi_Recive_Data.len = 0;
	memset(Wifi_Recive_Data.data, 0, WIFIBUFFLEN);
}

/**
  * @brief  wifi发送数据流程
  * @param  None
  * @retval None
  */
void HAL_Wifi_SendProcess(void)
{
	uint8_t i;
	
	if((WIFI_WORK_MODE == WIFI_WORK_PASSTHROUGH) &&	(Wifi_Passthough_Data.len == 0))
	{
		for (i=0; i<3;i++)
		{
			if(Wifi_Send_Data[i].len != 0)
			{
				Wifi_Passthough_Data.len = Wifi_Send_Data[i].len;
				memcpy(Wifi_Passthough_Data.data, Wifi_Send_Data[i].data, Wifi_Send_Data[i].len);

				Wifi_Cmd_PASSTHROUGH();

				Wifi_Send_Data[i].len = 0;

				return;
			}
		}
	}
}


/**
  * @brief  wifi 发送AT指令后，超时处理函数
  *			该函数将判断AT指令的重发次数，超过重发次数，将重启wifi模块
  * @param  None
  * @retval None
  */
void Wifi_Wait_Rec_Timer_CallBack(void)
{
	DEBUG_PRINTF("%s overtime", Wifi_Cmd.send);

	//超过重发次数
	if(Wifi_Cmd.Num == 0x00)
	{
		DEBUG_PRINTF_ERROR("%s ERROR", Wifi_Cmd.send);

		Clear_Timer_Msg(TIMER_MSG_WIFI_LED_FLASHED);

		Set_Timer_Msg(TIMER_MSG_OVERTIME_RESET, 6000, HAL_Reset_Wifi);
		
		return;
	}

	//重复当前wifi指令
	sprintf((char*)Wifi_Buf, "%s\r\n", Wifi_Cmd.send);
	pf_Hal_Wifi_Send_Data(Wifi_Buf, strlen((const char*)Wifi_Buf)); //发送命令

	Set_Timer_Msg(TIMER_MSG_WIFIREC, Wifi_Cmd.timer, Wifi_Wait_Rec_Timer_CallBack);

	//如果重发设置为0xFF，则无限次重发
	if(Wifi_Cmd.Num != 0xFF )
	{
		Wifi_Cmd.Num--;
	}
}

/**
  * @brief  wifi模块硬件重启
  * @param  None
  * @retval None
  */
void WIfi_Device_Reset(void)
{
	//复位WIFI模块
	WIFI_RST_ON;
	delay_1ms(10);
	WIFI_RST_OFF;
}

/**
  * @brief  AT指令发送
  *			如果AT指令发送发送后，在等待时间内，未收到回应，将会重发该AT指令
  * @param  pcmd：指令内容
  *			waittime：模块响应等待时间
  *			num：重发次数。
  * @retval None
  */
void Wifi_SendCMD(uint8_t *pcmd, uint32_t waittime, uint8_t num)
{	
	DEBUG_PRINTF("cmd=%s", pcmd);
	
	strcpy((char*)(Wifi_Cmd.send), (const char*)pcmd);
	Wifi_Cmd.timer = waittime;
	Wifi_Cmd.Num = num;

	sprintf((char*)Wifi_Buf, "%s\r\n", Wifi_Cmd.send);
	pf_Hal_Wifi_Send_Data(Wifi_Buf, strlen((const char*)Wifi_Buf)); //发送命令

	Set_Timer_Msg(TIMER_MSG_WIFIREC, Wifi_Cmd.timer, Wifi_Wait_Rec_Timer_CallBack);
} 

/**
  * @brief  wifi模块初始化
  * @param  pSSID：wifi AP ssid
  *			pPassWord：wifi AP 密码
  *			pServerIP：服务器IP地址
  *			pServerPort：服务器端口号
  *			pHeartData：心跳数据
  *			pf_SendData：发送数据函数
  *			pf_Passthough_data：接收数据处理函数  
  * @retval None
  */
void HAL_Wifi_Init(uint8_t *pSSID, uint8_t *pPassWord, uint8_t *pServerIP, uint8_t *pServerPort, 
					  /*void(*pf_SendData)(u8 *pbuf, u8 len),*/
						void(*pf_Passthough_data)(uint8_t *pbuf, uint8_t len))
{

	DEBUG_PRINTF("WIFI init start");	

	//WIFI 基本数据初始化	
	p_Wifi_AP_SSID 						= pSSID;
	p_Wifi_AP_PASSWORD					= pPassWord;
	p_Wifi_SERVER_IP					= pServerIP;
	p_Wifi_SERVER_PORT					= pServerPort;
//	p_Wifi_Heartbeat_Data				= pHeartData;
//	pf_Hal_Wifi_Send_Data				= pf_SendData;
	pf_Hal_Wifi_Passthough_Data_Procss 	= pf_Passthough_data;
	Set_Timer_Msg(TIMER_MSG_ZIGBEE_LED_FLASHED, 50, Led_flah_info);  //闪灯
	//WIFI模块初始化
	HAL_Reset_Wifi();
}

/**
  * @brief  wifi模块重启
  * @param  None
  * @retval None
  */
void HAL_Reset_Wifi(void)
{	
	WIfi_Device_Reset();	
	wifi_rx_len = 0;
	wifi_rx_flag =0;
	memset(Wifi_Recive_Data.data, 0, WIFIBUFFLEN);
	Wifi_Cmd_AT();
}

/**
  * @brief  wifi模块接收数据
  * @param  len：接收数据的长度
  *			pData：数据
  * @retval None
  */
void HAL_Wifi_SetRecData(uint8_t len, uint8_t* pData)
{

	Wifi_Recive_Data.len = len;

	memset(Wifi_Recive_Data.data, 0, WIFIBUFFLEN);

	memcpy(Wifi_Recive_Data.data, pData, len);
}

/**
  * @brief  wifi 发送数据
  * @param  None
  * @retval None
  */
void HAL_Wifi_SendData(uint8_t* pData, uint8_t len)
{
	uint8_t i;
	
	for (i=0; i<3;i++)
	{
		if(Wifi_Send_Data[i].len == 0)
		{
			Wifi_Send_Data[i].len = len;
			memcpy(Wifi_Send_Data[i].data, pData, len);

			return;
		}
	}

	DEBUG_PRINTF("WIFI busy");
}




