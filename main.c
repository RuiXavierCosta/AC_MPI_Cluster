#include <stdio.h>
#include <stdlib.h>
#include "pbm/funcs.h"

#define MPI_PARALLEL
#ifdef MPI_PARALLEL
#include <mpi.h>
#endif

/* message tags */
#define REQUEST     1
#define REPLY       2

int world_size, my_rank;
int elements_to_send;
int *elements_per_process;

MPI_Status status;

int main(int argc, char **argv){
    ImageF *mask;
    ImageF *imgin_real, *imgin_imag;
    Image *imgin, *imgout;

    imgin = loadPBM("teste.pgm");
    imgin_real = image_to_imagef(imgin);
    imgin_imag = gen_blank_imaginary(imgin_real->rows, imgin_real->rows);
    mask = genlpfmask(128, 128);


    imgout = imagef_to_image(imgin_real);
    savePBM("build/images/img.pbm", imgout);
}

    
