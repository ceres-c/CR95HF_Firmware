/**
  ******************************************************************************
  * @file    main.h 
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
	
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f10x.h"
#include "stm32f10x_crc.h"

#include "lib_ConfigManager.h"
#include "drv_LED.h"

#include "usb_lib.h"

/*  status and erroc code------------------------------------------------------ */
#define MAIN_SUCCESS_CODE														RESULTOK

/* external constants --------------------------------------------------------*/
extern bool HID_TRANSACTION;	


/* Exported function --------------------------------------------------------*/
void Decrement_TimingDelay(void);


/* exported constants --------------------------------------------------------*/

#endif /* __MAIN_H */

