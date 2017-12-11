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

int world_size, my_rank;
int elements_to_send;
int *elements_per_process;

int main(int argc, char **argv){
    int rank = 0, size;

    #ifdef MPI_PARALLEL
        MPI_Status *status;

        MPI_Init( &argc , &argv );
        MPI_Comm_rank(MPI_COMM_WORLD, &rank );
        MPI_Comm_size(MPI_COMM_WORLD, &size);

        status=(MPI_Status*) malloc(sizeof(MPI_Status));
    #endif

    // printf("Running...\n");
    ImageF *mask;
    ImageF *imgin_real, *imgin_imag;
    Image *imgin, *imgout;
    int cols, rows, *block_sizes;
    
    /**
     * Carregamento de imagem e
     * envio de informacao da imagem de entrada.
     **/
    if (rank == 0)
    {        
        imgin = loadPBM("teste.pgm");
        cols = imgin->cols;
        rows = imgin->rows;
        block_sizes = (int*)malloc(sizeof(int) * size);
        block_sizes[0]= 0;

        printf("Sending block info\n");
        for(int i = 1; i < size; i++){
            if(i == size - 1){
                block_sizes[i] = imgin->rows  - (size - 2) * floor(imgin->rows / (size - 1));
            } else {
                block_sizes[i] = floor(imgin->rows / (size - 1));
            }
        } 
        for( int rank = 1 ; rank < size ; rank++){  
            MPI_Send((void*)&cols, 1, MPI_INT, rank, COLS_TAG, MPI_COMM_WORLD);
            MPI_Send((void*)&rows, 1, MPI_INT, rank, ROWS_TAG, MPI_COMM_WORLD);
            MPI_Send((void*)block_sizes, size, MPI_INT, rank, BLOCK_SIZE_TAG, MPI_COMM_WORLD);
        }
    } else {
        block_sizes = (int*)malloc(sizeof(int)*size);
        printf("Receiving block info\n");
        MPI_Recv((void*)&cols, 1, MPI_INT, 0, COLS_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv((void*)&rows, 1, MPI_INT, 0, ROWS_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv((void*)block_sizes, size, MPI_INT, 0, BLOCK_SIZE_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    /**
     * Primeiro envio de imagem.
     * Cada proc faz dft do seu bloco, transpoem
     * e devolve o bloco transposto
     **/
    if(rank == 0){
        imgin_real = image_to_imagef(imgin);
        send_image(imgin_real, size, block_sizes);
    } else {

        ImageF *block_real, *block_imag;
        block_imag = gen_blank_imaginary(block_sizes[rank], cols);
        block_real = malloc_imagef(block_sizes[rank], cols, cols);

        block_real = receive_image_block(block_sizes, cols, cols, rank);
        send_image_block(block_real, rank);
        // ImageF *dft_real, *dft_imag;
        // dft_real = malloc_imagef(block_size, row_size, row_size);
        // dft_imag = malloc_imagef(block_size, row_size, row_size);

        // dft_and_tranpose();
        free(block_real);
        free(block_imag);
    }

    if (rank == 0)
    {    
        // ImageF *filt_real, *filt_imag;
        // filt_real = malloc_imagef(imgin_real->rows, imgin_real->cols, imgin_real->widthStep);
        // filt_imag = malloc_imagef(imgin_imag->rows, imgin_imag->cols, imgin_imag->widthStep);

        ImageF *imgout_real, *imgout_imag;
        imgout_real = malloc_imagef(imgin->rows, imgin->cols, imgin->widthStep);
        imgout_imag = malloc_imagef(imgin->rows, imgin->cols, imgin->widthStep);

        // mask = genlpfmask(imgin->rows, imgin->cols);
        // dft(imgin_real, imgin_imag, dft_real, dft_imag, false);
        // dofilt(dft_real, dft_imag, mask, filt_real, filt_imag);
        // dft(filt_real, filt_imag, imgout_real, imgout_imag, true);

        // free(dft_real);
        // free(dft_imag);

        // // transpor_matriz(imgin_real);
        // normalize_img(imgout_real);
        // imgout = imagef_to_image(imgout_real);
        // savePBM("build/images/filtered.pbm", imgout);

        imgout_real = receive_image(imgin->rows, imgin->cols, imgin->widthStep, block_sizes, size);
        imgout = imagef_to_image(imgin_real);
        savePBM("build/images/original.pbm", imgout);
    }
    // free(imgin);
    // free(imgin_real);
    // free(imgin_imag);

	MPI_Finalize();
    return 0;
}

    
