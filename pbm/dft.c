#include "funcs.h"

/**
 * passei o n para inverse. 
 * */
void dft(double *inreal , double *inimag, double *outreal, double *outimag, int inverse, int cols, int rows) {
   
/**
 * Declarar o expoente da DFT caso seja transformada direta(!=0) ou inversa(=0)
 * */
    
    if (inverse == 0){
        double exp = -2* PI;
        }
    else{
        double exp = 2* PI;
    }
    
/**
 * A transformada é calculada SEMPRE sobre as linhas
 **/

for (int l1 = 0; l1 < rows; l1++)
{
    for (int c= 0; c< cols; c++)
    {
        outreal[l1*cols+c] = 0.0;
	    outimag[l1*cols+c] = 0.0;


        for(int l2=0; l2 < rows; l2++){
            outreal[l1*cols+c] += inreal[l1*cols+l2]*cos(exp*c*l2/cols) - inimag[l1*cols+l2]*sin( exp * c*l2/cols);
            outimag[l1*cols+c] += inimag[l1*cols+l2]*cos(exp*c*l2/cols) + inreal[l1*cols+l2]*sin( exp *c*l2/cols);
        }
    }
}
        
}