/******************** (C) COPYRIGHT 2011 STMicroelectronics ********************
* File Name          : usb_desc.h
* Author             : MCD Application Team
* Version            : V3.3.0
* Date               : 21-March-2011
* Description        : Descriptor Header for Mass Storage Device
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_DESC_H
#define __USB_DESC_H

/* Includes ------------------------------------------------------------------*/

 #include "stm32f10x.h"
 
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define COMPOSITE_SIZ_DEVICE_DESC              		18

#ifdef USE_MSD_DRIVE
	#define COMPOSITE_SIZ_CONFIG_DESC              		64 /*MSD + HID*/
#else
	#define COMPOSITE_SIZ_CONFIG_DESC              		41 /*HID*/
#endif /*USE_MSD_DRIVE*/

#define BRIDGE_SIZ_DEVICE_DESC                  	18
#define BRIDGE_SIZ_CONFIG_DESC                   	125

#define BRIDGE_SIZ_STRING_LANGID                	4
#define BRIDGE_SIZ_STRING_VENDOR                	38
#define BRIDGE_SIZ_STRING_PRODUCT               	18
#define BRIDGE_SIZ_STRING_SERIAL                	26
//#define BRIDGE_SIZ_STRING_PRODUCT          			 	14

#define COMPOSITE_SIZ_STRING_LANGID       	     	4
#define COMPOSITE_SIZ_STRING_VENDOR       	     	38
#define COMPOSITE_SIZ_STRING_PRODUCT     	     	 	38
#define COMPOSITE_SIZ_STRING_SERIAL       	     	26
#define COMPOSITE_SIZ_STRING_INTERFACE    	     	16

#define HID_DESCRIPTOR_TYPE                  		  0x21
#define CUSTOMHID_SIZ_HID_DESC              	    0x09
#define CUSTOMHID_OFF_HID_DESC              	    0x12
#define CUSTOMHID_SIZ_REPORT_DESC           	    52

#define USB_DEVICE_DESCRIPTOR_TYPE								0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE					0x02
#define USB_STRING_DESCRIPTOR_TYPE								0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE      		    0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE        	    0x05


#define HID_DESCRIPTOR_TYPE                    	 	0x21
#define HID_SIZ_REPORT_DESC                     	52

/* Exported functions ------------------------------------------------------- */
extern const uint8_t COMPOSITE_DeviceDescriptor[COMPOSITE_SIZ_DEVICE_DESC];
extern const uint8_t COMPOSITE_ConfigDescriptor[COMPOSITE_SIZ_CONFIG_DESC];

extern const uint8_t COMPOSITE_StringLangID[COMPOSITE_SIZ_STRING_LANGID];
extern const uint8_t COMPOSITE_StringVendor[COMPOSITE_SIZ_STRING_VENDOR];
extern const uint8_t COMPOSITE_StringProduct[COMPOSITE_SIZ_STRING_PRODUCT];
extern uint8_t COMPOSITE_StringSerial[COMPOSITE_SIZ_STRING_SERIAL];
extern const uint8_t COMPOSITE_StringInterface[COMPOSITE_SIZ_STRING_INTERFACE];
extern const uint8_t HID_ReportDescriptor[CUSTOMHID_SIZ_REPORT_DESC];

extern const uint8_t BRIDGE_DeviceDescriptor[BRIDGE_SIZ_DEVICE_DESC];
extern const uint8_t BRIDGE_ConfigDescriptor[BRIDGE_SIZ_CONFIG_DESC];
extern const uint8_t BRIDGE_StringLangID[BRIDGE_SIZ_STRING_LANGID];
extern const uint8_t BRIDGE_StringVendor[BRIDGE_SIZ_STRING_VENDOR];
extern const uint8_t BRIDGE_StringProduct[BRIDGE_SIZ_STRING_PRODUCT];



#endif /* __USB_DESC_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/


