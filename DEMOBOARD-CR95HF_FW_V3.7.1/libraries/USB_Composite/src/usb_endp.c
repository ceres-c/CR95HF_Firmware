/******************** (C) COPYRIGHT 2014 STMicroelectronics ********************
* File Name          : usb_endp.c
* Author             : MMY Application Team
* Version            : V3.3.1
* Date               : 12-June-2012
* Description        : Endpoint routines
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
#include "usb_bot.h"
#include "usb_istr.h"
#include "usb_hid.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/

/*******************************************************************************
* Function Name  : EP1_IN_Callback
* Description    : EP1 IN Callback Routine
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP1_IN_Callback(void)
{
#ifdef USE_MSD_DRIVE
  Mass_Storage_In();
#endif /*USE_MSD_DRIVE*/
	
	
}

/*******************************************************************************
* Function Name  : EP2_OUT_Callback.
* Description    : EP2 OUT Callback Routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP2_OUT_Callback(void)
{
#ifdef USE_MSD_DRIVE
  Mass_Storage_Out();
#endif /*USE_MSD_DRIVE*/

#ifdef USE_STRFNFCADEMO_BOARD	
	 CCID_Data_Out( );
#endif 
}


/*******************************************************************************
* Function Name  : EP2_OUT_Callback.
* Description    : EP2 OUT Callback Routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP2_IN_Callback(void)
{

#ifdef USE_STRFNFCADEMO_BOARD	
	 CCID_Data_In( );
#endif 
}


/*******************************************************************************
* Function Name  : EP3_IN_Callback
* Description    : EP3 IN Callback Routine
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP3_IN_Callback(void)
{
  HID_Data_In( );	 
}
/*******************************************************************************
* Function Name  : EP3_OUT_Callback.
* Description    : EP3 OUT Callback Routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP3_OUT_Callback(void)
{
  HID_Data_Out( );	
}

/******************* (C) COPYRIGHT 2014 STMicroelectronics *****END OF FILE****/

