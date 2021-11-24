#include <cstdio>
#include <cstdlib>
#include "image-pnm.h"
#include <time.h>

#define cudaCheckError() {                                                                       \
        cudaError_t e=cudaGetLastError();                                                        \
        if(e!=cudaSuccess) {                                                                     \
            printf("Cuda failure %s:%d: '%s'\n",__FILE__,__LINE__,cudaGetErrorString(e));        \
            exit(EXIT_FAILURE);                                                                  \
        }                                                                                        \
    }

__global__ blur_pbm_kernel(uint8 t *ImgOut, const uint8 t* imgIn, int width, int height, int b)
{
  int col=blockIdx.x*blockDim.x+threadIdx.x;
  int row=blockIdx.y*blockDim.y+threadIdx.y;

  int idx = row*width + col;

  if( (row<width) && (col <height))
    c[idx]=a[idx]+b[idx];
  {
    if(row>=b && col>=b)
    {
       int s=0;
      for(int k = i-b; k<i+b ; k++)
        for(int l = j-b ; l<j+b ; l++)
          s+= imgIn(k,l);
      s= s/((2*b+1)*(2*b+1));
      ImgOut[i*dst.width()+j]=s;
    }

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

int main(int argc, char *argv[]) {
  srand( time( NULL ) );
  
  if(argc < 2) { 
    return 0;
 }
  int n  = atoi(argv[1]);
  int m  = atoi(argv[2]);
  
  uint8 *d_A, *d_B;
  uint8 *h_A, *h_B;

  //int * data;
  Image<int> src = Image<int>("P2", 512,512 );
  Image<int> dst = Image<int>("P2", 512,512 );
  n  = 512;
  m  = 512;

  int threads=4;
  int size = m*n * sizeof(uint8);

  dim3 DimGrid((n-1)/threads+1, (m-1)/threads+1,1);
  dim3 DimBlock(threads, threads,1);

  cudaMalloc((void **) &d_A, size);    
  cudaMalloc((void **) &d_B, size);

  h_A = (uint8*)malloc(size);
  h_B = (uint8*)malloc(size);

  cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);


  blur_pbm_kernel<<<DimGrid,DimBlock>>>(d_A,d_B, n,m,25);
  cudaDeviceSynchronize();

  cudaMemcpy(h_B, d_B, size, cudaMemcpyDeviceToHost);


  src.read("image-grey.pgm");
  //data = src.getData(); 
  /*for(int i =0 ; i < 512*512;i++)
    //printf("%d",data[i]);
    data[i]*=2;*/
  blur_pgm(src,  dst, 25);
  
  printf("ok");
    cudaFree(d_A); cudaFree(d_B);
  free(h_A);  free(h_B); 


  cudaCheckError();
  return 0;
}
