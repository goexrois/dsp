#include "lcd.h"


#define LCD_PORT_DATO GPIO_C_DATA
#define LCD_PORT_DATO_CONF GPIO_C_DATA

#define LCD_E_ON   GPIO_C_DATA |= GPIO_C_DATA_D13_MASK;
#define LCD_E_OFF  GPIO_C_DATA &=~GPIO_C_DATA_D13_MASK;
#define LCD_RS_ON  GPIO_A_DATA |= GPIO_A_DATA_D11_MASK;
#define LCD_RS_OFF GPIO_A_DATA &=~GPIO_A_DATA_D11_MASK;
#define LCD_RW_ON  GPIO_C_DATA |= GPIO_C_DATA_D12_MASK;
#define LCD_RW_OFF GPIO_C_DATA &=~GPIO_C_DATA_D12_MASK;
#define LCD_HAL_INIT  GPIO_C_PEREN &=~ 0x30F0;GPIO_A_PEREN &=~ 0x0800;GPIO_C_DATA &=~ 0x30F0;GPIO_A_DATA &=~ 0x0800;GPIO_C_DDIR |= 0x30F0;GPIO_A_DDIR |= 0x0800;	


inline void Delay();


void  LCD_EDireccion(unsigned char Dato);
void  LCD_EDato(char Dato);
void  LCD_Espera(void);



void  uDSC_LCD_Init(void) {	   
  LCD_HAL_INIT;
	
  LCD_EDireccion(0x30);
  LCD_Espera();
  LCD_EDireccion(0x30);
  LCD_Espera();
  LCD_EDireccion(0x30);
  LCD_Espera();
  LCD_EDireccion(0x20);
  LCD_Espera();
  
  LCD_EDireccion(0x20);
  LCD_EDireccion(0x80);
  LCD_Espera();
  

  LCD_EDireccion(0x10);
  LCD_EDireccion(0x40);
  LCD_Espera();
  
  LCD_EDireccion(0x00);
  LCD_EDireccion(0xC0);
  LCD_Espera();
  
  LCD_EDireccion(0x00);
  LCD_EDireccion(0x40);
  LCD_Espera();      
  
  LCD_EDireccion(0x00);
  LCD_EDireccion(0x10);
  LCD_Espera();      
  LCD_Espera();      
}




void  LCD_EDireccion(unsigned char Direc){
  LCD_PORT_DATO=((LCD_PORT_DATO & 0xFF0F) | (Direc & 0xF0));
  LCD_E_ON;
  asm(nop);
  asm(nop);
  asm(nop);
  asm(nop);
  asm(nop);
  asm(nop);
  asm(nop);
  asm(nop);
  asm(nop);
  asm(nop);
  LCD_E_OFF;
}

void  LCD_EDato(char Dato){
  LCD_PORT_DATO=((LCD_PORT_DATO & 0xFF0F) | (Dato & 0xF0));
  LCD_RS_ON; 
  LCD_E_ON;
  asm(nop);
  asm(nop);
  asm(nop);
  asm(nop);
  asm(nop);
  asm(nop);
  asm(nop);
  asm(nop);
  asm(nop);
  asm(nop);
  LCD_E_OFF;
  LCD_RS_OFF;
}

void  LCD_Espera(void){
  unsigned int i;
  LCD_PORT_DATO_CONF &=~0x00F0;
  LCD_RW_ON;
  LCD_E_ON
  //while((LCD_PORT_DATO & 0x00F0) > 128){};
  i=2000;
  while (i>0) i--;
  LCD_E_OFF;
  LCD_RW_OFF;
  LCD_PORT_DATO_CONF |=0x00F0;
} 



void  uDSC_LCD_WriteChar(unsigned char Direccion, char Dato){
  LCD_EDireccion(Direccion);
  Direccion=(unsigned char)(Direccion<<4);
  LCD_EDireccion(Direccion);
  LCD_Espera();
  
  LCD_EDato(Dato);
  Dato=(char)(Dato<<4);
  LCD_EDato(Dato);
  LCD_Espera();
  
}

void  uDSC_LCD_WriteStr(unsigned char Direccion, char *String){
  
  if (Direccion >= 0x80) 
   while (*String!='\0') 
      uDSC_LCD_WriteChar(Direccion++,*String++);
   
}  


#define TIMEOUT_DELAY 50000
inline void Delay()
{
	unsigned long i;
	
	i=TIMEOUT_DELAY;
	while (i-->0) 
	{
	 asm(NOP);
	}
}


void  uDSC_LCD_WriteStr_Mode1(unsigned char Direccion, char *String){
  unsigned char i,j,x;
	
  i=0;
  
  uDSC_LCD_WriteStr(Direccion,"                ");
  
  while (*String != '\0')
  {
    x=(unsigned char)(Direccion+0x0F);
    for (j=15;j>i;j--)
    {
      uDSC_LCD_WriteChar(x--,' ');
      uDSC_LCD_WriteChar(x,*String);
      if (*String != ' ')  Delay();
    }
    String++;
    i++;
  }

}