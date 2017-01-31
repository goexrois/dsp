/** 
  * FFT PARA BOLUDOS
  *
  */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define SAMPLE_LENGTH 1024

typedef struct{
	int a, b;
} complex_t;

/** Prototipos **/

void index_generator(int,int*) ;
unsigned int reverse(unsigned int,int) ; 
void butterfly(complex_t*, complex_t*, int, int) ;
void fft(complex_t*,int) ;
float norma (complex_t c) ;
void p_array(FILE*,complex_t* array,int size);
void p_array_int(FILE*,int* array,int size);
complex_t complex_add(complex_t, complex_t);
complex_t complex_sub(complex_t, complex_t);
complex_t complex_mult(complex_t x, complex_t y);
FILE* out ;
FILE* debug ;
int main(int argc,char** argv){
	int i ;
	int length = SAMPLE_LENGTH ;
	complex_t * muestras ;
	FILE* signal ;
	debug = fopen("./debug","w+");
	out = fopen("./out","w+");
	signal = fopen("./signal","w+");

	if( argc > 1 ){
		length = atoi(argv[1]);  
	} 

	muestras = (complex_t *) malloc(sizeof(complex_t)*length) ;

	for ( i = 0 ; i < length ; i++ ){
		muestras[i].a = sin(M_PI*2*(i+1)/8) ;
		muestras[i].b = 0 ;
	}
	p_array(signal,muestras,length) ;
	fprintf(debug,"fft para retrasados ------------------------------\n");
	fft(muestras,length) ; 
	p_array(out,muestras,length) ;

	return 1 ;
}

void index_generator(int largo, int* indexes){
	unsigned int i ;
	for( i = 0; i < largo; i++ ){
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

complex_t complex_add(complex_t x, complex_t y){
	complex_t result ;
	result.a = x.a + y.a ;
	result.b = x.b + y.b ;
	
	return result ;
}

complex_t complex_sub(complex_t x, complex_t y){
	complex_t result ;
	result.a = x.a - y.a ;
	result.b = x.b - y.b ;
	
	return result ;
}

complex_t complex_mult(complex_t x, complex_t y){
	complex_t result ;
	result.a = x.a*y.a - x.b*y.b ;
	result.b = x.a*y.b + x.b*y.a ;
	
	return result ;
}

void butterfly(complex_t* a0, complex_t* a1, int order, int n){
	complex_t W ;
	complex_t mult_aux ;

	W.a = cos(2*M_PI*order/n) ;
	W.b = -sin(2*M_PI*order/n) ;

	mult_aux = complex_mult(a1, W) ;
	complex_t aux = *a0 ;

	*a0 = complex_add(*a0, mult_aux) ;
	*a1 = complex_sub(aux, mult_aux) ;
}

void fft(complex_t* res,int length){
	int i,j,k,power ;
	char niveles = log2(length) ; 
	int * indexes = (int *) malloc(sizeof(int)*length) ;
	complex_t w_m, w, mult_aux ;
	fprintf(debug,"i\tj\tk\n") ;
//	index_generator(length,indexes) ; 
	for ( i = 1 ; i <= niveles ; i++ ){
		index_generator(length/i,indexes) ; 
		p_array_int(debug,indexes,length/i) ;
		power	= pow(2,i) ;
		// w_m.a 	= cos(2*M_PI/i) ; 
		// w_m.b 	= -sin(2*M_PI/i) ; 
		for( j = 0 ; j < power ; j++ ){
		//for( j = 0 ; j < length ; j=j+power ){
			//w.a = 1 ;
			//w.b = 0 ;
			for ( k = j*length/power ; k < (j+1)*length/power ; k +=  2){
			//for ( k = 0 ; k < power/2 ; k++){
				fprintf(debug,"%d\t",i);
				fprintf(debug,"%d\t",j);
				fprintf(debug,"%d\n",k);
				//p_array(debug,res,length);
				fprintf(debug,"k=%d\tk+1=%d\n",indexes[k],indexes[k+1]) ;
 				butterfly(&res[indexes[k]],&res[indexes[k+1]],,length/power) ;
				//p_array(debug,res,length);
				/*
				fprintf(debug,"%d\t",i);
				fprintf(debug,"%d\t",j);
				fprintf(debug,"%d\n",k);
				
				p_array(debug,res,length);
				fprintf(debug,"k=%d\tk+1=%d\n",indexes[j+k],indexes[j+k+power/2]) ;
				
				mult_aux = complex_mult(w, res[indexes[j+k+power/2]]) ;
 				butterfly(&res[indexes[k+j]],&mult_aux) ;
				res[indexes[j+k+power/2]] = mult_aux; 
				
				w = complex_mult(w, w_m) ;
				
				p_array(debug,res,length);
				*/
			}
	fprintf(debug,"-------------------------\n");
		} 
	fprintf(debug,">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
	}
	for ( i = 0 ; i < length/2 ; i++ ){
		fprintf(debug,"k=%d\tk+1=%d\n",i,i+length/2) ;
		butterfly(&res[i], &res[i+length/2]) ;		
	}	
}

void p_array(FILE* fd,complex_t* array,int size){
	int i = 0 ;
	for ( ; i < size ; i++ ){
			fprintf(fd,"%f ",norma(array[i])) ;
	}
	fprintf(fd,"\n") ;
}

void p_array_int (FILE* fd, int* array,int size){
	int i = 0 ;
	for ( ; i < size ; i++ ){
			fprintf(fd,"%d ",array[i]) ;
	}
			fprintf(fd,"%d ",array[i]) ;
}

float norma (complex_t c){
	return sqrt((c.a * c.a) + (c.b * c.b)) ;
} 
