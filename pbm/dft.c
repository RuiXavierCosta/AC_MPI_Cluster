#include "funcs.h"

void dft_linha_a_linha(ImageF *inreal , ImageF *inimag, ImageF *outreal, ImageF *outimag, int row, bool inverse) {
    /**
     * Declarar o expoente da DFT caso seja transformada inversa == true ou direta == false
     * */
    double exp;  
    if (inverse == true){
        exp = 2.0 * PI;
    }
    else{
        exp = -2.0 * PI;
    }

    /**
     * A transformada é calculada SEMPRE sobre as linhas
     **/
    int N = inreal->cols;
    int L = outreal->cols;
    for (int l = 0; l < L; l++)
    {
        double real_val = 0.0;
        double imag_val = 0.0;

        // Inicio de DFT nas linhas neste index (k,l)
        for(int n = 0; n < N; n++)
        {
            double angle = exp * l * n / N;
            int array_index = row * L + n;
            real_val += inreal->data[array_index] * cos(angle) + inimag->data[array_index] * sin(angle);
            imag_val += - inreal->data[array_index] * sin(angle) + inimag->data[array_index] * cos(angle);
        } 

        // Fim de DFT nas linhas neste index (k,l)
        outreal->data[row * L + l] = real_val;
        outimag->data[row * L + l] = imag_val;
    }
}

void dft(ImageF *inreal , ImageF *inimag, ImageF *outreal, ImageF *outimag, bool inverse) {
    ImageF *auxreal = malloc_imagef(inreal->rows, inreal->cols, inreal->widthStep);
    ImageF *auximag = malloc_imagef(inimag->rows, inimag->cols, inimag->widthStep);

    for(int m = 0; m < inreal->rows; m++){
        dft_linha_a_linha(inreal, inimag, auxreal, auximag, m, false);
        // dft_linha_a_linha(inreal , inimag, outreal, outimag, m, false);
    }

    transpor_matriz(auxreal);
    transpor_matriz(auximag);
    transpor_matriz(outreal);
    transpor_matriz(outimag);

    for(int m = 0; m < auxreal->rows; m++){
        dft_linha_a_linha(auxreal, auximag, outreal, outimag, m, false);
    }

    transpor_matriz(outreal);
    transpor_matriz(outimag);
}