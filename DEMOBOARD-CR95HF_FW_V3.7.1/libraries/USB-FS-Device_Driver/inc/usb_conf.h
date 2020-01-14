/******************** (C) COPYRIGHT 2011 STMicroelectronics ********************
* File Name          : usb_conf.h
* Author             : MCD Application Team
* Version            : V3.3.0
* Date               : 21-March-2011
* Description        : Mass Storage Demo configuration header
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_CONF_H
#define __USB_CONF_H

/*-------------------------------------------------------------*/
/* EP_NUM */
/* defines how many endpoints are used by the device */
/*-------------------------------------------------------------*/
#if defined (USE_MSD_DRIVE) || defined (USE_STRFNFCADEMO_BOARD) 
	#define EP_NUM                          (11)
#else 
	#define EP_NUM                          (5)
#endif 
/*-------------------------------------------------------------*/
/* --------------   Buffer Description Table  -----------------*/
/*-------------------------------------------------------------*/
/* buffer table base address */

#define BTABLE_ADDRESS      		(0x00)

#define  BULK_PAY_LOAD	         64        /* Size of Bulk PayLoad */
#define  INTERRUPT_PAY_LOAD	     64	       /* Size of Interrupt PayLoad */
#define  BULK_BUFFER_QUEUE_SIZE  1024      /* Size of the Ring Buffer  Queue*/

#define MAX_PACKET_SIZE					0x40

/* EP0  */
/* Tx/rx buffer base address */
#define ENDP0_TXADDR     			  (BTABLE_ADDRESS + MAX_PACKET_SIZE)
#define ENDP0_RXADDR     			  (ENDP0_TXADDR   + MAX_PACKET_SIZE)


#ifdef USE_STRFNFCADEMO_BOARD
	/* EP1  */
	/* Tx/Rx buffer base address */
	#define ENDP1_TXADDR    		   (ENDP0_RXADDR + CCID_INT_EP_SIZE_TX)
	#define ENDP1_RXADDR      		 (ENDP1_TXADDR + CCID_INT_EP_SIZE_RX)

	/* EP2  */
	/* Tx/Rx buffer base address */
	#define ENDP2_TXADDR     		  (ENDP1_RXADDR + CCID_BULK_EP_SIZE_TX)
	#define ENDP2_RXADDR      		(ENDP2_TXADDR + CCID_BULK_EP_SIZE_RX)
#else  

	/* EP1  */
	/* Tx/Rx buffer base address */
	#define ENDP1_TXADDR    			   (ENDP0_RXADDR + MAX_PACKET_SIZE)
	#define ENDP1_RXADDR     		 		 (ENDP1_TXADDR + MAX_PACKET_SIZE)

	/* EP2  */
	/* Tx/Rx buffer base address */
	#define ENDP2_TXADDR       			(ENDP1_RXADDR + MAX_PACKET_SIZE)
	#define ENDP2_RXADDR       			(ENDP1_TXADDR + MAX_PACKET_SIZE)
#endif


/* EP3  */
/* Tx/Rx buffer base address */
#define ENDP3_TXADDR	   			  (ENDP2_RXADDR + HID_INT_EP_SIZE_TX)
#define ENDP3_RXADDR	   			  (ENDP3_TXADDR + HID_INT_EP_SIZE_RX)

/* ISTR events */
/* IMR_MSK */
/* mask defining which events has to be handled */
/* by the device application software */
#define IMR_MSK (CNTR_CTRM  | CNTR_RESETM)

/* CTR service routines */
/* associated to defined endpoints */
//#define  EP1_IN_Callback   NOP_Process
//#define  EP2_IN_Callback   NOP_Process
//#define  EP3_IN_Callback   NOP_Process
#define  EP4_IN_Callback   NOP_Process
#define  EP5_IN_Callback   NOP_Process
#define  EP6_IN_Callback   NOP_Process
#define  EP7_IN_Callback   NOP_Process


#define  EP1_OUT_Callback   NOP_Process
//#define  EP2_OUT_Callback   NOP_Process
//#define  EP3_OUT_Callback  NOP_Process
#define  EP4_OUT_Callback   NOP_Process
#define  EP5_OUT_Callback   NOP_Process
#define  EP6_OUT_Callback   NOP_Process
#define  EP7_OUT_Callback   NOP_Process


#endif /* __USB_CONF_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
