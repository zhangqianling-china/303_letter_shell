/*! @file radio.c
 * @brief This file contains functions to interface with the radio chip.
 *
 * @b COPYRIGHT
 * @n Silicon Laboratories Confidential
 * @n Copyright 2012 Silicon Laboratories, Inc.
 * @n http://www.silabs.com
 */
#include "systick.h"
#include "data_base.h"
#include "radio_config.h"
#include "si446x_api_lib.h"
#include "radio_hal.h"
#include "radio.h"
#include "si446x_cmd.h"
 
U8 Radio_Configuration_Data_Array[]=RADIO_CONFIGURATION_DATA_ARRAY;
tRadioConfiguration RadioConfiguration=RADIO_CONFIGURATION_DATA;
tRadioConfiguration* pRadioConfiguration=&RadioConfiguration;


U8 customRadioPacket[RADIO_MAX_PACKET_LENGTH];

extern volatile uint8_t   irq_flag;
extern U8 Rx_Data_len;

void vRadio_PowerUp(void)
{
  U16  wDelay=0;
  
  /* Hardware reset the chip */
  si446x_reset();
  
  /* Wait until reset timeout or Reset IT signal */
  for (; wDelay < pRadioConfiguration->Radio_Delay_Cnt_After_Reset; wDelay++);
}

/*!
 *  Radio Initialization.
 *
 *  @author Sz. Papp
 *
 *  @note
 *
 */
void vRadio_Init(void)
{
  U16 wDelay;
  /* Power Up the radio chip */
  vRadio_PowerUp();
	
  si446x_part_info();//get  part information
  if(Si446xCmd.PART_INFO.PART!=0x4463)//part information check
  {
    while(1)
    {
      /* ERROR!!!!  SPI communicate error  or part information error */
      #ifdef RADIO_COMM_ERROR_CALLBACK
        RADIO_COMM_ERROR_CALLBACK();
      #endif
    }
  }

  /* Load radio configuration */
  while (SI446X_SUCCESS != si446x_configuration_init(pRadioConfiguration->Radio_ConfigurationArray))
  {
    for (wDelay = 0x7FFF; wDelay--; ) ;
    /* Power Up the radio chip */
    vRadio_PowerUp();
  }

  // Read ITs, clear pending ones
  si446x_get_int_status(0, 0, 0);
}

/*!
 *  Check if Packet sent IT flag or Packet Received IT is pending.
 *
 *  @return   SI4455_CMD_GET_INT_STATUS_REP_PACKET_SENT_PEND_BIT / SI4455_CMD_GET_INT_STATUS_REP_PACKET_RX_PEND_BIT
 *
 *  @note
 *
 */
U8 bRadio_Check_Tx_RX(void)
{
  //if(radio_hal_NirqLevel()==RESET)
  if(irq_flag)//²úÉúÖÐ¶Ï
  {
      irq_flag=0;
      
      /* Read ITs, clear pending ones */
      si446x_get_int_status(0, 0, 0);
      
      if (Si446xCmd.GET_INT_STATUS.CHIP_PEND & SI446X_CMD_GET_CHIP_STATUS_REP_CHIP_PEND_CMD_ERROR_PEND_BIT)//cmd EEROR
      {
      	/* State change to */
      	si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEXT_STATE1_NEW_STATE_ENUM_SLEEP);
	
	  	/* Reset FIFO */
      	si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_FIFO_RX_BIT);
        
	  	/* State change to */
        si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEXT_STATE1_NEW_STATE_ENUM_RX);
      }
      
      
      if(Si446xCmd.GET_INT_STATUS.PH_PEND & SI446X_CMD_GET_INT_STATUS_REP_PH_PEND_PACKET_SENT_PEND_BIT)//send finish
      {
        return SI446X_CMD_GET_INT_STATUS_REP_PH_PEND_PACKET_SENT_PEND_BIT;
      }
      
      
      if(Si446xCmd.GET_INT_STATUS.PH_PEND & SI446X_CMD_GET_INT_STATUS_REP_PH_PEND_PACKET_RX_PEND_BIT)//recive finish
      {
        /* Packet RX */
        
        /* Get payload length */
        si446x_fifo_info(0x00);
        
        si446x_read_rx_fifo(Si446xCmd.FIFO_INFO.RX_FIFO_COUNT, &customRadioPacket[0]);

        return SI446X_CMD_GET_INT_STATUS_REP_PH_PEND_PACKET_RX_PEND_BIT;
      }
      
     if(Si446xCmd.GET_INT_STATUS.PH_PEND & SI446X_CMD_GET_INT_STATUS_REP_PH_STATUS_CRC_ERROR_BIT)//crc error
      {
      	/* Reset FIFO */
      	si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_FIFO_RX_BIT);
      }
  }
  return 0;
}


/*!
 *  Set Radio to RX mode. .
 *
 *  @param channel Freq. Channel,  packetLength : 0 Packet handler fields are used , nonzero: only Field1 is used
 *
 *  @note
 *
 */
void vRadio_StartRX(U8 channel, U8 packetLenght )
{
  // Read ITs, clear pending ones
  si446x_get_int_status(0, 0, 0);

   // Reset the Rx Fifo
   si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_FIFO_RX_BIT);

  /* Start Receiving packet, channel 0, START immediately, Packet length used or not according to packetLength */
  si446x_start_rx(channel, 0, packetLenght,
                  SI446X_CMD_START_RX_ARG_NEXT_STATE1_RXTIMEOUT_STATE_ENUM_NOCHANGE,
                  SI446X_CMD_START_RX_ARG_NEXT_STATE2_RXVALID_STATE_ENUM_READY,
                  SI446X_CMD_START_RX_ARG_NEXT_STATE3_RXINVALID_STATE_ENUM_RX );
}

/*!
 *  Set Radio to TX mode, variable packet length.
 *
 *  @param channel Freq. Channel, Packet to be sent length of of the packet sent to TXFIFO
 *
 *  @note
 *
 */
void vRadio_StartTx_Variable_Packet(U8 channel, U8 *pioRadioPacket, U8 length)
{
  /* Leave RX state */
  si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEXT_STATE1_NEW_STATE_ENUM_READY);

  /* Read ITs, clear pending ones */
  si446x_get_int_status(0, 0, 0);

  /* Reset the Tx Fifo */
  si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_FIFO_TX_BIT);

  /* Fill the TX fifo with datas */
  si446x_write_tx_fifo(length, pioRadioPacket);

  /* Start sending packet, channel 0, START immediately */
   si446x_start_tx(channel, 0x80, length);
 
}


void vRadio_StartTx_CW(U8 channel, U8 *pioFixRadioPacket)
{
  // Read ITs, clear pending ones
  si446x_get_int_status(0, 0, 0);

  /* Start sending packet on channel, START immediately, Packet according to PH */
  si446x_start_tx(channel, 0, 0);
}


