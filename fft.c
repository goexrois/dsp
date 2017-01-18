/** 
  * FFT PARA BOLUDOS
  *
  */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define SAMPLE_LENGTH 512

/** Prototipos **/

void index_generator(int*) ;
unsigned int reverse(unsigned int,int) ; 
void butterfly(int, int, int, int) ;
void fft() ;

int main(void){
	int indexes[SAMPLE_LENGTH] = {0}; 
	int i ;
	index_generator(indexes) ; 
	for( i = 0 ; i < SAMPLE_LENGTH ; i++ )
		printf("%u\n", indexes[i]) ; 

	return 1 ;
}

void index_generator( int* indexes){
	unsigned i ;
	for( i = 0; i < SAMPLE_LENGTH; i++ ){
		indexes[i] = reverse(i, log2(SAMPLE_LENGTH)) ;
	}	
}


unsigned int reverse (unsigned int index, int bits){
	 index = ((index & 0x55555555) << 1 ) | ((index & 0xAAAAAAAA) >>1) ; 
	 index = ((index & 0x33333333) << 2 ) | ((index & 0xCCCCCCCC) >>2) ; 
	 index = ((index & 0x0f0f0f0f) << 4 ) | ((index & 0xf0f0f0f0) >>4) ; 
	 index = ((index & 0x00ff00ff) << 8 ) | ((index & 0xff00ff00) >>8) ;
	 index = ((index & 0x0000ffff) << 16) | ((index & 0xffff0000) >>16) ;	
	 return index >> ( 32 - bits ) ; 
}

void butterfly(int a0, int a1, int* o0, int* o1){
	*o0 = a0 + a0*a1 ;
	*o1 = a0 - a0*a1 ;
}

void fft(int* muestras){
	int i,j ;
	int index[SAMPLE_LENGTH] ;
	char bits = log2(SAMPLE_LENGTH) ; 
	for ( i = 0 ; i < log2(SAMPLE_LENGTH) ; i++ ){
		for( j = 0 ; j < SAMPLE_LENGTH ; j++ ){
			butterfly( muestras[reverse(j,bit)], muestras[reverse(j+1,bit)] ) ;
		}
	}
}
