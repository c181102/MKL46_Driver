/*
 * flash.c
 *
 *  Created on: Mar 11, 2024
 *      Author: DELL
 */

#include "MKL46Z4.h"

#define CMD_PROGRAM_LONGWORD	0x06
#define CMD_ERASE_FLASH_SECTOR	0x09

// return status
void FLASH_Write32(uint32_t address, uint32_t data)
{
	while ((FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK) == 0U);

	if ((FTFA->FSTAT & FTFA_FSTAT_ACCERR_MASK) != 0U || (FTFA->FSTAT & FTFA_FSTAT_FPVIOL_MASK) != 0U)
	{
		FTFA->FSTAT = (FTFA_FSTAT_ACCERR_MASK | FTFA_FSTAT_FPVIOL_MASK | FTFA_FSTAT_RDCOLERR_MASK);
	}

	FTFA->FCCOB0 = CMD_PROGRAM_LONGWORD;

	FTFA->FCCOB3 = (uint8_t)(address >> 0U);
	FTFA->FCCOB2 = (uint8_t)(address >> 8U);
	FTFA->FCCOB1 = (uint8_t)(address >> 16U);

	FTFA->FCCOB7 = (uint8_t)(data >> 0U);
	FTFA->FCCOB6 = (uint8_t)(data >> 8U);
	FTFA->FCCOB5 = (uint8_t)(data >> 16U);
	FTFA->FCCOB4 = (uint8_t)(data >> 24U);

	FTFA->FSTAT = FTFA_FSTAT_CCIF_MASK;
}

uint8_t FLASH_Read8(uint32_t address)
{
	return *(uint8_t*)address;
}

uint32_t FLASH_Read32(uint32_t address)
{
	return *(uint32_t*)address;
}

uint8_t* FLASH_ReadBuffer(uint32_t address)
{

}

void FLASH_EraseSector(uint32_t address)
{
	/* Flash Configuration */
	if (0x400 <= address && address <= 0x7FF)
	{
		return;
	}

	while ((FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK) == 0U);

	if ((FTFA->FSTAT & FTFA_FSTAT_ACCERR_MASK) != 0U || (FTFA->FSTAT & FTFA_FSTAT_FPVIOL_MASK) != 0U)
	{
		FTFA->FSTAT = (FTFA_FSTAT_ACCERR_MASK | FTFA_FSTAT_FPVIOL_MASK | FTFA_FSTAT_RDCOLERR_MASK);
	}

	FTFA->FCCOB0 = CMD_ERASE_FLASH_SECTOR;

	FTFA->FCCOB3 = (uint8_t)(address >> 0U);
	FTFA->FCCOB2 = (uint8_t)(address >> 8U);
	FTFA->FCCOB1 = (uint8_t)(address >> 16U);

	FTFA->FSTAT = FTFA_FSTAT_CCIF_MASK;
}


