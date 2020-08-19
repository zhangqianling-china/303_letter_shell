/**
  ******************************************************************************
  * @file    wifi.h
  * @author  
  * @version V1.0.0
  * @date    2019/10/5
  * @brief   wifiģ���װ
  ******************************************************************************
  * @attention
  * 
  * 
  ******************************************************************************
  */
#ifndef _wifi_H
#define _wifi_H

#include "gd32f30x.h"

#define WIFIAPSSIDLEN		30		//wifi ssid����
#define WIFIAPPASSWORDLEN	30		//wifi ���볤��
#define WIFISERVERIPLEN		20		//������IP����
#define WIFISERVERPORTLEN	10		//�������˿ںų���

#define WIFIBUFFLEN			100		//wifi ���ݻ���������


//wifiģ��Ĺ���״̬
typedef enum euWifiWorkMode 
{	
	WIFI_WORK_NULL,

	WIFI_CMD_AT,
	WIFI_CMD_ATE,
	WIFI_CMD_CWMODE,
	WIFI_CMD_CWJAP,
	WIFI_CMD_CIPSTART,
	WIFI_CMD_CIPSEND,
	WIFI_CMD_CIPMODE,
	WIFI_WORK_PASSTHROUGH,		
}WIFIWORKMODE;

typedef struct stWifiCmd 
{
	uint8_t send[WIFIBUFFLEN];

	uint32_t timer;	//ִ��ʱ��
	uint8_t	Num;	//0,��ִ�д�����0xFFһֱִ��

}WIFICMD;

typedef struct stWifiBuffer 
{
	uint8_t len;					//���ݳ���
	uint8_t data[WIFIBUFFLEN];	//����buffer
}WIFIBUFF;

void HAL_Wifi_SendCMD(uint8_t *pcmd, uint32_t waittime, uint8_t num);
void HAL_Reset_Wifi(void);
void HAL_Wifi_Init(uint8_t *pSSID, uint8_t *pPassWord, uint8_t *pServerIP, uint8_t *pServerPort, 
/*void(*pf_SendData)(uint8_t *pbuf uint8_t len),,*/
void(*pf_Passthough_data)(uint8_t *pbuf, uint8_t len));
void HAL_Wifi_Process(void);
void HAL_Wifi_SetRecData(uint8_t len, uint8_t* pData);
void HAL_Wifi_SendData(uint8_t* pData, uint8_t len);
void Wifi_SendCMD(uint8_t *pcmd, uint32_t waittime, uint8_t num);
void WIfi_Device_Reset(void);
void HAL_Wifi_SendProcess(void);

#endif
