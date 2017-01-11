#include "filters.h"


void FIR16_Init(FIR16_Struct *filter, Frac16 *ptrcoef_buf, Frac16 *ptrsamples_buf, UInt16 order) 
{
	filter->coeficients=ptrcoef_buf;
	filter->inp_samples=ptrsamples_buf;
	filter->order=order;
}


Frac16 FIR16_Filter(FIR16_Struct *filter, Frac16 current_sample)
{
	UInt16 i;
	Frac32 res=0;	
	Frac16 *ptr_coef = filter->coeficients;
	Frac16 *ptr_samples = filter->inp_samples;
	
	ptr_samples += filter->order;
	ptr_samples -= 2;

	for (i=filter->order;i>1;i--)
	{	    
	    res=__L_mac(res,*ptr_samples,*ptr_coef);
	    *ptr_samples = *(ptr_samples-1);	    
	    ptr_samples--;
	    ptr_coef++;	    
	}
	
	*(filter->inp_samples) = current_sample;
	return __mac_r(res,*ptr_coef,current_sample);
}


Frac16 IIR16_Run(IIR16_Struct *filter, Frac16 current_sample)
{
	
}