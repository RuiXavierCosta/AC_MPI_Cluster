#include "dft.h"


void dft(double *inreal , double *inimag, double *outreal, double *outimag, int n) {

    for (int k = 0; k < n; k++) {
        double sumreal = 0;
        double sumimag = 0;
        for (int t = 0; t < n; t++) {
            double angle = 2 * PI * t * k / n;
            sumreal +=  inreal[t] * cos(angle) + inimag[t] * sin(angle);
            sumimag += -inreal[t] * sin(angle) + inimag[t] * cos(angle);
        }
        outreal[k] = sumreal;
        outimag[k] = sumimag;
    }
}