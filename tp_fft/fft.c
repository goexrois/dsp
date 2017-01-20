/** 
  * FFT PARA BOLUDOS
  *
  */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define SAMPLE_LENGTH 8

/** Prototipos **/

void index_generator(int,int*) ;
unsigned int reverse(unsigned int,int) ; 
void butterfly(int, int, int, int) ;
void fft(int*) ;

int main(void){
	int indexes[SAMPLE_LENGTH] = {0} ; 
	int muestras[SAMPLE_LENGTH] = {0} ;
	int i ;
	for ( i = 0 ; i < SAMPLE_LENGTH ; i++ ){
		muestras[i] = sin(2*3.14*i/8) ;
		printf("%d\n",muestras[i]);
	}
	return 1 ;
}

void index_generator(int largo, int* indexes){
	unsigned int i ;
	for( i = 0; i < SAMPLE_LENGTH; i++ ){
		indexes[i] = reverse(i, log2(largo)) ;
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

void butterfly(int* a0, int* a1){
	int aux = *a0 ;
	*a0 = *a0 + *a0*(*a1) ;
	*a1 = aux - aux*(*a1) ;
}

void fft(int* res){
	int i,j,k ;
	char niveles = log2(SAMPLE_LENGTH) ; 
	int indexes[SAMPLE_LENGTH] = {0} ; 
	/*int index[SAMPLE_LENGTH] ;
	for ( i = 0 ; i < log2(SAMPLE_LENGTH) ; i++ ){
		for( j = 0 ; j < SAMPLE_LENGTH ; j++ ){
			butterfly( muestras[reverse(j,bit)], muestras[reverse(j+1,bit)] ) ;
		}
	}*/
	for ( i = 1 ; i < niveles+1 ; i++ ){
		index_generator(SAMPLE_LENGTH/i,indexes) ; 
		for( j = 0 ; j < i ; j++ ){
			for ( k = 0 ; k < SAMPLE_LENGTH/(2*i) ; k++){
				butterfly(res[indexes[k]],res[indexes[k+1]]) ;
			}
		} 
	}	
}
