#include <cstdio>
#include <cstdlib>
#include "image-pnm.h"
#include <time.h>
#include <iostream>
#include <fstream>

#define cudaCheckError() {                                                                       \
        cudaError_t e=cudaGetLastError();                                                        \
        if(e!=cudaSuccess) {                                                                     \
            printf("Cuda failure %s:%d: '%s'\n",__FILE__,__LINE__,cudaGetErrorString(e));        \
            exit(EXIT_FAILURE);                                                                  \
        }                                                                                        \
    }

__global__ void blur_pbm_kernel(uint8_t *ImgOut, const uint8_t* imgIn, int width, int height, int b)
{
  
  int col=blockIdx.x*blockDim.x+threadIdx.x;
  int row=blockIdx.y*blockDim.y+threadIdx.y;

  int idx = row*width + col;

  int s;
  if( (row<width) && (col <height))
  {
    s =0;

    for(int i = -b ; i < b+1; i++)
    {
      for(int j = -b ; j < b+1; j++)
      {
        int curRow = row +i;
        int curCol = col +j;
        if(curRow> -1&& curRow< height && curCol> -1&& curCol< width)
          s+=imgIn[curRow*width + curCol];
      } 
      
    }
    s = s/((2*b+1)*(2*b+1));
    ImgOut[idx] = s; 
  }
}
__global__ void colorToGreykernel(uint8_t *ImgOut, const uint8_t* imgIn, int width, int height, int b)
{
  
  int col=blockIdx.x*blockDim.x+threadIdx.x;
  int row=blockIdx.y*blockDim.y+threadIdx.y;

  int idx = row*width + col;

  int s;
  if( (row<width) && (col <height))
  {
    ImgOut[idx] = imgIn[idx]; 
    /*
    s =0;

        for(int i = -b ; i < b+1; i++)
          for(int j = -b ; j < b+1; j++)
          {
            int curRow = row +i;
            int curCol = col +j;
            if(curRow> -1&& curRow< height && curCol> -1&& curCol< width)
              s+=imgIn[curRow*width + curCol];
          } 
          
        s = s/((2*b+1)*(2*b+1));
        ImgOut[idx] = s; 
    }
  */
  }
  
}


void blur_pgm(Image<int> src, Image<int> dst, int b)
{
  int *data;
  data = dst.getData();
  for(int i =b; i < dst.height();i++)
    for(int j = b ; j< dst.width();j++)
    {
      int s=0;

      for(int k = i-b; k<i+b ; k++)
        for(int l = j-b ; l<j+b ; l++)
          s+= src.get(k,l);
      s= s/((2*b+1)*(2*b+1));
      data[i*dst.width()+j]=s;
    }
    dst.write("image-grey2.pgm");

}



uint8_t * loadPPM(const char * file)
{
    uint8_t *data; 
    std::string control;
    int _col;
    int _row;
    std::ifstream is(file);
    is>>control;
    is>>_col>>_row;
    size_t channels = 3;

    data = (uint8_t*)malloc(_col*_row*channels);

    for (size_t i=0;i<_row;i++)
    for(size_t j=0;j<_col;j++)
    for(size_t c=0; c< channels;c++)
        is>>data[i*(_col*c)+j];


    std::ofstream os("output2.ppm");
    for (size_t i=0;i<_row;i++)
    for(size_t j=0;j<_col;j++)
        is>>data[i*_col+j];

    os<<control<<std::endl;
    os<<_col<<" "<<_row<<std::endl;




}
int main(int argc, char *argv[]) {
  srand( time( NULL ) );
    
  uint8_t *d_A, *d_B;
  uint8_t *h_A, *h_B;

  int * data;
  int *data2;
  //Image<int> src = Image<int>("P2", 512,512 );
  //Image<int> dst = Image<int>("P2", 512,512 );
  int n  = 512;
  int m  = 512;

  Image<int> src = Image<int>("P3", 512,512 );
  Image<int> dst = Image<int>("P3", 512,512 );

  int threads=16;
  //int size = m*n * sizeof(uint8_t);

  int size = 3*m*n *sizeof(uint8_t);

  dim3 DimGrid((n-1)/threads+1, (m-1)/threads+1,1);
  dim3 DimBlock(threads, threads,1);

  cudaMalloc((void **) &d_A, size);    
  cudaMalloc((void **) &d_B, size);

  h_A = (uint8_t*)malloc(size);
  h_B = (uint8_t*)malloc(size);

  //src.read("image-grey.pgm");
  src.read("image-color.ppm");

  data = src.getData();

  data2 = dst.getData();

  for(size_t i = 0 ; i < n*m; i++)
  {
    h_A[i]=(uint8_t )data[i];
  }
  cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);


  colorToGreykernel<<<DimGrid,DimBlock>>>(d_B,d_A, n,m,25);
  cudaDeviceSynchronize();

  cudaMemcpy(h_B, d_B, size, cudaMemcpyDeviceToHost);
  for(size_t i = 0 ; i < n*m; i++)
  {
    data2[i]=(int)h_B[i];
  }
  for(int i =0 ; i < n*m;i++)
      printf("%d ",data2[i]);
/*
  src.read("image-grey.pgm");
  data = src.getData(); 
  for(int i =0 ; i < n*m;i++)
    //printf("%d",data[i]);
    data[i]*=2;
  blur_pgm(src,  dst, 25);
  
  printf("ok");
  */
  dst.write("image-color2.ppm");

  loadPPM("image-color.ppm");

  cudaFree(d_A); cudaFree(d_B);
  free(h_A);  free(h_B); 

  cudaCheckError();
  return 0;
}
