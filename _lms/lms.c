/** LMS **/
#include <stdlib.h>
#include <stdio.h>
#define MAX_LEN 50 
#define FIL_LEN 30
#define PLAN_LEN 30
#define SAMPLE_ITERS 10000  

/** Funciones **/ 
int conv(double* res,double* f, int f_len, double* g, int g_len) ;
int lms(double* res,double* a,double* b) ; 


int main(void) {

	/** Defino los arreglos que intervienen en el proceso **/
	double entrada[MAX_LEN] = {0}; 
	double fad[FIL_LEN] = {0} ; 
	double sfad = 0 ; 
	double planta[PLAN_LEN] = {0} ;
	double splanta = 0 ;
	double mu = 1/(FIL_LEN*10) ; 
	double error;
	int i,j ;   

	for ( i = PLAN_LEN/2 - 5 ; i < PLAN_LEN/2 + 5 ; i++ ) 
		planta[i] = 1 ; 

	for( i = 0 ; i < SAMPLE_ITERS ; i++){	
		for( j = MAX_LEN ; j > 0  ; j-- ) 
			entrada[j] = entrada[j-1] ;

		entrada[0] = rand() ;
		/*for ( j = MAX_LEN - 1 ; j > 0 ; j-- ) {
			entrada[j-1] = entrada[j] ;
			if( j < PLAN_LEN ) 
				splanta += planta[j]*entrada[j] ; 
			if( j < FIL_LEN ) 	
				sfad += fad[j]*entrada[j] ; 
		}	*/


		for( j = 0 ; j < PLAN_LEN ; j++ ){
			splanta += planta[j]*entrada[j] ; 
		
		}

		for( j = 0 ; j < FIL_LEN ; j++ ){
			sfad += fad[j]*entrada[j] ; 
		
		}


		error = splanta - sfad ; 
		for ( j = 0 ; j < FIL_LEN ; j++ ) {
			fad[j] += mu*error*entrada[j] ;
		}			
	}
	
	puts("planta") ;
	for( i = 0 ; i < PLAN_LEN ; i++ ){
		printf("%1.1f, ",planta[i]) ; 
	}
	puts("") ;

	puts("filter") ;
	for( i = 0 ; i < FIL_LEN ; i++ ){
		printf("%1.6f, ",fad[i]) ; 
	}

	puts("") ;

	return 1 ; 
			
}
