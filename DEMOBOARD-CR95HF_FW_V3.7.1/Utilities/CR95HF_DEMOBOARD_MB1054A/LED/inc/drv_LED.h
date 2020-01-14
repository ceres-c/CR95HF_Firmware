/**
  ******************************************************************************
  * @file    drv_LED.h
  * @author  MMY Application Team
  * @version V1.1
  * @date    15/03/2011
  * @brief   This file provides
  *            - set of firmware functions to manage Led available on
  *           MB1054A CR95HF demonstration board from STMicroelectronics.    
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
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  */ 


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRIVER_LED_H
#define __DRIVER_LED_H

#ifdef __cplusplus
 extern "C" {
#endif


#define LEDn                             1

#define LED1_PIN                         GPIO_Pin_5
#define LED1_GPIO_PORT                   GPIOB
#define LED1_GPIO_CLK                    RCC_APB2Periph_GPIOB  
  

typedef enum 
{
  LED1 = 0,
	LED2,
	LED3,
	LED4,
	LED5
} Led_TypeDef;


void LED_Config (Led_TypeDef Led);
void LED_On			(Led_TypeDef Led);
void LED_Off		(Led_TypeDef Led);
void LED_AllOff (void);

#ifdef __cplusplus
}
#endif

#endif /* __LCD_LIB_H */


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
  
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
