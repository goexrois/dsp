#include "spi_mem.h"

#define MEM_CE_OFF GPIO_B_DATA &=~ (GPIO_B_DATA_D4_MASK)
#define MEM_CE_ON 	GPIO_B_DATA |= (GPIO_B_DATA_D4_MASK)

#define MEM_WP_ON  GPIO_A_DATA |= GPIO_A_DATA_D8_MASK
#define MEM_WP_OFF GPIO_A_DATA &=~ GPIO_A_DATA_D8_MASK

#define WRSR 0x01
#define WRBP 0x02
#define RDBP 0x03
#define WREN 0x06
#define RDSR 0x05
#define WRDI 0x04
#define ERSE 0x20
#define EWSR 0x50
#define ERCH 0xC7

void MEM_WriteProtect_ON()
{
	MEM_WP_ON;
}

void MEM_WriteProtect_OFF()
{
	MEM_WP_OFF;
}

void MEM_Init () 
{	
	// Select direction pins
	GPIO_A_DDIR |= (GPIO_A_DDIR_DD12_MASK + GPIO_A_DDIR_DD14_MASK + GPIO_A_DDIR_DD8_MASK);  //CLK + MOSI + WP
	GPIO_B_DDIR |= (GPIO_B_DDIR_DD4_MASK);  //SS
	GPIO_A_DDIR &=~ (GPIO_A_DDIR_DD13_MASK); // MISO
	
	MEM_CE_ON;
	MEM_WP_ON;
	
	// Outputs controlled via SPI module
	GPIO_A_PEREN |= (GPIO_A_PEREN_PE12_MASK + GPIO_A_PEREN_PE13_MASK + GPIO_A_PEREN_PE14_MASK); // CLK + MOSI + MISO automatic drive
	GPIO_A_PEREN &=~ (GPIO_A_PEREN_PE8_MASK);	 // WP pin memory
	GPIO_B_PEREN &=~ (GPIO_B_PEREN_PE4_MASK);	 // SS pin manual drive


	// Select module that controll the pinout
	SIM_GPSA1 |= SIM_GPSA1_GPS_A140_MASK+SIM_GPSA1_GPS_A130_MASK+SIM_GPSA1_GPS_A120_MASK;

	// SPI Clock enabled
	SIM_PCE0|=SIM_PCE0_QSPI1_MASK;

    
	QSPI1_SCTRL  = (QSPI0_SCTRL_SPMSTR_MASK ); 
   // QSPI1_SCTRL |= QSPI0_SCTRL_CPOL_MASK | QSPI0_SCTRL_CPHA_MASK;    
	QSPI1_DSCTRL = (0x0007);		
	QSPI1_FIFO = (QSPI0_FIFO_FIFO_ENA_MASK | QSPI0_FIFO_TFWM0_MASK |QSPI0_FIFO_TFWM1_MASK | QSPI0_FIFO_RFWM0_MASK | QSPI0_FIFO_RFWM1_MASK);
    QSPI1_DELAY = 0x0000;
    (void)QSPI1_DRCV;
    (void)QSPI1_DXMIT;
	QSPI1_SCTRL  |= 	QSPI1_SCTRL_SPE_MASK;
}



int MEM_WriteEnable()
{
	MEM_CE_OFF;				// CE Down
	
    QSPI1_DXMIT = WREN;
    while ((QSPI1_FIFO & (QSPI1_FIFO_RFCNT_MASK)) != QSPI1_FIFO_RFCNT0_MASK) {asm (NOP);}
    (void)QSPI1_DRCV;
	        
    MEM_CE_ON;
    return TRUE;
}

unsigned char MEM_ReadStatusRegister()
{
    unsigned char a;
    
	MEM_CE_OFF;				// CE Down
	
    QSPI1_DXMIT = RDSR;
    QSPI1_DXMIT = 0x00;
    while ((QSPI1_FIFO & (QSPI1_FIFO_RFCNT_MASK)) != QSPI1_FIFO_RFCNT1_MASK) {asm (NOP);}
    (void)QSPI1_DRCV;
    a=(unsigned char)QSPI1_DRCV;
	        
    MEM_CE_ON;	
    
    return a;
}

int MEM_EraseChip()
{
	MEM_WriteEnable();	
	
	MEM_CE_OFF;				// CE Down
	
    QSPI1_DXMIT = ERCH;
    while ((QSPI1_FIFO & (QSPI1_FIFO_RFCNT_MASK)) != QSPI1_FIFO_RFCNT0_MASK) {asm (NOP);}
    (void)QSPI1_DRCV;
	        
    MEM_CE_ON;
    while (MEM_ReadStatusRegister() & 0x01) asm {nop};    
    return TRUE;
}

int MEM_WriteStatusRegister(unsigned char Value)
{
	unsigned char i;
		
	MEM_WriteEnable();	
	
	MEM_CE_OFF;				// CE Down
	
    QSPI1_DXMIT = EWSR;
    while ((QSPI1_FIFO & (QSPI1_FIFO_RFCNT_MASK)) != QSPI1_FIFO_RFCNT0_MASK) {asm (NOP);}
    (void)QSPI1_DRCV;
	        
    MEM_CE_ON;	
    
    i=100;
    while (i-->0)  {asm(nop);}
	
	MEM_CE_OFF;				// CE Down
	
    QSPI1_DXMIT = WRSR;
    QSPI1_DXMIT = Value;
    while ((QSPI1_FIFO & (QSPI1_FIFO_RFCNT_MASK)) != QSPI1_FIFO_RFCNT1_MASK) {asm (NOP);}
    (void)QSPI1_DRCV;
    (void)QSPI1_DRCV;
	        
    MEM_CE_ON;		
    return TRUE;
}


int MEM_WriteByte(unsigned long Address, unsigned char Data)
{
	MEM_WriteEnable();	
	
	MEM_CE_OFF;				// CE Down
	
    QSPI1_DXMIT = WRBP;
    QSPI1_DXMIT = (unsigned int)(Address & 0x00FF0000)>>16;
    QSPI1_DXMIT = (unsigned int)(Address & 0x0000FF00)>>8;
    QSPI1_DXMIT = (unsigned int)(Address & 0x000000FF);
    while ((QSPI1_FIFO & (QSPI1_FIFO_RFCNT_MASK)) != QSPI1_FIFO_RFCNT2_MASK) {asm (NOP);}
    (void)QSPI1_DRCV;
    (void)QSPI1_DRCV;	        
    (void)QSPI1_DRCV;
    (void)QSPI1_DRCV;
    
    QSPI1_DXMIT = Data;
    while ((QSPI1_FIFO & (QSPI1_FIFO_RFCNT_MASK)) != QSPI1_FIFO_RFCNT0_MASK) {asm (NOP);}
    (void)QSPI1_DRCV;
    
    MEM_CE_ON;		
    
    while (MEM_ReadStatusRegister() & 0x01) asm {nop};
    return TRUE;
}

unsigned char MEM_ReadByte(unsigned long Address)
{
	unsigned char a=0;
	
	MEM_CE_OFF;				// CE Down
	
    QSPI1_DXMIT = RDBP;
    QSPI1_DXMIT = (unsigned int)(Address & 0x00FF0000)>>16;
    QSPI1_DXMIT = (unsigned int)(Address & 0x0000FF00)>>8;
    QSPI1_DXMIT = (unsigned int)(Address & 0x000000FF);
    while ((QSPI1_FIFO & (QSPI1_FIFO_RFCNT_MASK)) != QSPI1_FIFO_RFCNT2_MASK) {asm (NOP);}
    (void)QSPI1_DRCV;
    (void)QSPI1_DRCV;	        
    (void)QSPI1_DRCV;
    (void)QSPI1_DRCV;
    
    QSPI1_DXMIT = 0x00;
    while ((QSPI1_FIFO & (QSPI1_FIFO_RFCNT_MASK)) != QSPI1_FIFO_RFCNT0_MASK) {asm (NOP);}
    a=(unsigned char)QSPI1_DRCV;
    
    MEM_CE_ON;	
    
    return a;	
}

int MEM_SectorErase(unsigned long Address)
{
	
	MEM_CE_OFF;				// CE Down
		
    QSPI1_DXMIT = ERSE;
    QSPI1_DXMIT = (unsigned int)(Address & 0x00FF0000)>>16;
    QSPI1_DXMIT = (unsigned int)(Address & 0x0000FF00)>>8;
    QSPI1_DXMIT = (unsigned int)(Address & 0x0000FF);
    while ((QSPI1_FIFO & (QSPI1_FIFO_RFCNT_MASK)) != QSPI1_FIFO_RFCNT2_MASK) {asm (NOP);}
    (void)QSPI1_DRCV;
    (void)QSPI1_DRCV;	        
    (void)QSPI1_DRCV;
    (void)QSPI1_DRCV;
    MEM_CE_ON;	
    
    return TRUE;	
}
int MEM_WriteDisable()
{
	MEM_CE_OFF;				// CE Down
	
    QSPI1_DXMIT = WRDI;
    while ((QSPI1_FIFO & (QSPI1_FIFO_RFCNT_MASK)) != QSPI1_FIFO_RFCNT0_MASK) {asm (NOP);}
    (void)QSPI1_DRCV;
	        
    MEM_CE_ON;
    return TRUE;	
}