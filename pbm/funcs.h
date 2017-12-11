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
#define IMAGE_BLOCK_TAG     0
#define IMAGE_TAG           2
#define COLS_TAG            3
#define ROWS_TAG            4
#define BLOCK_SIZE_TAG      5

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
ImageF * gen_blank_imaginary(int rows, int cols);
ImageF *malloc_imagef(int rows, int cols, int widthStep);

ImageF * image_to_imagef(Image *);
Image * imagef_to_image(ImageF *);

/**
 * MULTIPLICA A MATRIZ PELA MASCARA
 * */
void dofilt(ImageF * , ImageF * , ImageF * , ImageF * , ImageF * );
void dft(ImageF *inreal , ImageF *inimag, ImageF *outreal, ImageF *outimag, bool inverse) ;
struct timespec SubtracaoTempo(struct timespec Inicio, struct timespec Fim);

void teste(ImageF * , ImageF *);

void transpor_matriz(ImageF *M);

void normalize_img(ImageF * );

void send_image(ImageF * img, int cluster_size, int *block_sizes);

ImageF *receive_image_block(int *block_sizes, int cols, int widthStep, int rank);

void send_image_block(ImageF * block, int rank);

ImageF *receive_image(int rows, int cols, int widthStep, int *block_sizes, int cluster_size);
