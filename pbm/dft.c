#include "funcs.h"


void dft(ImageF *inreal , ImageF *inimag, ImageF *outreal, ImageF *outimag, int n) {
    double *real = inreal->data;
    double *imag = inimag->data;

    for (int k = 0; k < n; k++) {
        double sumreal = 0;
        double sumimag = 0;
        for (int t = 0; t < n; t++) {
            double angle = 2 * PI * t * k / n;
            sumreal +=  real[t] * cos(angle) + imag[t] * sin(angle);
            sumimag += -real[t] * sin(angle) + imag[t] * cos(angle);
        }
        outreal->data[k] = sumreal;
        outimag->data[k] = sumimag;
    }
}