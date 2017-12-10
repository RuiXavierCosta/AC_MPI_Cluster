#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <complex.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>

#define UM_SEC 1000000000L
#define PI 3.14159265359
#define MAX 5

struct Matrix{
  int rows;
  int cols;
  unsigned char * data;
  int widthStep;
};
typedef struct Matrix Image; 

struct MatrixF{
  int rows;
  int cols;
  double * data;
  int widthStep;
};
typedef struct MatrixF ImageF;

Image * loadPBM(char * fname);
void savePBM(char * fname, Image * image);

/* coloque aqui a seguir adeclaração das funções a desenvolver */
ImageF * genlpfmask(int , int);

ImageF * image_to_imagef(Image *);
Image * imagef_to_image(ImageF *);
/**
 * MULTIPLICA A MATRIZ PELA MASCARA
 * */
void dofilt(ImageF * , ImageF * , ImageF * , ImageF * , ImageF * );
void dft(ImageF *inreal , ImageF *inimag, ImageF *outreal, ImageF *outimag, int inverse) ;
struct timespec SubtracaoTempo(struct timespec Inicio, struct timespec Fim);

void teste(ImageF * , ImageF *);

ImageF * gen_blank_imaginary(int rows, int cols);
void transpor_matriz(ImageF *matriz_re, ImageF *matriz_img);
