#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define Q 16 
#define MAX_ITER 1000

/* PROTOTIPO */
double fir ( double* , double* , double ) ; 
double lms ( double* , double* , double, double, double ) ; 
void print_array (double*,int,char*) ;

int main(void){
	int i,j ; 
	double h[Q], w_fir[Q+1],  w_lms[Q+1], p[Q], z, mu;
	double x, y ;
 	for( i = 0 ; i < Q ; i++ ){
		h[i] = 0 ; 
		p[i] = 1 ;
		w_fir[i] = 0 ;  
		w_lms[i] = 0 ; 
		if( i < Q/2+5 && i > Q/2-5 ) p[i] = 2 ;
	} 
	
	//mu = 0.033333333333333333 ; 
	mu = (double) sqrt(2)/Q;
	printf("%f",mu);
	for ( i = 0  ; i < MAX_ITER ; i++) {
		x = sin(i*3.14/180) ;
		//x = (double)(rand()%100)/100 ;
		puts("--------------------PLANTA----------------") ;
		z = fir(p, w_fir, x);
		//printf("z_ant=%f\n",z) ;
		z += (double)rand()/RAND_MAX ;
		//printf("z_pos=%f\n",z) ;
		puts("--------------------FAD-----------------------");
		y = fir(h, w_lms, x); 
		printf("%s\t\t%s\t\t%s\n","mu","z-y","w_lms");
		for( j = Q-1 ; j >= 0 ; j-- ) {	
			h[j] += mu * (z - y) * w_lms[j+1] ;
			printf("%f\t%f\t%f\n",mu,z-y,w_lms[j+1]) ;
			
		}
	//	y =	lms(h,w_lms,x,z,mu) ;
	//	printf("%s\t\t%s\t\t%s\n","z","y","error");
	//	printf("%f\t%f\t%f\n",z,y,z-y) ;
	}

	printf("%s\t\t%s\t\t%s\t\t%s\n","planta","fad","w_fir","w_lms");
	for( i = 0 ; i < Q ; i++ ) {
		printf("%f\t%f\t%f\t%f\n",p[i],h[i],w_fir[i+1],w_lms[i+1]) ;
	}
		
	return 1 ; 
}

double fir( double * h, double * w, double x){
	double y ;
	int i ;

	w[0] = x ; 
	for( i = Q-1 ; i >= 0 ; i-- ){
		w[i+1] = w[i] ;
		printf( "%f * %f \n",h[i], w[i+1]) ;
		y += h[i] * w[i+1] ;
	}

	return y ;
}

double lms( double * h, double * w, double x, double z, double mu){
	double y ; 
	int i ; 

	w[0] = x ; 
	for( i = Q-1 ; i >= 0 ; i-- ){
		w[i+1] = w[i] ; 
		y += h[i] * w[i+1] ; 
	}

	for( i = Q-1 ; i >= 0 ; i-- )
		h[i] += mu * (z - y) * w[i+1] ;

	return y ; 
}

void print_array( double* array, int length, char* nombre){
	int i ;
	printf("%s = [ ", nombre)  ;
	for (i = 0 ; i < length ; i++ )
		printf("%.2f, ",array[i]);
	printf("]\n") ;	
}
