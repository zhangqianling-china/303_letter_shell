/*! @file radio.h
 * @brief This file contains the public functions controlling the radio chip.
 *
 * @b COPYRIGHT
 * @n Silicon Laboratories Confidential
 * @n Copyright 2012 Silicon Laboratories, Inc.
 * @n http://www.silabs.com
 */

#ifndef RADIO_H_
#define RADIO_H_

#include "data_base.h"
#include "gd32f30x.h"
/*****************************************************************************
 *  Global Macros & Definitions
 *****************************************************************************/
 extern  U8 fixRadioPacket[];
 extern  U8 customRadioPacket[];

/*! Maximal packet length definition (FIFO size) */

#define RADIO_MAX_PACKET_LENGTH     64
   
/*****************************************************************************
 *  Global Typedefs & Enums
 *****************************************************************************/
typedef struct
{
    U8   *Radio_ConfigurationArray;

    U8   Radio_ChannelNumber;
    U8   Radio_PacketLength;
    U8   Radio_State_After_Power_Up;

    U16  Radio_Delay_Cnt_After_Reset;

    U8   Radio_CustomPayload[RADIO_MAX_PACKET_LENGTH];
} tRadioConfiguration;


typedef enum
{
    RFLR_STATE_IDLE,
    RFLR_STATE_RX_INIT,
    RFLR_STATE_RX_RUNNING,
    RFLR_STATE_RX_DONE,
    
    RFLR_STATE_RX_ACK_INIT,
    RFLR_STATE_RX_ACK_RUNNING,
    RFLR_STATE_RX_ACK_DONE,
    RFLR_STATE_RX_TIMEOUT,
   
    RFLR_STATE_TX_INIT,
    RFLR_STATE_TX_RUNNING,
    RFLR_STATE_TX_DONE,
    RFLR_STATE_TX_TIMEOUT,
    
    RFLR_STATE_TX_ACK_INIT,
    RFLR_STATE_TX_ACK_RUNNING,
    RFLR_STATE_TX_ACK_DONE,
    RFLR_STATE_ACK_EORRER,
    RFLR_STATE_SLEEP,
}tRFLRStates;



extern tRadioConfiguration* pRadioConfiguration;
extern tRadioConfiguration RadioConfiguration;
extern  U8 Radio_Configuration_Data_Array[];


void  vRadio_Init(void);
void  vRadio_StartTx(U8, U8 *);
void vRadio_StartTx_CW(U8 channel, U8 *pioFixRadioPacket);   
U8    bRadio_Check_Ezconfig(U16);

bool gRadio_CheckTransmitted(void);

U8 bRadio_Check_Tx_RX(void);
void vRadio_StartRX(U8 channel, U8 packetLenght );
void vRadio_StartTx_Variable_Packet(U8 channel, U8 *pioRadioPacket, U8 length);


#endif /* RADIO_H_ */
