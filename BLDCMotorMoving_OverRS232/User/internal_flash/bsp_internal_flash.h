#ifndef __INTERNAL_FLASH_H
#define	__INTERNAL_FLASH_H

#include "stm32f10x.h"

/* STM32 large-capacity products 2KByte per page, small and medium-capacity products 1KByte per page */
#if defined (STM32F10X_HD) || defined (STM32F10X_HD_VL) || defined (STM32F10X_CL) || defined (STM32F10X_XL)
  #define FLASH_PAGE_SIZE    ((uint16_t)0x800)	//2048
#else
  #define FLASH_PAGE_SIZE    ((uint16_t)0x400)	//1024
#endif

// Write start and end addresses
#define WRITE_START_ADDR  ((uint32_t)0x08007800)
#define WRITE_END_ADDR    ((uint32_t)0x08008000)



typedef enum 
{
	FAILED = 0, 
  PASSED = !FAILED
} TestStatus;


int InternalFlash_Test(void);
void STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite);
u8 STMFLASH_Read(u32 ReadAddr,u32 *pBuffer);



#endif /* __INTERNAL_FLASH_H */

