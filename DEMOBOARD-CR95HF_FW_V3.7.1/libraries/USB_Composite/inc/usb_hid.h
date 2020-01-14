/**
  ******************************************************************************
  * @file    usb_hid.c 
  * @author  SMD/MMY Application Team
  * @version V1.3.2
  * @date    02/09/2015
  * @brief   Handle messages received through USB HID
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 


/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef __HID_APPLI_H
#define __HID_APPLI_H

/* Includes ----------------------------------------------------------------- */
#include "stm32f10x.h"
#include "string.h"
#include "stdbool.h"

#include "usb_lib.h"

#include "hw_config.h"

				
/* ID definition ------------------------------------------------------------ */
#define HID_SPI_SEND_CR95HFCMD											0x01
#define HID_SPI_SEND_DATA														0x02

#define HID_SEND_HID_RESPONSE												0x07

				
/* Offset definitions for global buffers ------------------------------------  */
#define HID_OFFSET																	0x00
#define HID_OFFSET_CMDCODE													HID_OFFSET + 1
#define HID_OFFSET_LENGTH														HID_OFFSET + 2
#define HID_OFFSET_CTRLCODE													HID_OFFSET + 3
/* Max buffer size for HID communication */
#define HID_MAX_BUFFER_SIZE													0x40


/* success and error code ---------------------------------------------------- */
#define	HID_SUCCESSCODE															RESULTOK
#define	HID_SUCCESS_CODE_0X80												0x80
#define	HID_ERRORCODE_DEFAULT												0xFE
#define	HID_ERRORCODE_TIMEOUT												0xFD
#define	HID_ERRORCODE_UARTDATARATEUNCHANGED					0xFC
#define	HID_ERRORCODE_UARTDATARATEPROCESS						0xFB
#define	HID_ERRORCODE_FILENOTFOUND									0xFA
#define	HID_ERRORCODE_READALLMEMORY									0xF9
#define	HID_ERRORCODE_TAGDETECTINGCALIBRATION				0xF8
#define	HID_ERRORCODE_CUSTOMCOMMANDUNKNOWN					0xF7
#define	HID_ERRORCODE_TAGDETECTING									0xF5
#define	HID_ERRORCODE_NOTAGFOUND										0xF4
#define HID_ERRORCODE_PARAMETERLENGTH								0x41
#define HID_ERRORCODE_PARAMETER											0x42
#define HID_ERRORCODE_COMMANDUNKNOWN								0x43
#define HID_ERRORCODE_PORERROR											0x44



#define HID_ERRORCODE_OUTOFMEMORY										0xEF
#define HID_WRITE_MEMEORY_IN_MSD_FILE								0xDD

/* RF transceiver version ----------------------------------------------------------------- */
#define MCUVERSION_STATUS																0x00
#define MCUVERSION_LENGTH																0x03

/* MCU FW version ------------------------------------------------------------------------- */
#define MCUVERSION_FIRSTNUMBER													3	
#define MCUVERSION_SECONDNUMBER													7
#define MCUVERSION_THIRDNUMBER													1
#define MCUVERSION_STRING																"  V3.7.1 Internal   "

/* Wrapper for RX95HF and CR95HF */

#ifdef RX95HF

#define XX95HF_COMMAND_OFFSET						PICC_COMMAND_OFFSET	
#define XX95HF_RESETSEQUENCE						PICC_RESETSEQUENCE
#define XX95HF_PULSE_IRQIN 							PICC_PULSE_IRQIN
#define XX95HF_PULSE_SPINSS 						PICC_PULSE_SPINSS
#define XX95HF_PULSE_POOLINGREADING			PICC_PULSE_POOLINGREADING
#define XX95HF_GETMCUVERSION 						PICC_GETMCUVERSION
#define XX95HF_GETHARDWAREVERSION 			PICC_GETHARDWAREVERSION
#define XX95HF_POR 											PICC_POR
#define XX95HF_GETINTERFACEPINSTATE 		PICC_GETINTERFACEPINSTATE
#define XX95HF_INVENTORY16SLOTS 				PICC_INVENTORY16SLOTS
#define XX95HF_15693ANTICOL 						PICC_15693ANTICOL
#define XX95HF_MULTITAG 								PICC_MULTITAG
#define XX95HF_MULTITAG_EXTENDED 				PICC_MULTITAG_EXTENDED
#define XX95HF_TAG_TRAKING 							PICC_TAG_TRAKING
#define XX95HF_GOTOTAGDETECTINGSTATE 		PICC_GOTOTAGDETECTINGSTATE
#define XX95HF_ISWAKEUP 								PICC_ISWAKEUP
#define XX95HF_CALIBRATETHETAGDETECTION PICC_CALIBRATETHETAGDETECTION

#ifdef USE_MSD_DRIVE
#define XX95HF_SETUSBDISCONNECTSTATE 		PCD_SETUSBDISCONNECTSTATE
#define XX95HF_COPYTAGTOFILE 						PCD_COPYTAGTOFILE
#define XX95HF_COPYFILETOTAG 						PCD_COPYFILETOTAG
#endif

#else

#define XX95HF_COMMAND_OFFSET						PCD_COMMAND_OFFSET
#define XX95HF_RESETSEQUENCE						PCD_RESETSEQUENCE
#define XX95HF_PULSE_IRQIN 							PCD_PULSE_IRQIN
#define XX95HF_PULSE_SPINSS 						PCD_PULSE_SPINSS
#define XX95HF_PULSE_POOLINGREADING			PCD_PULSE_POOLINGREADING
#define XX95HF_GETMCUVERSION 						PCD_GETMCUVERSION
#define XX95HF_GETHARDWAREVERSION 			PCD_GETHARDWAREVERSION
#define XX95HF_POR 											PCD_POR
#define XX95HF_GETINTERFACEPINSTATE 		PCD_GETINTERFACEPINSTATE
#define XX95HF_INVENTORY16SLOTS 				PCD_INVENTORY16SLOTS
#define XX95HF_15693ANTICOL 						PCD_15693ANTICOL
#define XX95HF_MULTITAG 								PCD_MULTITAG
#define XX95HF_MULTITAG_EXTENDED 				PCD_MULTITAG_EXTENDED
#define XX95HF_TAG_TRAKING 							PCD_TAG_TRAKING
#define XX95HF_GOTOTAGDETECTINGSTATE 		PCD_GOTOTAGDETECTINGSTATE
#define XX95HF_ISWAKEUP 								PCD_ISWAKEUP
#define XX95HF_CALIBRATETHETAGDETECTION PCD_CALIBRATETHETAGDETECTION
#define XX95HF_CONFIGTYPEBPRIME         PCD_CONFIGTYPEBPRIME
#define XX95HF_CHAINEDCOMMAND           PCD_CHAINEDCOMMAND
#define XX95HF_CHAINEDRESPONSE          PCD_CHAINEDRESPONSE
#define XX95HF_UPGRADERECEIVEBUFFER     PCD_UPGRADERECEIVEBUFFER

#ifdef USE_MSD_DRIVE
#define XX95HF_SETUSBDISCONNECTSTATE 		PCD_SETUSBDISCONNECTSTATE
#define XX95HF_COPYTAGTOFILE 						PCD_COPYTAGTOFILE
#define XX95HF_COPYFILETOTAG 						PCD_COPYFILETOTAG
#endif
#endif

/* external variables--------------------------------------------------------- */

/* exportable functions --------------------------------------------------------- */
void HID_Data_Out	( void );
void HID_Data_In		( void );

#endif /* __HID_APPLI_H */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
