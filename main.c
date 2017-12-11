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

#define SEND_IN_IMG_ID 0
#define FIRST_DFT_REAL_ID 1
#define FIRST_DFT_IMAG_ID 2
#define FIRST_TRANSPOSE_REAL_ID 3
#define FIRST_TRANSPOSE_IMAG_ID 4
#define SECOND_DFT_REAL_ID 5
#define SECOND_DFT_IMAG_ID 6
#define AFTER_FILTER_REAL_ID 7
#define AFTER_FILTER_IMAG_ID 8
#define THIRD_DFT_REAL_ID 9
#define THIRD_DFT_IMAG_ID 10
#define SECOND_TRANSPOSE_REAL_ID 11
#define SECOND_TRANSPOSE_IMAG_ID 12
#define FOURTH_DFT_REAL_ID 13
#define FOURTH_DFT_IMAG_ID 14

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
    ImageF *block_real, *block_imag;
    ImageF *dft_real, *dft_imag;

    if(rank == 0){
        imgin_real = image_to_imagef(imgin);
        send_image(imgin_real, size, block_sizes, SEND_IN_IMG_ID);
    } else {

        block_imag = gen_blank_imaginary(block_sizes[rank], cols);
        block_real = malloc_imagef(block_sizes[rank], cols, cols);

        block_real = receive_image_block(block_sizes, cols, cols, rank, SEND_IN_IMG_ID);

        dft_real = malloc_imagef(cols, block_sizes[rank], block_sizes[rank]);
        dft_imag = malloc_imagef(cols, block_sizes[rank], block_sizes[rank]);

    // PRIMEIRA DFT
        printf("\n\nPRIMEIRA DFT PRIMEIRA DFT PRIMEIRA DFT\n\n");
        dft_and_transpose(block_real , block_imag, dft_real, dft_imag, false);
    // ENVIA PRIMEIRA DFT REAL
        send_image_block(dft_real, rank, FIRST_DFT_REAL_ID);
    }


    ImageF *filt_real, *filt_imag;
    ImageF *imgout_real, *imgout_imag;
    if (rank == 0){
    // RECEBE PRIMEIRA DFT REAL
        filt_real = malloc_imagef(cols, rows, rows);
        filt_real = receive_image(cols, rows, rows, block_sizes, size, FIRST_DFT_REAL_ID);
    } else {
    // ENVIA PRIMEIRA DFT IMAG
        send_image_block(dft_imag, rank, FIRST_DFT_IMAG_ID);
    }

    if (rank == 0){
    // RECEBE PRIMEIRA DFT IMAG
        filt_imag = malloc_imagef(cols, rows, rows);
        filt_imag = receive_image(cols, rows, rows, block_sizes, size, FIRST_DFT_IMAG_ID);

    // ENVIA PRIMEIRA TRASPOSTA REAL
        send_image(filt_real, size, block_sizes, FIRST_TRANSPOSE_REAL_ID);
    } else {
    // RECEBEU TRANSPOSTA REAL PARA REPETIR DFT PRIMEIRA
        block_real = malloc_imagef(block_sizes[rank], cols, cols);
        block_real = receive_image_block(block_sizes, cols, cols, rank, FIRST_TRANSPOSE_REAL_ID);
    }

    if (rank == 0){
        send_image(filt_imag, size, block_sizes, FIRST_TRANSPOSE_IMAG_ID);
    } else {
    // RECEBEU TRANSPOSTA IMAG PARA REPETIR DFT PRIMEIRA
        block_imag = malloc_imagef(block_sizes[rank], cols, cols);
        block_imag = receive_image_block(block_sizes, cols, cols, rank, FIRST_TRANSPOSE_IMAG_ID);
        
        dft_real = malloc_imagef(cols, block_sizes[rank], block_sizes[rank]);
        dft_imag = malloc_imagef(cols, block_sizes[rank], block_sizes[rank]);

    // SEGUNDA DFT
        printf("\n\nSEGUNDA DFT PRIMEIRA DFT PRIMEIRA DFT\n\n");
        dft_and_transpose(block_real , block_imag, dft_real, dft_imag, false);
        send_image_block(dft_real, rank, SECOND_DFT_REAL_ID);
    }

    if (rank == 0){
    // RECEBEU DFT REAL PARA FILTRAR
        filt_real = malloc_imagef(cols, rows, rows);
        filt_real = receive_image(cols, rows, rows, block_sizes, size, SECOND_DFT_REAL_ID);
    } else {
        send_image_block(dft_imag, rank, SECOND_DFT_IMAG_ID);
    }


    ImageF *output_real, *output_imag;
    if (rank == 0){
    // RECEBEU DFT IMAG PARA FILTRAR
        filt_imag = malloc_imagef(cols, rows, rows);
        filt_imag = receive_image(cols, rows, rows, block_sizes, size, SECOND_DFT_REAL_ID);

        output_real = malloc_imagef(rows, cols, cols);
        output_imag = malloc_imagef(rows, cols, cols);
        mask = genlpfmask(rows, cols);

    // FILTROU IMAGEM
        dofilt(filt_real, filt_imag, mask, output_real, output_imag);

    // ENVIA REAL PARA TERCEIRA DFT
        send_image(filt_real, size, block_sizes, AFTER_FILTER_REAL_ID);
    } else {
    // RECEBE REAL PARA TERCEIRA DFT
        block_real = malloc_imagef(block_sizes[rank], cols, cols);
        block_real = receive_image_block(block_sizes, cols, cols, rank, AFTER_FILTER_REAL_ID);
    }

    if (rank == 0){
    // ENVIA IMAG PARA TERCEIRA DFT
        send_image(filt_imag, size, block_sizes, AFTER_FILTER_IMAG_ID);
    } else {
    // RECEBE IMAG PARA TERCEIRA DFT
        block_imag = malloc_imagef(block_sizes[rank], cols, cols);
        block_imag = receive_image_block(block_sizes, cols, cols, rank, AFTER_FILTER_IMAG_ID);

    // TERCEIRA DFT
        printf("\n\nTERCEIRA DFT PRIMEIRA DFT PRIMEIRA DFT\n\n");
        dft_and_transpose(block_real , block_imag, dft_real, dft_imag, false);
        send_image_block(dft_real, rank, THIRD_DFT_REAL_ID);
    }

    if (rank == 0){
    // RECEBE TERCEIRA DFT REAL
        filt_real = malloc_imagef(cols, rows, rows);
        filt_real = receive_image(cols, rows, rows, block_sizes, size, THIRD_DFT_REAL_ID);
    } else {
        send_image_block(filt_imag, rank, THIRD_DFT_IMAG_ID);
    }

    if (rank == 0){
    // RECEBE TERCEIRA DFT IMAG
        filt_imag = malloc_imagef(cols, rows, rows);
        filt_imag = receive_image(cols, rows, rows, block_sizes, size, THIRD_DFT_IMAG_ID);

    // ENVIA SEGUNDA TRASPOSTA REAL
        send_image(filt_real, size, block_sizes, SECOND_TRANSPOSE_REAL_ID);
    } else {
    // RECEBEU TRANSPOSTA REAL PARA REPETIR DFT QUARTA
        block_real = malloc_imagef(block_sizes[rank], cols, cols);
        block_real = receive_image_block(block_sizes, cols, cols, rank, SECOND_TRANSPOSE_REAL_ID);
    }

    if (rank == 0){
        send_image(filt_imag, size, block_sizes, SECOND_TRANSPOSE_IMAG_ID);
    } else {
    // RECEBEU TRANSPOSTA IMAG PARA QUARTA DFT
        block_imag = malloc_imagef(block_sizes[rank], cols, cols);
        block_imag = receive_image_block(block_sizes, cols, cols, rank, SECOND_TRANSPOSE_IMAG_ID);
        
        dft_real = malloc_imagef(cols, block_sizes[rank], block_sizes[rank]);
        dft_imag = malloc_imagef(cols, block_sizes[rank], block_sizes[rank]);

    // QUARTA DFT
        printf("\n\nQUARTA DFT PRIMEIRA DFT PRIMEIRA DFT\n\n");
        dft_and_transpose(block_real , block_imag, dft_real, dft_imag, true);
        send_image_block(dft_real, rank, FOURTH_DFT_REAL_ID);
    }

    if (rank == 0 ){
        // RECEBE QUARTA DFT REAL
        filt_real = malloc_imagef(cols, rows, rows);
        filt_real = receive_image(cols, rows, rows, block_sizes, size, FOURTH_DFT_REAL_ID);

        normalize_img(filt_real);
        imgout = imagef_to_image(filt_real);
        savePBM("build/images/filtered.pbm", imgout);
    }
    free(imgin);
    free(imgin_real);
    free(imgin_imag);

	MPI_Finalize();
    return 0;
}

    
