#ifndef _lcd_h
#define _lcd_h

#ifndef  __IO_Map_H
#include  "IO_Map.h"
#endif

#define LCD_ROW1 0x80
#define LCD_ROW2 0xC0


void  uDSC_LCD_Init(void);
void  uDSC_LCD_WriteChar(unsigned char Direccion, char Dato);
void  uDSC_LCD_WriteStr(unsigned char Direccion, char *String);
void  uDSC_LCD_WriteStr_Mode1(unsigned char Direccion, char *String);

#endif