/******************** (C) COPYRIGHT 2011 STMicroelectronics ********************
* File Name          : usb_prop.h
* Author             : MCD Application Team
* Version            : V3.3.0
* Date               : 21-March-2011
* Description        : All processing related to Mass Storage Demo (Endpoint 0)
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __usb_prop_H
#define __usb_prop_H
/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
#define Composite_Device_GetConfiguration          NOP_Process
/* #define Composite_Device_SetConfiguration          NOP_Process*/
#define Composite_Device_GetInterface              NOP_Process
#define Composite_Device_SetInterface              NOP_Process
#define Composite_Device_GetStatus                 NOP_Process
/* #define Composite_Device_ClearFeature              NOP_Process*/
#define Composite_Device_SetEndPointFeature        NOP_Process
#define Composite_Device_SetDeviceFeature          NOP_Process
/*#define Composite_Device_SetDeviceAddress          NOP_Process*/

#ifdef USE_MSD_DRIVE
/* MASS Storage Requests*/
#define GET_MAX_LUN                0xFE
#define MASS_STORAGE_RESET         0xFF
#define LUN_DATA_LENGTH            1
#endif /*USE_MSD_DRIVE*/

#define REPORT_DESCRIPTOR          0x22

/** 
 * @addtogroup Properties_Exported_Defines
 * @{
 */

#define BRIDGE_GetConfiguration          NOP_Process
/* #define BRIDGE_SetConfiguration          NOP_Process*/
#define BRIDGE_GetInterface              NOP_Process
#define BRIDGE_SetInterface              NOP_Process
#define BRIDGE_GetStatus                 NOP_Process
/* #define BRIDGE_ClearFeature              NOP_Process*/
#define BRIDGE_SetEndPointFeature        NOP_Process
#define BRIDGE_SetDeviceFeature          NOP_Process
/*#define BRIDGE_SetDeviceAddress          NOP_Process*/

#define CCID_INT_EP                      ENDP1
#define CCID_INT_EP_TX                   ENDP1_TXADDR
#define CCID_INT_EP_RX                   ENDP1_RXADDR
#define CCID_INT_EP_SIZE_TX              0x10	
#define CCID_INT_EP_SIZE_RX              0	/* --> not used (simple buffer) */

#define CCID_BULK_EP                     ENDP2
#define CCID_BULK_EP_TX                  ENDP2_TXADDR
#define CCID_BULK_EP_RX                  ENDP2_RXADDR
#define CCID_BULK_EP_SIZE_TX             BULK_PAY_LOAD /* --> TX size is defined by the CCID state machine */
#define CCID_BULK_EP_SIZE_RX             BULK_PAY_LOAD

#define HID_INT_EP                       ENDP3
#define HID_INT_EP_TX                    ENDP3_TXADDR
#define HID_INT_EP_RX                    ENDP3_RXADDR
#define HID_INT_EP_SIZE_TX               INTERRUPT_PAY_LOAD
#define HID_INT_EP_SIZE_RX               INTERRUPT_PAY_LOAD

#define REPORT_DESCRIPTOR                0x22




typedef enum _HID_REQUESTS
{
  GET_REPORT = 1,
  GET_IDLE,
  GET_PROTOCOL,

  SET_REPORT = 9,
  SET_IDLE,
  SET_PROTOCOL
} HID_REQUESTS;


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void COMPOSITE_init(void);
void COMPOSITE_Reset(void);
void Composite_Device_SetConfiguration(void);
void Composite_Device_ClearFeature(void);
void Composite_Device_SetDeviceAddress (void);
void COMPOSITE_Status_In (void);
void COMPOSITE_Status_Out (void);
uint8_t COMPOSITE_Data_Setup(uint8_t);
uint8_t COMPOSITE_NoData_Setup(uint8_t);
uint8_t COMPOSITE_Get_Interface_Setting(uint8_t Interface, uint8_t AlternateSetting);
uint8_t *COMPOSITE_GetDeviceDescriptor(uint16_t );
uint8_t *COMPOSITE_GetConfigDescriptor(uint16_t);
uint8_t *COMPOSITE_GetStringDescriptor(uint16_t);
uint8_t *Get_Max_Lun(uint16_t Length);
uint8_t *HID_Report                (uint16_t Length);

#endif /* __usb_prop_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

