/** 
  * FFT PARA BOLUDOS
  *
  */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define SAMPLE_LENGTH 1024

typedef struct{
	float a, b;
} complex_t;

/** Prototipos **/

void index_generator(int,int*) ;
unsigned int reverse(unsigned int,int) ; 
void butterfly(complex_t*, complex_t*, complex_t) ;
void fft(complex_t*,int) ;
float norma (complex_t c) ;
void p_array(FILE*,complex_t* array,int size);
void p_array_int(FILE*,int* array,int size);
void p_array_float(FILE*,float* array,int size);
void p_array_complex(FILE* fd,complex_t* array,int size);
complex_t complex_add(complex_t, complex_t);
complex_t complex_sub(complex_t, complex_t);
complex_t complex_mult(complex_t x, complex_t y);
void print_complex (FILE* fd,complex_t z);
FILE* out ;
FILE* debug ;
int main(int argc,char** argv){
	int i ;
	int length = SAMPLE_LENGTH ;
	float * signal_samples ;
	complex_t * muestras ;
	FILE* signal ;
	float f = 100;
	float T = 0.001;
	debug = fopen("./debug","w+");
	out = fopen("./out","w+");
	signal = fopen("./signal","w+");

	if( argc > 1 ){
		length = atoi(argv[1]);
	} 

	signal_samples = (float*) malloc(sizeof(float)*length) ;
	muestras = (complex_t *) malloc(sizeof(complex_t)*length) ;

	for ( i = 0 ; i < length ; i++ ){
	//	if( i+1 == length/2 ) signal_samples[i] = 1 ;
	//	else	signal_samples[i] = sin(M_PI*8*(i+1-length/2)/length)/(M_PI*8*(i+1-length/2)/length) ;
		signal_samples[i] = sin(M_PI*2*i*f/(float)length)+0.5*sin(M_PI*4*i*f/(float)length)+0.25*sin(M_PI*8*i*f/(float)length);
		muestras[i].a = signal_samples[i] ; 
		muestras[i].b = 0 ;
	}
	p_array_float(signal,signal_samples,length) ;
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

void print_complex (FILE* fd,complex_t z){
	fprintf(fd,"%f+i%f\n",z.a,z.b);
}

void butterfly(complex_t* a0, complex_t* a1, complex_t W){
	complex_t mult_aux ;
	complex_t aux = *a0 ;

	mult_aux = complex_mult(*a1, W) ;

	*a0 = complex_add(*a0, mult_aux) ;
	*a1 = complex_sub(aux, mult_aux) ;
}

void fft(complex_t* res,int length){
	int i,j,k,power;
	char niveles = log2(length) ; 
	int * indexes = (int *) malloc(sizeof(int)*length) ;
	complex_t W, aux, z,w; 
	index_generator(length,indexes) ; 
	p_array_int(debug,indexes,length);
	for ( i = 1 ; i <= niveles ; i++ ){
		fprintf(debug,"nivel%d------------------------\n",i);
		power	= pow(2,i) ;
		W.a = cos((float)2*M_PI/power) ;
		fprintf(debug,"W.a=%f,",W.a);
		W.b = -sin((float)2*M_PI/power) ;
		fprintf(debug,"W.b=%f\n",W.b);
		for( j = 0 ; j < length ; j+=power ){
				aux.a = 1 ; 
				aux.b = 0 ;
			for( k = 0 ; k < power/2 ; k++ ) { 
				fprintf(debug,"up=%d\tdown=%d\t,order=%d\t,n=%d\n",indexes[j+k],indexes[j+k+power/2],k*length/power,length) ;
 				//butterfly(&res[indexes[j+k]],&res[indexes[j+k+power/2]],W) ;
				
				z = complex_mult(aux,res[indexes[j+k+power/2]]) ;
				w = res[indexes[j+k]] ;
				res[indexes[j+k]] = complex_add(z,w) ;
				fprintf(debug,"res[%d]=w(%.2f+%.2fi) + W(%.2f+%.2fi)*res[%d](%.2f+%.2fi) = %.2f+%.2fi\n",
					indexes[j+k],w.a,w.b,W.a,W.b,indexes[j+k+power/2],res[indexes[j+k+power/2]].a,res[indexes[j+k+power/2]].b,
					res[indexes[j+k]].a,res[indexes[j+k]].b );
				res[indexes[j+k+power/2]] = complex_sub(w,z) ;
				fprintf(debug,"res[%d]=w(%.2f+%.2fi) - z(%.2f+%.2fi) = %.2f+%.2fi\n",
					indexes[j+k+power/2],w.a,w.b,z.a,z.b,res[indexes[j+k+power/2]].a,res[indexes[j+k+power/2]].b ) ;
				aux = complex_mult(aux,W) ;
				p_array_complex(debug,res,length);
			}
		} 
	}
}

void p_array(FILE* fd,complex_t* array,int size){
	int i = 0 ;
	for ( ; i < size ; i++ ){
			fprintf(fd,"%f ",norma(array[i])) ;
	}
	fprintf(fd,"\n") ;
}

void p_array_complex(FILE* fd,complex_t* array,int size){
	int i = 0 ;
	for ( ; i < size ; i++ ){
			fprintf(fd,"%.2f+%.2fi ",array[i].a,array[i].b) ;
	}
	fprintf(fd,"\n") ;
}


void p_array_int (FILE* fd, int* array,int size){
	int i = 0 ;
	for ( ; i < size ; i++ ){
			fprintf(fd,"%d ",array[i]) ;
	}
	fprintf(fd,"\n") ;
}

void p_array_float (FILE* fd, float* array,int size){
	int i = 0 ;
	for ( ; i < size ; i++ ){
			fprintf(fd,"%f ",array[i]) ;
	}
	fprintf(fd,"\n") ;
}

float norma (complex_t c){
	return sqrt((c.a * c.a) + (c.b * c.b)) ;
} 
