/******************** (C) COPYRIGHT 2011 STMicroelectronics ********************
* File Name          : usb_prop.c
* Author             : MCD Application Team
* Version            : V3.3.0
* Date               : 21-March-2011
* Description        : All processing related to Mass Storage Demo
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "usb_bot.h"
#include "hw_config.h"
#include "memory.h"
#include "mass_mal.h"
#include "usb_prop.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

#ifdef USE_MSD_DRIVE
uint32_t Max_Lun = 0;
#endif /*USE_MSD_DRIVE*/

DEVICE Device_Table =
  {
    EP_NUM,
    1
  };

DEVICE_PROP Device_Property =
  {
    COMPOSITE_init,
    COMPOSITE_Reset,
    COMPOSITE_Status_In,
    COMPOSITE_Status_Out,
    COMPOSITE_Data_Setup,
    COMPOSITE_NoData_Setup,
    COMPOSITE_Get_Interface_Setting,
    COMPOSITE_GetDeviceDescriptor,
    COMPOSITE_GetConfigDescriptor,
    COMPOSITE_GetStringDescriptor,
    0,
    0x40 /*MAX PACKET SIZE*/
  };

USER_STANDARD_REQUESTS User_Standard_Requests =
  {
    Composite_Device_GetConfiguration,
    Composite_Device_SetConfiguration,
    Composite_Device_GetInterface,
    Composite_Device_SetInterface,
    Composite_Device_GetStatus,
    Composite_Device_ClearFeature,
    Composite_Device_SetEndPointFeature,
    Composite_Device_SetDeviceFeature,
    Composite_Device_SetDeviceAddress
  };

ONE_DESCRIPTOR Device_Descriptor =
  {
    (uint8_t*)COMPOSITE_DeviceDescriptor,
    COMPOSITE_SIZ_DEVICE_DESC
  };

/* the description of the configuration depends of the boards 
	 STRFNFCA supports HID & CCID
	 the others 	support HID & MSD
*/
#ifdef USE_STRFNFCADEMO_BOARD
ONE_DESCRIPTOR Config_Descriptor =
  {
    (uint8_t*)BRIDGE_ConfigDescriptor,
    BRIDGE_SIZ_CONFIG_DESC
  };	
	
#else
ONE_DESCRIPTOR Config_Descriptor =
  {
    (uint8_t*)COMPOSITE_ConfigDescriptor,
    COMPOSITE_SIZ_CONFIG_DESC
  };
#endif
	
ONE_DESCRIPTOR String_Descriptor[] =
  {
    {(uint8_t*)COMPOSITE_StringLangID, COMPOSITE_SIZ_STRING_LANGID},
    {(uint8_t*)COMPOSITE_StringVendor, COMPOSITE_SIZ_STRING_VENDOR},
    {(uint8_t*)COMPOSITE_StringProduct, COMPOSITE_SIZ_STRING_PRODUCT},
    {(uint8_t*)COMPOSITE_StringSerial, COMPOSITE_SIZ_STRING_SERIAL},
    /*{(uint8_t*)MASS_StringInterface, MASS_SIZ_STRING_INTERFACE},*/
  };

  
ONE_DESCRIPTOR HID_Report_Descriptor =
{
	(uint8_t *)HID_ReportDescriptor,
	HID_SIZ_REPORT_DESC
};

/* Extern variables ----------------------------------------------------------*/
#ifdef USE_MSD_DRIVE
extern unsigned char Bot_State;
extern Bulk_Only_CBW CBW;
#endif /*USE_MSD_DRIVE*/


/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/*******************************************************************************
* Function Name  : MASS_init
* Description    : Mass Storage init routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void COMPOSITE_init()
{
  /* Update the serial number string descriptor with the data from the unique
  ID*/
	#ifdef USE_MSD_DRIVE
  Get_SerialNum();
	#endif
  pInformation->Current_Configuration = 0;

  /* Connect the device */
  PowerOn();

  /* Perform basic device initialization operations */
  USB_SIL_Init();

  bDeviceState = UNCONNECTED;
}

/*******************************************************************************
* Function Name  : MASS_Reset
* Description    : Mass Storage reset routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void COMPOSITE_Reset()
{
  /* Set the device as not configured */
  Device_Info.Current_Configuration = 0;

  /* Current Feature initialization */
	#ifdef USE_STRFNFCADEMO_BOARD
			pInformation->Current_Feature = BRIDGE_ConfigDescriptor[7];
	#else 																  
			pInformation->Current_Feature = COMPOSITE_ConfigDescriptor[7];
	#endif 

  /* Set  DEVICE with the default Interface*/
  pInformation->Current_Interface = 0; 

  SetBTABLE(BTABLE_ADDRESS);

  /* Initialize Endpoint 0 */
  SetEPType(ENDP0, EP_CONTROL);
  SetEPTxStatus(ENDP0, EP_TX_NAK);
  SetEPRxAddr(ENDP0, ENDP0_RXADDR);
  SetEPRxCount(ENDP0, Device_Property.MaxPacketSize);
  SetEPTxAddr(ENDP0, ENDP0_TXADDR);
  Clear_Status_Out(ENDP0);
  SetEPRxValid(ENDP0);

/**************MASS STORAGE ENDPOINT*******************/
#ifdef USE_MSD_DRIVE
  /* Initialize Endpoint 1 */
  SetEPType(ENDP1, EP_BULK);
  SetEPTxAddr(ENDP1, ENDP1_TXADDR);
  SetEPTxStatus(ENDP1, EP_TX_NAK);
  SetEPRxStatus(ENDP1, EP_RX_DIS);

  /* Initialize Endpoint 2 */
  SetEPType(ENDP2, EP_BULK);
  SetEPRxAddr(ENDP2, ENDP2_RXADDR);
  SetEPRxCount(ENDP2, Device_Property.MaxPacketSize);
  SetEPRxStatus(ENDP2, EP_RX_VALID);
  SetEPTxStatus(ENDP2, EP_TX_DIS);
#endif /*USE_MSD_DRIVE*/

#ifdef USE_STRFNFCADEMO_BOARD

  /* Initialize Endpoint for Interrupt IN in simple buffer  -- CCID interface */
  SetEPType    (CCID_INT_EP, EP_INTERRUPT);    
  SetEPTxAddr  (CCID_INT_EP, CCID_INT_EP_TX);
	SetEPRxAddr  (CCID_INT_EP, CCID_INT_EP_RX);
  SetEPTxCount (CCID_INT_EP, CCID_INT_EP_SIZE_TX);
  SetEPTxStatus(CCID_INT_EP, EP_TX_NAK);
  SetEPRxStatus(CCID_INT_EP, EP_RX_DIS);

  /* Initialize Endpoint for Bulk IN and Bulk OUT in simple buffer -- CCID interface */  
  SetEPType    (CCID_BULK_EP, EP_BULK);    
  SetEPTxAddr  (CCID_BULK_EP, CCID_BULK_EP_TX);
  SetEPRxAddr  (CCID_BULK_EP, CCID_BULK_EP_RX);
  SetEPRxCount (CCID_BULK_EP, CCID_BULK_EP_SIZE_RX);
  SetEPTxStatus(CCID_BULK_EP, EP_TX_NAK);
  SetEPRxStatus(CCID_BULK_EP, EP_RX_VALID);


#endif


/**************HID ENDPOINT*******************/

  /* Initialize Endpoint 3 for HID */ 
  SetEPType(HID_INT_EP, EP_INTERRUPT);
  SetEPTxAddr(HID_INT_EP, HID_INT_EP_TX);
  SetEPRxAddr(HID_INT_EP, HID_INT_EP_RX);
  SetEPTxCount(HID_INT_EP, 0x40);
  SetEPRxCount(HID_INT_EP, 0x40);
  SetEPRxStatus(HID_INT_EP, EP_RX_VALID);
  SetEPTxStatus(HID_INT_EP, EP_TX_NAK);

  /* Set the device to response on default address */
  SetDeviceAddress(0);

  bDeviceState = ATTACHED;

#ifdef USE_MSD_DRIVE
  CBW.dSignature = BOT_CBW_SIGNATURE;
  Bot_State = BOT_IDLE;
#endif /*USE_MSD_DRIVE*/
}

/*******************************************************************************
* Function Name  : Mass_Storage_SetConfiguration
* Description    : Handle the SetConfiguration request.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Composite_Device_SetConfiguration(void)
{
  if (pInformation->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;

#ifdef USE_MSD_DRIVE  
    ClearDTOG_TX(ENDP1);
    ClearDTOG_RX(ENDP2);

    Bot_State = BOT_IDLE; /* set the Bot state machine to the IDLE state */
#endif /*USE_MSD_DRIVE*/
  }
}

/*******************************************************************************
* Function Name  : Mass_Storage_ClearFeature
* Description    : Handle the ClearFeature request.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Composite_Device_ClearFeature(void)
{
#ifdef USE_MSD_DRIVE
  /* when the host send a CBW with invalid signature or invalid length the two
     Endpoints (IN & OUT) shall stall until receiving a Mass Storage Reset     */
  if (CBW.dSignature != BOT_CBW_SIGNATURE)
    Bot_Abort(BOTH_DIR);
#endif /*USE_MSD_DRIVE*/
}

/*******************************************************************************
* Function Name  : Mass_Storage_SetConfiguration.
* Description    : Update the device state to addressed.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Composite_Device_SetDeviceAddress (void)
{
#ifdef USE_MSD_DRIVE
  bDeviceState = ADDRESSED;
#endif /*USE_MSD_DRIVE*/
}
/*******************************************************************************
* Function Name  : MASS_Status_In
* Description    : Mass Storage Status IN routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void COMPOSITE_Status_In(void)
{
  return;
}

/*******************************************************************************
* Function Name  : MASS_Status_Out
* Description    : Mass Storage Status OUT routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void COMPOSITE_Status_Out(void)
{
  return;
}

/*******************************************************************************
* Function Name  : MASS_Data_Setup.
* Description    : Handle the data class specific requests..
* Input          : RequestNo.
* Output         : None.
* Return         : RESULT.
*******************************************************************************/
RESULT COMPOSITE_Data_Setup(uint8_t RequestNo)
{
  uint8_t    *(*CopyRoutine)(uint16_t);

  CopyRoutine = NULL;

  if ((RequestNo == GET_DESCRIPTOR)
      && (Type_Recipient == (STANDARD_REQUEST | INTERFACE_RECIPIENT))
      && (pInformation->USBwIndex0 == 1))
  {	
    if (pInformation->USBwValue1 == REPORT_DESCRIPTOR)
    {
	  CopyRoutine = HID_Report;
    }
  }	
    
  if (CopyRoutine == NULL)
  {
    return USB_UNSUPPORT;
  }

  pInformation->Ctrl_Info.CopyData = CopyRoutine;
  pInformation->Ctrl_Info.Usb_wOffset = 0;
  (*CopyRoutine)(0);

  return USB_SUCCESS;

}

/*******************************************************************************
* Function Name  : MASS_NoData_Setup.
* Description    : Handle the no data class specific requests.
* Input          : RequestNo.
* Output         : None.
* Return         : RESULT.
*******************************************************************************/
RESULT COMPOSITE_NoData_Setup(uint8_t RequestNo)
{
#ifdef USE_MSD_DRIVE  
  if ((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
      && (RequestNo == MASS_STORAGE_RESET) && (pInformation->USBwValue == 0)
      && (pInformation->USBwIndex == 0) && (pInformation->USBwLength == 0x00))
  {

    /* Initialize Endpoint 1 */
    ClearDTOG_TX(ENDP1);

    /* Initialize Endpoint 2 */
    ClearDTOG_RX(ENDP2);

    /*initialize the CBW signature to enable the clear feature*/
    CBW.dSignature = BOT_CBW_SIGNATURE;
    Bot_State = BOT_IDLE;

    return USB_SUCCESS;
  }
#endif /*USE_MSD_DRIVE*/

  return USB_UNSUPPORT;
}

/*******************************************************************************
* Function Name  : MASS_Get_Interface_Setting
* Description    : Test the interface and the alternate setting according to the
*                  supported one.
* Input          : uint8_t Interface, uint8_t AlternateSetting.
* Output         : None.
* Return         : RESULT.
*******************************************************************************/
RESULT COMPOSITE_Get_Interface_Setting(uint8_t Interface, uint8_t AlternateSetting)
{
  if (AlternateSetting > 0)
  {
    return USB_UNSUPPORT;/* in this application we don't have AlternateSetting*/
  }
  else if (Interface > 1)
  {
    return USB_UNSUPPORT;/*in this application we have only 1 interfaces*/
  }
  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : MASS_GetDeviceDescriptor
* Description    : Get the device descriptor.
* Input          : uint16_t Length.
* Output         : None.
* Return         : None.
*******************************************************************************/
uint8_t *COMPOSITE_GetDeviceDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &Device_Descriptor );
}

/*******************************************************************************
* Function Name  : MASS_GetConfigDescriptor
* Description    : Get the configuration descriptor.
* Input          : uint16_t Length.
* Output         : None.
* Return         : None.
*******************************************************************************/
uint8_t *COMPOSITE_GetConfigDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &Config_Descriptor );
}

/*******************************************************************************
* Function Name  : MASS_GetStringDescriptor
* Description    : Get the string descriptors according to the needed index.
* Input          : uint16_t Length.
* Output         : None.
* Return         : None.
*******************************************************************************/
uint8_t *COMPOSITE_GetStringDescriptor(uint16_t Length)
{
  uint8_t wValue0 = pInformation->USBwValue0;

  if (wValue0 > 5)
  {
    return NULL;
  }
  else
  {
    return Standard_GetDescriptorData(Length, &String_Descriptor[wValue0]);
  }
}

#ifdef USE_MSD_DRIVE
/*******************************************************************************
* Function Name  : Get_Max_Lun
* Description    : Handle the Get Max Lun request.
* Input          : uint16_t Length.
* Output         : None.
* Return         : None.
*******************************************************************************/
uint8_t *Get_Max_Lun(uint16_t Length)
{
  if (Length == 0)
  {
    pInformation->Ctrl_Info.Usb_wLength = LUN_DATA_LENGTH;
    return 0;
  }
  else
  {
    return((uint8_t*)(&Max_Lun));
  }
}
#endif /*USE_MSD_DRIVE*/ 

/*******************************************************************************
* Function Name  : HID_Report
* Description    : Report the HID Descriptor.
* Input          : uint16_t Length.
* Output         : None.
* Return         : None.
*******************************************************************************/
uint8_t *HID_Report(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &HID_Report_Descriptor);
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
