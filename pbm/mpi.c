#include "funcs.h"

#define MPI_PARALLEL
#ifdef MPI_PARALLEL
#include <mpi.h>
#endif

void send_image(ImageF * img, int cluster_size, int block_rows){
    printf("\nSending image...\n");
    int cols = img->cols;
    double *img_block = (double*)malloc(sizeof(double)*cols*block_rows);

    int img_index = 0;
    for( int rank = 1 ; rank < cluster_size ; rank++){
        for(int i = 0; i < block_rows; i++){
            for(int j=0; j < cols; j++){
                img_index = (rank - 1 + i) * cols + j;
                img_block[i*cols + j] = img->data[img_index];
            }
        }        
        MPI_Send(img_block, sizeof(double)*cols, MPI_DOUBLE, rank, IMAGE_TAG, MPI_COMM_WORLD);
    }
}

ImageF *receive_image_block(int block_rows, int cols, int widthStep, int rank){
    printf("\r Receiving image block...\n");
    ImageF *out = malloc_imagef(block_rows, cols, widthStep);
    double *buffer = (double*)malloc(sizeof(double)*cols*block_rows);

    MPI_Recv(
        buffer, 
        sizeof(double)*cols, 
        MPI_DOUBLE, 
        0, 
        IMAGE_BLOCK_TAG, 
        MPI_COMM_WORLD, 
        MPI_STATUS_IGNORE
    );
    out->data = buffer;
    return out;
}

void send_image_block(ImageF * block, int rank){
    printf("\r Sending image block...\n");
    int block_size = block->cols * block->rows;

    MPI_Send(block, sizeof(double)* block->cols , MPI_DOUBLE, 0, IMAGE_BLOCK_TAG, MPI_COMM_WORLD);
}

ImageF *receive_image(int rows, int cols, int widthStep, int cluster_size){
    printf("\r Receiving image...\n");
    ImageF *out = malloc_imagef(rows, cols, widthStep);
    int block_size = floor(rows / (cluster_size - 1));
    double *buffer = (double*)malloc(sizeof(double)*cols*block_size);

    for(int rank = 1; rank < cluster_size; rank++){
        int array_index = (rank - 1) * cluster_size;
        MPI_Recv(
            buffer, 
            cols * sizeof(double), 
            MPI_DOUBLE, 
            0, 
            IMAGE_TAG, 
            MPI_COMM_WORLD, 
            MPI_STATUS_IGNORE
        );

        // printf("\n dasse out-%d %d da\n", out->data + array_index), *buffer);
        *(out->data + array_index) = *buffer;
    }

    return out;
}