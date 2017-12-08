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
    Image imgout;

    mask = genlpfmask(128, 128);

    imgout.rows=128;
    imgout.cols=128;
    imgout.widthStep=128;
    imgout.data=(unsigned char *) malloc(imgout.rows *imgout.cols);
    // imgout.data=(unsigned char *) img->data;
    for (int i=0; i<imgout.rows; i++){      
        for(int j=0; j<imgout.cols; j++){
            imgout.data[imgout.widthStep*i+j] = (unsigned char)mask->data[imgout.widthStep*i+j];
        }      
    }
    savePBM("build/images/img.pbm",&imgout);
}

    
