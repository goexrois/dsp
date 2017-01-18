/** 
  * FFT PARA BOLUDOS
  *
  */

#include <stdlib.h>
#include <stdio.h>
#define SAMPLE_LENGTH 512

/** Prototipos **/

void index_generator(int*) ;
int reverse(int,int) ; 


int main(void){
	int indexes[SAMPLE_LENGTH] = {0}; 
	int i ;
	index_generator(indexes) ; 
//	for( i = 0 ; i < SAMPLE_LENGTH ; i++ ){
		printf("%d, ",1U) ; 
//	}	
	printf("\n") ;
	return 1 ;
}

void index_generator( int* indexes){
	
}


int reverse (int index, int bits){
	
}
