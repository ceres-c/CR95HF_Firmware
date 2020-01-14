/**
  ******************************************************************************
  * @file    drv_LED.c
  * @author  MMY Application Team
  * @version V1.1
  * @date    15/03/2011
  * @brief   This file provides a set of firmware functions to manage Leds   
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


  
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "drv_LED.h"

/** @addtogroup Drivers
 * 	@{
 */

/** @addtogroup LED
 * 	@{
 * @brief      This file includes the LED driver.
 */

extern GPIO_TypeDef* GPIO_PORT[LEDn] = {LED1_GPIO_PORT};
extern const uint32_t GPIO_CLK[LEDn] = {LED1_GPIO_CLK};		  
extern const uint16_t GPIO_PIN[LEDn] = {LED1_PIN};


/** @addtogroup public functions
 *  @{
 */

/**
  * @brief  this function configures LED GPIO.
  * @param  Led: Specifies the Led to be configured. 
  *   This parameter can be one of following parameters:
  *     @arg LED1
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4
  */
void LED_Config(Led_TypeDef Led)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* Enable the GPIO_LED Clock */
  RCC_APB2PeriphClockCmd(GPIO_CLK[Led], ENABLE);

  /* Configure the GPIO_LED pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN[Led];
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIO_PORT[Led], &GPIO_InitStructure);
}

/**
  * @brief  this function turns selected LED On.
  * @param  Led: Specifies the Led to be set on. 
  *   This parameter can be one of following parameters:
  *     @arg LED1
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4  
  */
void LED_On(Led_TypeDef Led)
{
  GPIO_PORT[Led]->BSRR = GPIO_PIN[Led];     
}

/**
  * @brief  this function turns selected LED Off.
  * @param  Led: Specifies the Led to be set off. 
  *   This parameter can be one of following parameters:
  *     @arg LED1
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4 
  */
void LED_Off(Led_TypeDef Led)
{
  GPIO_PORT[Led]->BRR = GPIO_PIN[Led];  
}

/**
  * @brief  this function turns all LEDs Off.
  */
void LED_AllOff(void)
{
	uint8_t i;
	for (i=LED1;i<=LED5;i++)
		GPIO_PORT[i]->BRR = GPIO_PIN[i];  
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

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
