#include "funcs.h"

#define MPI_PARALLEL
#ifdef MPI_PARALLEL
#include <mpi.h>
#endif

void send_image(ImageF * img, int cluster_size, int *block_sizes){
    printf("\nR0 Sending image...\n");
    int cols = img->cols;
    double *img_block;

    int img_index = 0;
    for( int rank = 1 ; rank < cluster_size ; rank++){
        img_block = (double*)malloc(sizeof(double)*cols*block_sizes[rank]);

        for(int i = 0; i < block_sizes[rank]; i++){
            for(int j=0; j < cols; j++){
                img_index = (rank - 1 + i) * cols + j;
                img_block[i*cols + j] = img->data[img_index];
            }
        }        
        MPI_Send(img_block, cols*block_sizes[rank], MPI_DOUBLE, rank, IMAGE_TAG, MPI_COMM_WORLD);
    }
}

ImageF *receive_image_block(int *block_sizes, int cols, int widthStep, int rank){
    printf("\nR%d Receiving image block...\n", rank);
    int block_rows = block_sizes[rank];
    ImageF *out = malloc_imagef(block_rows, cols, widthStep);
    double *buffer = (double*)malloc(sizeof(double)*cols*block_rows);

    MPI_Recv(
        buffer, 
        cols*block_rows, 
        MPI_DOUBLE, 
        0, 
        IMAGE_TAG, 
        MPI_COMM_WORLD, 
        MPI_STATUS_IGNORE
    );
    out->data = buffer;
    return out;
}

void send_image_block(ImageF * block, int rank){
    printf("\rR%d Sending image block...\n",rank);
    int block_size = block->cols * block->rows;

    MPI_Send(block, block->cols*block->rows , MPI_DOUBLE, 0, IMAGE_TAG, MPI_COMM_WORLD);
}

ImageF *receive_image(int rows, int cols, int widthStep, int *block_sizes, int cluster_size){
    printf("\rR0 Receiving image...\n");
    ImageF *out = malloc_imagef(rows, cols, widthStep);

    for(int rank = 1; rank < cluster_size; rank++){
        double *buffer = (double*)malloc(sizeof(double)*cols*block_sizes[rank]);
        int array_index = (rank - 1) * cluster_size;
        MPI_Recv(
            buffer, 
            cols*block_sizes[rank], 
            MPI_DOUBLE, 
            rank, 
            IMAGE_TAG, 
            MPI_COMM_WORLD, 
            MPI_STATUS_IGNORE
        );
        
        for(int i = 0; i < block_sizes[rank]; i++){
            for(int j = 0; j < cols; j++){
                out->data[(array_index + i)*out->widthStep + j] = buffer[i*cols + j];
            }
        }
    }
    return out;
}