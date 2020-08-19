/*!
 * File:
 *  radio_hal.c
 *
 * Description:
 *  This file contains RADIO HAL.
 *
 * Silicon Laboratories Confidential
 * Copyright 2011 Silicon Laboratories, Inc.
 */

#include "spi.h"
#include "gd32f30x.h"
#include "data_base.h"
//#include "stm32f10x_gpio.h"
//#include "task.h"
#include "systick.h"
#include "gpio.h"


#if GW_SPI1_INIT
	#define SI4463_SPI	SPI1
#endif
void radio_hal_AssertShutdown(void)
{
   SI4463_PDN_SET;
}

void radio_hal_DeassertShutdown(void)
{

  SI4463_PDN_RESET;
}

void radio_hal_ClearNsel(void)
{
   // RF_NSEL = 0;
   SI4463_CSN_RESET;
//	delay_ms(5);
}

void radio_hal_SetNsel(void)
{
   // RF_NSEL = 1;
   SI4463_CSN_SET;
}

BitStatus radio_hal_NirqLevel(void)
{
   // return RF_NIRQ;
  return (BitStatus)(SI4463_IRQ_IO_READ);
}

void radio_hal_SpiWriteByte(U8 byteToWrite)
{
  SpiReadWrite(SI4463_SPI,byteToWrite);
}

U8 radio_hal_SpiReadByte(void)
{
  return SpiReadWrite(SI4463_SPI,0xFF);
}

void radio_hal_SpiWriteData(U8 byteCount, U8* pData)
{
  GW_SpiWriteData(SI4463_SPI,byteCount, pData);
}

void radio_hal_SpiReadData(U8 byteCount, U8* pData)
{
	while(byteCount --)
		*pData++ = GW_SpiReadData(SI4463_SPI);
//  SpiReadData(byteCount, pData);
}

BitStatus radio_hal_Gpio0Level(void)
{
  BitStatus retVal;
 
  retVal=RESET;
  
  return retVal;
}

BitStatus radio_hal_Gpio1Level(void)
{
  BitStatus retVal;
  

  retVal=RESET;
    
  return retVal;
}

BitStatus radio_hal_Gpio2Level(void)
{
  BitStatus retVal;
  
  
  retVal=RESET;

  return retVal;
}

BitStatus radio_hal_Gpio3Level(void)
{
  BitStatus retVal;

  retVal=RESET;
  
  return retVal;
}
