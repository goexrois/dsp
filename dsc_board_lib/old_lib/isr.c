

 

/*  metrowerks sample code  */



/*
	this source illustrates code called by ISR 
	and #pragma directives necessary for ISR-safe code		
*/



#include <stdio.h>
#include "MC56F802x3x.h"
#include "led.h"
#include "isr.h"
#include "lcd.h"
static void delay ();

int led_position = 0;
int led_group = 0;



void initIsr ()
{
    SIM_PCE1 = 0x0001;
//  asm (move.w #$0001,X:SIM_PCE1);   /* turn on clock to timer A0*/
    
	asm (bfclr #$0300,sr);           /* allow lowest priority */
	INTC_IPR4 |= 0x0300;
//	asm (bfset #$0300,X:ITCN_IPR4);	 /* TMRA 0 IPL 2 */
	INTC_IPR2 |= 0x000C;	 
//  asm (bfset #$000C,X:ITCN_IPR2);  /* GPIO B IPL 2 */

/* Use GPIOB2 and GPIOB3 for IRQ's */
//	asm(bfclr #$000C,X:GPIOB_PEREN); /* set bits for GPIO B enable */
	GPIO_B_PEREN &=~ 0x000C;
//	asm(bfclr #$000C,X:GPIOB_DDIR);  /* set bits for output */
	GPIO_B_DDIR &=~ 0x000C;
//	asm(bfset #$000C,X:GPIOB_IEN);   /* set bits in interrupt enable reg */
	GPIO_B_IEN |= 0x000C;

	/* stop timer 0*/
//	asm	(move.w	#$0000,X:TMRA0_CTRL);
	TMRA0_CTRL = 0x0000;
	
	/* timer A channel 0 */
//	asm (move.w #$5555,X:TMRA0_COMP1);
	TMRA0_COMP1 = 0x5555;	
//	asm (move.w	#$0000,X:TMRA0_COMP2);
	TMRA0_COMP2 = 0x0000;	
//	asm (move.w #$FFFF,X:TMRA0_LOAD);
	TMRA0_LOAD = 0xFFFF;	
//	asm (move.w	#$FFFF,X:TMRA0_CNTR);
	TMRA0_CNTR = 0xFFFF;
	/* 100000000000000	*/
	/* bit 14 -- TCFIE - Timer Compare Flag Interrupt Enable  */
    /* enables interrupts when the TCF bit is set. */
//	asm (move.w #$4000,X:TMRA0_SCTRL);
	TMRA0_SCTRL = 0x4000;
	
	/* 111110000110000 */
	/* bit 13 -- turn on counter; count rising edges of source  */
	/* bits 9-12 -- divide by 128  */
	/* bit 5 -- direction: count up  */
	/* bit 4 -- length: count until compare, then reinitialize.  */
//	asm (bfset #$7C30,X:TMRA0_CTRL);	 /* Run counter */	
	TMRA0_CTRL |= 0x7C30;

}



/* Interrupt-safe function: 
A function call that conserves all register content
(excluding PC,SP) across a call. Please note that 
Standard C libary is not interrupt-safe */


/* ISR that call non interrupt-safe functions need
#pragma interrupt saveall to:

1. call a runtime routine that save all register (except SP,PC)
2. return with RTI */
#pragma interrupt saveall

void gpioB_ISR(void)
{
//	unsigned int * pGPIOB_IEDGE = (unsigned int *)GPIOB_IEDGE;
	

	/* debounce key with delay for simplicity, disadvantage:
	1. high latency
	2. no repoll of switch state after deounce delay */

//	if (((*pGPIOB_IEDGE) & 0x0004) == 0x0004)  // ASSIGNED TO GPIOB2 (BIT 2)
//	{
//	    delay ();
	    
        /* Standard C library call */
//	    puts("GPIOB_IEDGE detected: EVM pushbutton #1");	 
	    
//	    offLED (led_position + 1 + led_group * 3);	/* switch off LED from last group */
//	    led_group = 0;
	    
//		asm(move.w #$0004,X:GPIOB_IEDGE);           /* clear IESR */
//	}
	
//	if (((*pGPIOB_IEDGE) & 0x0008) == 0x0008) // ASSIGNED TO GPIOB3 
//	{
//	    delay ();
	
        /* Standard C library call */
//	    puts("GPIOB_IEDGE detected: EVM pushbutton #2");	  
	    
//	    offLED (led_position + 1 + led_group * 3);	/* switch off LED from last group */
//	    led_group = 1;
	    
//		asm(move.w #$0008,X:GPIOB_IEDGE);           /* clear IESR */
//	}
	
}





/* ISR that does not call interrupt-unsafe functions can use #pragma interrupt to:
1. save all used register
2. return with RTI
*/

#pragma interrupt

void isrTimerA0Compare()
{
	/* next LED */
//	offLED (led_position + 1 + led_group * 3);
//	led_position++;
//	led_position = led_position % 3;
//	onLED (led_position + 1 + led_group * 3);
	
    // 100000000000000	-- bit 14
	/* clear compare status flags */
//	asm (move.w #$4000,X:TMRA0_SCTRL);
}





#pragma interrupt called
static void delay ()
{
	long i;
	
	for (i = 0; i < 0x7FFF; i++)
	{
		/* more nop's for consistence across optimization */
		asm (nop);
		asm (nop);
		asm (nop);
		asm (nop);
		asm (nop);
		asm (nop);
		asm (nop);
		asm (nop);
		asm (nop);
		asm (nop);
		asm (nop);
		asm (nop);
		asm (nop);
		asm (nop);
		asm (nop);
		asm (nop);
	}
}

