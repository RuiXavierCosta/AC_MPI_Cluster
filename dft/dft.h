#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <complex.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>

#define UM_SEC 1000000000L
#define PI 3.14159265359
#define N 64

void dft(double *inreal , double *inimag, double *outreal, double *outimag, int n);