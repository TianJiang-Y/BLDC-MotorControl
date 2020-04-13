/**
  ******************************************************************************
  * @file    bsp_internalFlash.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   Internal FLASH Read and Write Code
  ******************************************************************************
  */
  
#include "bsp_internal_flash.h"   
	
typedef volatile u32  vu32;

int InternalFlash_Test(void)
{
	uint32_t EraseCounter = 0x00; 	// Record how many pages to erase
	uint32_t Address = 0x00;				// Record write address
	uint32_t Data = 0x3210ABCD;			// Record the written data
	uint32_t NbrOfPage = 0x00;			// Record how many pages are written
	
	FLASH_Status FLASHStatus = FLASH_COMPLETE; // Record the results of each erasure
	TestStatus MemoryProgramStatus = PASSED;   // Record the entire test result
	

  /* Lock Off */
  FLASH_Unlock();

  /* Calculate how many pages to erase */
  NbrOfPage = (WRITE_END_ADDR - WRITE_START_ADDR) / FLASH_PAGE_SIZE;


  /* Clear all flags */
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	

  /* Erase by page*/
  for(EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
  {
    FLASHStatus = FLASH_ErasePage(WRITE_START_ADDR + (FLASH_PAGE_SIZE * EraseCounter));
  }
  
  /* Write data to internal FLASH */
  Address = WRITE_START_ADDR;

  while((Address < WRITE_END_ADDR) && (FLASHStatus == FLASH_COMPLETE))
  {
    FLASHStatus = FLASH_ProgramWord(Address, Data);
    Address = Address + 4;
  }

  FLASH_Lock();
  
  /* Check if the data written is correct */
  Address = WRITE_START_ADDR;

  while((Address < WRITE_END_ADDR) && (MemoryProgramStatus != FAILED))
  {
    if((*(__IO uint32_t*) Address) != Data)
    {
      MemoryProgramStatus = FAILED;
    }
    Address += 4;
  }
	return MemoryProgramStatus;
}

void STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite)	
{ 
	u32 endaddr=0;	

	FLASH_Unlock();	// Lock Off

	endaddr=WriteAddr+NumToWrite*4;	// Write end address
	
//	  /* Calculate how many pages to erase */
//  NbrOfPage = (endaddr - addrx) / FLASH_PAGE_SIZE;	
	
  /* Clear all flags */
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	
	
	  /* Erase by page */
  if(WriteAddr == WRITE_START_ADDR)
  {
			FLASH_ErasePage(WriteAddr);//FLASH_PAGE_SIZE
  }  
	
  while(WriteAddr < endaddr)
  {
    if(!FLASH_ProgramWord(WriteAddr, *pBuffer))
		{
			break;
		}
    WriteAddr+=4;
		pBuffer++;
  }
	
	FLASH_Lock(); // Lock on
} 

//Read the data from Internal Flash at pointed out address.
//ReadAddr: Read Start Address
//pBuffer: Data Pointer
//NumToRead: 32-bit number

u8 STMFLASH_Read(u32 ReadAddr,u32 *pBuffer)
{
	u8 NumToRead = 30;
	u8 NumLength = 0;
	
	while(NumToRead--)
	{
		*pBuffer = *(vu32*)ReadAddr; // Read 4-bytes
		if(*pBuffer == 0xFFFFFFFF)
			return NumLength;
		ReadAddr+=4;// Pointer Increase 4 Bytes
		pBuffer++;
		NumLength++;
	}
	return NumToRead;
}
