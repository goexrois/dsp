#ifndef _filters_h
#define _filters_h

#ifndef  __IO_Map_H
#include  "IO_Map.h"
#endif

typedef struct
{
	Frac16 *coeficients;
	Frac16 *inp_samples;
	UInt16 order;
	UInt16 status;
}FIR16_Struct;

typedef struct
{
	Frac16 *coeficients;
	Frac16 *inp_samples;
	Frac16 *out_samples;	
	UInt16 order;
	UInt16 status;
}IIR16_Struct;


void FIR16_Init(FIR16_Struct *filter, Frac16 *ptrcoef_buf, Frac16 *ptrsamples_buf, UInt16 order);
Frac16 FIR16_Filter(FIR16_Struct *filter, Frac16 current_sample);

#endif
