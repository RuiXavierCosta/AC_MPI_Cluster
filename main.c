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
    ImageF *mask;
    Image *imgin_real, *imgin_imag;
    Image imgout;

    imgin_real = loadPBM("teste.pgm");
    imgin_imag = gen_blank_imaginary(imgin_real->rows, imgin_real->rows);
    mask = genlpfmask(128, 128);


    imgout.rows=imgin_real->rows;
    imgout.cols=imgin_real->cols;
    imgout.widthStep=imgin_real->widthStep;
    imgout.data=(unsigned char *) malloc(imgout.rows *imgout.cols*sizeof(unsigned char*));
    // imgout.data=(unsigned char *) img->data;
    for (int i=0; i<imgout.rows; i++){      
        for(int j=0; j<imgout.cols; j++){
            imgout.data[imgout.widthStep*i+j] = imgin_real->data[imgout.widthStep*i+j];
        }      
    }
    savePBM("build/images/img.pbm",&imgout);
}

    
