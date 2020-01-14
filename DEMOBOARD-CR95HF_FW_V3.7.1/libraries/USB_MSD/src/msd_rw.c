/**
  ******************************************************************************
  * @file    msd_rw.c 
  * @author  MMY Application Team
  * @version V2.0
  * @date    12/06/2012
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

/* Includes ------------------------------------------------------------------*/
#ifdef USE_MSD_DRIVE
#include "msd_rw.h"


/**
* @brief  Read a file in the Mass Storage Device
* @param  *File_Data : return the data read in the file
* @param  *Length : return the length of the data read
* @param  *File_Name : Define the name of the file to read
* @retval MSD_SUCCESS_CODE : the function is succesful
* @retval PCD_ERROR_DEFAULT : an error occured
*/	
int8_t MSD_ReadFile(uint8_t *File_Data, uint16_t *Length, const char *File_Name)
{
uint16_t Size_Read;

	f_mount(0, &ff);
	/*Read the file if is present on the MSD, else return error statement*/
	res = f_open(&fdft, File_Name, FA_OPEN_EXISTING | FA_READ);

	if(res != FR_OK)
	{
		return MSD_ERROR_DEFAULT;
	}
	
	Size_Read = f_size(&fdft);

	*Length = Size_Read;

	res = f_read(&fdft, File_Data, Size_Read, &br);

	f_close(&fdft);
	f_mount(0, NULL);

	return MSD_SUCCESS_CODE;
}

/**
* @brief  Write a file in the Mass Storage Device
* @param  *File_Data : Data to write in the file
* @param  *Length : Length of the data to write
* @param  *File_Name : Define the name of the file to write
* @retval MSD_SUCCESS_CODE : the function is succesful
* @retval PCD_ERROR_DEFAULT : an error occured
*/		
int8_t MSD_WriteFile(uint8_t *File_Data, uint16_t Length, const char *File_Name)
{

  f_mount(0, &ff);
	/*Create a new file if the file name is not present, else overwrite the file*/ 
	res = f_open(&fdft, File_Name, FA_OPEN_ALWAYS | FA_WRITE);

	  	if(res != FR_OK)
			return MSD_ERROR_DEFAULT;
			
	res = f_write(&fdft, File_Data ,Length , &bw);

			if(res != FR_OK)
			return MSD_ERROR_DEFAULT;
			
	 f_close(&fdft);
	 f_mount(0, NULL);

return MSD_SUCCESS_CODE;
}	


/**
* @brief  Reboot the USB Connection
* @param  none
* @retval MSD_SUCCESS_CODE ; the function is succesful
*/	
int8_t MSD_RebootUSB ( void )
{
		/*Use the PIN USB Disconnect to stop the data exchange in the USB Bus*/
		GPIO_SetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
		delayHighPriority_ms(1000);
		GPIO_ResetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
		delayHighPriority_ms(1000);		

		return MSD_SUCCESS_CODE;
}	




/**
* @brief  Read the TAG Memory and write the data in the file "READ_TAG.txt"
* @brief 	C1: Command name
* @brief 	04: Length Data
* @brief 	XX: MSB Nb Bytes to Read 
* @brief 	XX: LSB Nb Bytes to Read
* @brief 	XX: MSB First Byte to Read
* @brief 	XX: LSB First Byte to Read
* @param  *pCommand : Command for the function
* @param  *pResponse : return the response to send at the user by USB HID
* @retval HID_SUCCESS_CODE : the function is succesful
*/
int8_t MSD_CopyTagToFile ( uc8 *pCommand, uint8_t *pResponse)
{
uint16_t 	NbMemoryBlock = 0x00,
					NbBytes_To_Read,
					FirstBytes_To_Read;
int8_t  	status;
uint8_t  	Tag_Density,
					IC_Ref_Tag;
char    	 File_Name[] = {"0:READ_TAG.bin"};

		
		FirstBytes_To_Read = pCommand[2] <<8 | pCommand[3];
		NbBytes_To_Read = pCommand[4] <<8 | pCommand[5];

		// Configure the CR95HF as ISO15693 reader 
		ISO15693_Init();
		// get the identification of the tag 
		errchk(ISO15693_GetTagIdentification(&NbMemoryBlock,&Tag_Density, &IC_Ref_Tag));

		if (NbBytes_To_Read == 0x0000)
			NbBytes_To_Read = NbMemoryBlock * 0x04;
	
		
		ISO15693_ReadBytesTagData(Tag_Density, IC_Ref_Tag, TagMemoryBuffer, NbBytes_To_Read, FirstBytes_To_Read);

		MSD_WriteFile(TagMemoryBuffer, NbBytes_To_Read, File_Name);

		/*Rebbot the USB connection to force windows to update the FAT table of the MSD*/
		USB_MSD_Reboot();

		*(pResponse) = SUCCESFUL_COMMAND_0x80;
		*(pResponse+1) = 0x00;
	
	return MSD_SUCCESS_CODE;
		
Error : 
		pResponse [0] = PCD_ERRORCODE_NOTAGFOUND;
		pResponse [1] = 0x00;
		return PCD_ERRORCODE_NOTAGFOUND;
}

/**
* @brief  Read the data from the file "WRIT_TAG.txt" and write them in the TAG
* @brief  	C2: Command name
* @brief  	04: Length Data
* @brief  	XX: MSB Nb Bytes to Write
* @brief  	XX: LSB Nb Bytes to Write
* @brief  	XX: MSB First Byte to Write
* @brief  	XX: LSB First Byte to Write
* @param  *pCommand : Command for the function
* @param  *pResponse : return the response to send at the user by USB HID
* @retval HID_SUCCESS_CODE.
*/
int8_t MSD_CopyFileToTag (uc8 *pCommand, uint8_t *pResponse)
{
uint16_t Length_Memory_TAG,
				 NbBytes_To_Write_Cmd,
				 NbBytes_To_Write,
				 FirstBytes_To_Write;
uint8_t  Tag_Density,
				 IC_Ref_Tag;

char     File_Name[] = {"0:WRIT_TAG.bin"};

	FirstBytes_To_Write = pCommand[2] <<8 | pCommand[3];
	NbBytes_To_Write_Cmd = pCommand[4] <<8 | pCommand[5];
	
	
	
	MSD_ReadFile(&TagMemoryBuffer[0], &NbBytes_To_Write, File_Name);

	ISO15693_Init();
	ISO15693_GetTagIdentification(&Length_Memory_TAG,&Tag_Density, &IC_Ref_Tag);

	if(NbBytes_To_Write_Cmd == 0x0000)
		ISO15693_WriteBytesTagData(Tag_Density, TagMemoryBuffer, NbBytes_To_Write, FirstBytes_To_Write);	
	else
		ISO15693_Write_Bytes_Tag_Data(Tag_Density, TagMemoryBuffer, NbBytes_To_Write_Cmd, FirstBytes_To_Write);
	
	/*Reboot USB conection if the writing time is near 20 secondes*/
	if(IC_Ref_Tag == ISO15693_LRiS64K && NbBytes_To_Write_Cmd == 0x0000)
	{	
		USB_RebootMSD();
	}
	
	*(pResponse) = SUCCESFUL_COMMAND_0x80;
	*(pResponse+1) = 0x00;
	
	
		return MSD_SUCCESS_CODE;
}


#endif /*USE_MSD_DRIVE*/
/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */



/******************* (C) COPYRIGHT 2014 STMicroelectronics *****END OF FILE****/

