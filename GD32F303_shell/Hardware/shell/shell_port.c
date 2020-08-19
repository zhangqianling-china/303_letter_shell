/**
 * @file shell_port.c
 * @author Letter (NevermindZZT@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-02-22
 * 
 * @copyright (c) 2019 Letter
 * 
 */

#include "shell.h"
#include "usart_shell.h"
#include "gd32f30x.h"
#include <string.h>
#include "shell_port.h"
Shell shell;
char shellBuffer[512];

/**
 * @brief 用户shell写
 * 
 * @param data 数据
 */
void userShellWrite(char data)
{
	  usart_data_transmit(USART0, (uint8_t)data);
    while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
//    serialTransmit(&debugSerial, (uint8_t *)&data, 1, 0xFF);
}


/**
 * @brief 用户shell读
 * 
 * @param data 数据
 * @return char 状态
 */
signed char userShellRead(char *data)
{
	
//	if(receive_len!=0)
//	{
//		data=&Usart0_Rx_Buf[0];
//		receive_len=0;
//		memset(Usart0_Rx_Buf,0,sizeof(Usart0_Rx_Buf));
//		return 0;
//	}
//	else{
//		return -1;
//	}
}


/**
 * @brief 用户shell初始化
 * 
 */
void userShellInit(void)
{
    shell.write = userShellWrite;
//    shell.read = userShellRead;
    shellInit(&shell, shellBuffer, 512);
}

