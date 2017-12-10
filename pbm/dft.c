#include "funcs.h"

void dft(ImageF *inreal , ImageF *inimag, ImageF *outreal, ImageF *outimag, int inverse) {
    double real_val;
    double imag_val;
    /**
     * Declarar o expoente da DFT caso seja transformada direta(!=0) ou inversa(=0)
     * */
    double exp;
    if (inverse == 0){
         exp = -2* PI;
        }
    else{
         exp = 2* PI;
    }
    
    /**
     * A transformada é calculada SEMPRE sobre as linhas
     **/
    int widthStep = inreal->widthStep;
    for (int l1 = 0; l1 < inreal->rows; l1++)
    {
        for (int c= 0; c< inreal->cols; c++)
        {
            int array_index = l1*widthStep+c;
            outreal->data[array_index] = 0.0;
            outimag->data[array_index] = 0.0;


            for(int l2=0; l2 < inreal->rows; l2++){
                int inner_index = l1*widthStep+l2;
                double angle = exp*c*l2/inreal->cols;
                outreal->data[array_index] += inreal->data[inner_index]*cos(angle) - inimag->data[inner_index]*sin(angle);
                outimag->data[array_index] += inimag->data[inner_index]*cos(angle) + inreal->data[inner_index]*sin(angle);
            }
            /**
             * Eu queria fazer esta função com parametros de entrada 
            *  ImageF
            * */
            real_val=outreal->data[array_index];
            imag_val=outimag->data[array_index];    
        }
    }
        
}

