#include "cpu.h"

void uDSC_ICS_Init(void)
{
  // COP Disable
  COP_CTRL=0x0000;  
    
  /*** ### 56F8037 "Cpu" init code ... ***/
  /*** PE initialization code after reset ***/
  /* System clock initialization */
  clrRegBits(GPIO_D_PUPEN, 0x30);      /* Disable pull-up on EXTAL and XTAL pins */
  setRegBits(GPIO_D_PEREN, 0x30);      /* Set EXTAL and XTAL pins as peripheral */
  clrRegBit(OCCS_OCTRL, CLK_MODE);     /* Power-up the external crystal oscillator */
  setRegBitGroup(OCCS_OCTRL, EXT_SEL, 0x02); /* Select an external crystal oscillator to be used */
  setRegBit(OCCS_OCTRL, COHL);         /* External crystal oscillator clock source */
  /* Wait about 10ms for external oscillator stabilization */
  asm {
    /*
     * Delay
     *   - requested                  : 10 ms @ 4MHz,
     *   - possible                   : 40000 c, 10000000 ns
     */
    move.w #0x9C36, A                  /* (2 c: 500 ns) number of iterations */
    do A, label0                       /* (8 c: 2000 ns) repeat 39990x nop */
      nop                              /* (1 c: 250 ns) wait for 1 c */
    label0:
  }
  setRegBit(OCCS_CTRL, PRECS);         /* Select an external clock source for the CPU core */
  asm(nop);
  asm(nop);
  asm(nop);
  asm(nop);
  setRegBit(OCCS_OCTRL, ROPD);         /* Power down the internal relaxation oscillator */
  setReg(OCCS_CTRL, (OCCS_CTRL_PRECS_MASK | OCCS_CTRL_LCKON_MASK | OCCS_CTRL_ZSRC0_MASK)); /* Enable PLL, LCKON and select clock source from prescaler */
  /* OCCS_DIVBY: LORTP=2,??=0,PLLCOD=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0 */
  setReg16(OCCS_DIVBY, 0x2000);        /* Set the clock prescalers */ 
  while(!getRegBit(OCCS_STAT, LCK0)){} /* Wait for PLL lock */
  setReg(OCCS_CTRL, (OCCS_CTRL_PRECS_MASK | OCCS_CTRL_LCKON_MASK | OCCS_CTRL_ZSRC1_MASK)); /* Select clock source from postscaler */
  /* FM_CLKDIV: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,DIVLD=0,PRDIV8=0,DIV=0x28 */
  setReg16(FM_CLKDIV, 0x28);           /* Set the flash clock prescaler */ 
  /*** End of PE initialization code after reset ***/	

  // Enabled RESET
  GPIO_A_DDIR &=~ 0x0080; 
  GPIO_A_PEREN |= 0x0080;
  
  asm (bfclr #$0300,sr);           /* allow lowest priority */		
}
