/**
  ******************************************************************************
  * @file    main.c 
  * @author  MMY Application Team
  * @version V1.0.0
  * @date    03/21/2013
  * @brief   Main program body
  ******************************************************************************
  * @copyright
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup User_Appli
 * 	@{
 *  @brief      <b>This folder contains the application files</b> 
 */
 
/** @addtogroup Main
 * 	@{
 * @brief      This file contains the entry point of this demonstration firmeware
 */


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

static __IO uint32_t TimingDelay = 0;

/* Variables for the different modes */
extern DeviceMode_t devicemode;
extern TagType_t nfc_tagtype;

/**
* @brief  buffer to exchange data with the RF tranceiver.
*/
extern uint8_t				u95HFBuffer [RFTRANS_95HF_MAX_BUFFER_SIZE+3];

__IO uint32_t SELStatus = 0;

bool 		USB_Control_Allowed = false;

extern int8_t	HIDTransaction;
/* bmp saved in well defined area, initialize pointer on this bmp file */
/* adress is forced in header file */
  
 /* Private function prototypes -----------------------------------------------*/
 
 
/** @addtogroup Main_Private_Functions
 * 	@{
 */

/**
  * @}
  */

/** @addtogroup Main_Public_Functions
 * 	@{
 */

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  uint8_t TagType = TRACK_NOTHING, tagfounds=TRACK_ALL;
	
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x000);
	
	/* -- Configures Main system clocks & power */
	Set_System();
	
	/* Enable CRC periph used by application to compute CRC after download */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
	  
/*------------------- Resources Initialization -----------------------------*/
	
	/* configure the interuptions  */
	Interrupts_Config();

	/* configure the timers  */
	Timer_Config( );
	
	/* Configure systick in order to generate one tick every ms */
	/* also used to generate pseudo random numbers (SysTick->VAL) */
	SysTick_Config(SystemCoreClock / 1000);
	
/*------------------- Drivers Initialization -------------------------------*/

  /* Led Configuration */
  LED_Config(LED1);	

	/* activate the USB clock */
	Set_USBClock();
	delay_ms(10);
	/* initialize the USB  */
	USB_Init();
	/* but prevent PC to control CR95HF */
	USB_Cable_Config(DISABLE);
	USB_Control_Allowed = false;
		
	/* ST95HF HW Init */
  ConfigManager_HWInit();
  
  LED_On(LED1);
	delay_ms(400);
	LED_Off(LED1);
	
	/* allow PC to control CR95HF */
	USB_Cable_Config(ENABLE);
	/* allow PC to control CR95HF */
	USB_Control_Allowed = true;
	
	while (1)
  {
    
    if (HIDTransaction == false )
		{
      TagType = ConfigManager_TagHunting(tagfounds);
      // Turn on the LED if a tag was founded
      if ( TagType == TRACK_NFCTYPE1 ||
          TagType == TRACK_NFCTYPE2 ||
          TagType == TRACK_NFCTYPE3 ||
          TagType == TRACK_NFCTYPE4A || 
          TagType == TRACK_NFCTYPE4B ||
          TagType == TRACK_NFCTYPE5)
        LED_On(LED1);
      else 
        LED_Off(LED1);
    }
    
    delay_ms(100);
  }

}


/**
  * @brief  Decrements the TimingDelay variable. 
  * @param  None
  * @retval None
	*/
void Decrement_TimingDelay(void)
{
  if (TimingDelay != 0x00)
  {
    TimingDelay--;
  }
}



/**
  * @}
  */ 

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
