
#ifdef __APPLE__
#include <netpbm/pam.h>
#else
#include <pam.h>
#endif
#include "funcs.h"

ImageF * gen_blank_imaginary(int rows, int cols){
  ImageF *out = (ImageF *)malloc(sizeof(ImageF));
  out->rows = rows;
  out->cols = cols;
  out->data = (double*)malloc(sizeof(double) * rows * cols);
  out->widthStep = cols;

  for (int i=0; i<out->rows; i++){      
    for(int j=0; j<out->cols; j++){
      out->data[out->widthStep*i+j]= 0.0;
    }      
  }

  return out;
}

ImageF *malloc_imagef(int rows, int cols, int widthStep){
    ImageF *out = (ImageF*)malloc(sizeof(ImageF));
    out->rows=rows;
    out->cols=cols;
    out->widthStep=widthStep;
    out->data=(double*)malloc(out->rows*out->cols*sizeof(double));
    return out;
}

ImageF *image_to_imagef(Image *in){
  ImageF *out = (ImageF *)malloc(sizeof(ImageF));
  out->rows = in->rows;
  out->cols = in->cols;
  out->data = (double*)malloc(sizeof(double) * out->rows * out->cols);
  out->widthStep = in->widthStep;

  for (int i=0; i<out->rows; i++){      
    for(int j=0; j<out->cols; j++){
      out->data[out->widthStep*i+j]= (double)in->data[out->widthStep*i+j];
    }      
  }

  return out;
}

Image *imagef_to_image(ImageF *in){
  Image *out = (Image *)malloc(sizeof(Image));
  out->rows = in->rows;
  out->cols = in->cols;
  out->data = (unsigned char*)malloc(sizeof(unsigned char) * out->rows * out->cols);
  out->widthStep = in->widthStep;

  for (int i=0; i<out->rows; i++){      
    for(int j=0; j<out->cols; j++){
      out->data[out->widthStep*i+j]= (unsigned char)in->data[out->widthStep*i+j];
    }      
  }

  return out;
}

void dofilt(ImageF *in_real, ImageF *in_imag, ImageF *mask, ImageF *out_real, ImageF *out_imag){
  double real_val;
  double imag_val;

  for(int i = 0; i < in_real->rows; i++){
    for(int j = 0; j < in_real->cols; j++){
      real_val = in_real->data[i*in_real->widthStep + j] * mask->data[i*mask-> widthStep + j];
      imag_val = in_imag->data[i*in_imag->widthStep + j] * mask->data[i*mask-> widthStep + j];
      if(real_val > 255.0) real_val=255.0;
      if(imag_val > 255.0) imag_val=255.0;
      
      out_real->data[i*in_real->widthStep + j] = real_val;
      out_imag->data[i*in_imag->widthStep + j] = imag_val;
    }
  }
}

double get_mask_pixel_value(int i, int j, int rows, int cols){
  double min = 0.0;
  double max = 255.0;
  double left_bondary = (cols/4);
  double right_bondary = 3 * (cols/4);
  double top_bondary = (rows/4);
  double bottom_bondary = 3 * (rows/4);

  // i = rows(vert), j = cols(horiz)
  bool left_top_corner = i <= top_bondary && j <= left_bondary;
  bool left_bottom_corner = i > bottom_bondary && j <= left_bondary;
  bool right_top_corner = i <= top_bondary && j > right_bondary;
  bool right_bottom_corner = i > bottom_bondary && j > right_bondary;

  if (left_top_corner || left_bottom_corner || right_top_corner || right_bottom_corner) {
    return max;
  } else {
    return min;
  }
}

ImageF * genlpfmask(int rows, int cols){
  ImageF *out = (ImageF *)malloc(sizeof(ImageF));
  out->rows = rows;
  out->cols = cols;
  out->data = (double*)malloc(sizeof(double) * rows * cols);
  out->widthStep = cols;

  for (int i=0; i<out->rows; i++){      
    for(int j=0; j<out->cols; j++){
      out->data[out->widthStep*i+j]= get_mask_pixel_value(i, j, rows, cols);
    }      
  }

  return out;
}

void teste(ImageF * in, ImageF * out){
    int i,j;
 for (i=0;i<out->rows;i++){
      // ponteiros para linha i de cada imagem
      double * row1=(in->data)+i*in->cols;
      double * row2=(out->data)+i*out->cols;
      
      for(j=0;j<out->cols;j++){	  
	  row2[j]=row1[j]+sin(j/24.0*M_PI)*50.0;
      }      
  }
}

void transpor_matriz(ImageF *M)
{
	
	ImageF *A = malloc_imagef(M->cols, M->rows, M->rows);
  
	if (M != NULL) {
    int i, j;
    for(i=0; i < M->rows; i++) 
    {
      for(j=0; j < M->cols; j++)
      {
        // i = numero de linha
        // j = numero de coluna
        // i * widthStep = posicao no array, equivalente ao inicio
        // da linha i;
        A->data[j*A->widthStep + i] = M->data[M->widthStep*i + j];
      }
    }
  } else{
    printf("\nErro ao tentar transpor matriz!\n\n");
  }
  
  *M = *A;
}

Image * loadPBM(char * fname){
  FILE * file;
  struct pam inpam;
  tuple * tuplerow;
  unsigned int row;
  Image * image;
  int aux;

  file=fopen(fname,"r");
  pnm_readpaminit(file, &inpam, /*PAM_STRUCT_SIZE(tuple_type)*/ sizeof(struct pam));
   
  printf("Reading image\n");
  printf("width=%d,height=%d,depth=%d\n",inpam.width,inpam.height,inpam.depth);
   
  /* allocating image*/
  image=(Image*)malloc(sizeof(Image));
  image->cols=inpam.width;
  image->rows=inpam.height;
  image->widthStep=image->cols;
  //aux=image->cols & 0x3;
  // if (aux!=0){
  //   image->widthStep+=4-aux;
  // }
  image->data=(unsigned char *)malloc(image->widthStep*image->rows);
   
  tuplerow = pnm_allocpamrow(&inpam);
   
  for (row = 0; row < inpam.height; row++) {
    unsigned int column;
    pnm_readpamrow(&inpam, tuplerow);
    for (column = 0; column < inpam.width; ++column) {
      unsigned int plane;
      for (plane = 0; plane < inpam.depth; ++plane) {
	image->data[image->widthStep*row+column]= tuplerow[column][plane];
      }
    }
  } 
     
  pnm_freepamrow(tuplerow);
  fclose(file);
  return image;
}

void savePBM(char * fname, Image * image){
  FILE * file;
  struct pam outpam;
  tuple * tuplerow;
  unsigned int row;

  int aux;

  file=fopen(fname,"w");
  outpam.file=file;
  outpam.size=sizeof(struct pam);
  outpam.len=sizeof(struct pam);
  outpam.format=RPGM_FORMAT;
  outpam.plainformat=0;
  outpam.height=image->rows;
  outpam.width=image->cols;
  outpam.depth=1;
  outpam.maxval=255;
  strcpy(outpam.tuple_type,PAM_PGM_TUPLETYPE);
  /*  outpam.allocation_depth=0;
  outpam.comment_p="ficha 4 de Arquitecura de computadores 2010";
  */
  pnm_writepaminit( &outpam );
   
  printf("Writing image - width: %d, height: %d\n", outpam.width, outpam.height);

  tuplerow = pnm_allocpamrow(&outpam);
   
  for (row = 0; row < outpam.height; row++) {
    unsigned int column;
    for (column = 0; column < outpam.width; ++column) {
      unsigned int plane;
      for (plane = 0; plane < outpam.depth; ++plane) {
	tuplerow[column][plane]=image->data[image->widthStep*row+column];
      }
    }
    pnm_writepamrow(&outpam, tuplerow); 
  } 
     
  pnm_freepamrow(tuplerow);
  fclose(file);
}

void normalize_img(ImageF * img){
  for (int i = 0; i < img->rows; i++)
    {
        for (int j = 0; j < img->cols; j++)
        {
            double val;
            val = img->data[i*img->cols + j]/(img->cols*img->rows);

            if (val < 0)
                val = 0.0;
            else if (val > 255)
                val = 255.0;
            img->data[i * img->cols + j] =val;
        }
    }
}