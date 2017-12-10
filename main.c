#include <stdio.h>
#include <stdlib.h>
#include "pbm/funcs.h"
/**
 * #include "mpi.h"
 **/
#include <time.h>


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
    int rank = 0, size;

    // #ifdef MPI_PARALLEL
    //     MPI_Status *status;

    //     MPI_Init( &argc , &argv );
    //     MPI_Comm_rank(MPI_COMM_WORLD, &rank );
    //     MPI_Comm_size(MPI_COMM_WORLD, &size);

    //     status=(MPI_Status*) malloc(sizeof(MPI_Status));
    // #endif

    // printf("Running...\n");
    ImageF *mask;
    ImageF *imgin_real, *imgin_imag;
    Image *imgin, *imgout;

    /**    
     * if (my_rank == 0)
     * {}
     **/
    imgin = loadPBM("teste.pgm");
    
    imgin_real = image_to_imagef(imgin);
    imgin_imag = gen_blank_imaginary(imgin->rows, imgin->cols);
    mask = genlpfmask(imgin->rows, imgin->cols);
    free(imgin);

    ImageF *dft_real, *dft_imag;
    dft_real = malloc_imagef(imgin_real->rows, imgin_real->cols, imgin_real->widthStep);
    dft_imag = malloc_imagef(imgin_imag->rows, imgin_imag->cols, imgin_imag->widthStep);

    ImageF *filt_real, *filt_imag;
    filt_real = malloc_imagef(imgin_real->rows, imgin_real->cols, imgin_real->widthStep);
    filt_imag = malloc_imagef(imgin_imag->rows, imgin_imag->cols, imgin_imag->widthStep);

    ImageF *imgout_real, *imgout_imag;
    imgout_real = malloc_imagef(imgin_real->rows, imgin_real->cols, imgin_real->widthStep);
    imgout_imag = malloc_imagef(imgin_imag->rows, imgin_imag->cols, imgin_imag->widthStep);

    dft(imgin_real, imgin_imag, dft_real, dft_imag, false);
    // dofilt(dft_real, dft_imag, mask, filt_real, filt_imag);
    dft(dft_real, dft_imag, imgout_real, imgout_imag, true);
    
    free(imgin_real);
    free(imgin_imag);
    free(dft_real);
    free(dft_imag);

    // transpor_matriz(imgin_real);
    normalize_img(imgout_real);
    imgout = imagef_to_image(imgout_real);
    savePBM("build/images/img.pbm", imgout);
}

    
