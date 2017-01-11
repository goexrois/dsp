#ifndef _spi_mem_h
#define _spi_mem_h

#ifndef  __IO_Map_H
#include  "IO_Map.h"
#endif

void MEM_Init();
void MEM_WriteProtect_ON();
void MEM_WriteProtect_OFF();
int MEM_WriteEnable();
int MEM_WriteDisable();
unsigned char MEM_ReadStatusRegister();
int MEM_WriteStatusRegister(unsigned char Value);
int MEM_EraseChip();
int MEM_WriteByte(unsigned long Address, unsigned char Data);
unsigned char MEM_ReadByte(unsigned long Address);
int MEM_SectorErase(unsigned long Address);
#endif
