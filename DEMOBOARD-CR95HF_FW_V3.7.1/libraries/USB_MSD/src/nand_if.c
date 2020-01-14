/******************** (C) COPYRIGHT 2011 STMicroelectronics ********************
* File Name          : nand_if.c
* Author             : MCD Application Team
* Version            : V3.3.0
* Date               : 21-March-2011
* Description        : manage NAND operations state machine
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
#ifdef USE_MSD_DRIVE

#include "stm32f10x.h"

#ifdef STM32F10X_HD
uint16_t LUT[1024]; //DON'T REMOVE
#else
uint16_t LUT[256]; //DON'T REMOVE
#endif /*STM32F10X_HD*/  
/*******************************************************************************
* Function Name  : NAND_Build
* Description    : Build Nothing (BUT DON'T REMOVE...)
* Input          : None
* Output         : None
* Return         : None

*******************************************************************************/
void NAND_Build (void)
{
    if (LUT[0])
    {
	  LUT[0]=0;
    }
	return;
}

#endif /*USE_MSD_DRIVE*/

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
