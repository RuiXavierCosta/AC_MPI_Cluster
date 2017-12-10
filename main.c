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

    ImageF *mask;
    ImageF *imgin_real, *imgin_imag, *imgout_real, *imgout_imag;
    Image *imgin, *imgout;


    /**    
     * if (my_rank == 0)
     * {}
     **/
    imgin = loadPBM("teste.pgm");
    int in_rows = imgin->rows;
    int in_cols = imgin->cols;
    int in_step = imgin->widthStep;
    
    imgin_real = image_to_imagef(imgin);
    imgin_imag = gen_blank_imaginary(in_rows, in_cols);
    mask = genlpfmask(in_rows, in_cols);

    imgout_real=(ImageF*)malloc(sizeof(ImageF));
    imgout_real->cols=in_cols;
    imgout_real->rows=in_rows;
    imgout_real->widthStep=in_step;
    imgout_real->data=(double*)malloc(in_cols*in_rows);
    
    imgout_imag=(ImageF*)malloc(sizeof(ImageF));
    imgout_imag->cols=in_cols;
    imgout_imag->rows=in_rows;
    imgout_imag->widthStep=in_step;
    imgout_imag->data=(double*)malloc(in_cols*in_rows);
    
    dofilt(imgin_real, imgin_imag, mask, imgout_real, imgout_imag);
    imgout = imagef_to_image(imgout_real);
    savePBM("build/images/img.pbm", imgout);
}

    
