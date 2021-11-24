#include <cstdio>
#include <cstdlib>

#include <time.h>

#define cudaCheckError() {                                                                       \
        cudaError_t e=cudaGetLastError();                                                        \
        if(e!=cudaSuccess) {                                                                     \
            printf("Cuda failure %s:%d: '%s'\n",__FILE__,__LINE__,cudaGetErrorString(e));        \
            exit(EXIT_FAILURE);                                                                  \
        }                                                                                        \
    }

__global__ void kernel(float *a,float *b, float *c, int N) {
  int i=blockIdx.x*blockDim.x+threadIdx.x;
  if(i<N)
    c[i]=a[i]+b[i];
}

__global__ void addMatrices(float *a,float *b, float *c, int N) {
  int i=blockIdx.x*blockDim.x+threadIdx.x;
  if(i<N)
    c[i]=a[i]+b[i];
}

float * addMatrices(float *a, float *b ,int N, int M)
{

  float c* =(float*)malloc(N*M*sizeof(float));
  for(int i =0;i<N*M;i++)
  {
    int j = N*i+i;
    c[j]=a[j]+b[j];

  }
  return c;
}
/*

Reponses aux questions :
1/ 
1024 -> 1*1024 OU 2*512 OU 4*256 OU 8*128 OU 16*64 OU 32*32
1023 -> comme 1024
1025 -> 2*1024 OU OU 3*512 OU 4*256 OU 9*128 OU 17*64 OU 33*32
2/ 
taille grid = (n+127)/128

3/ 1+128*k
*/
int main(int argc, char *argv[]) {
  srand( time( NULL ) );
  
  if(argc < 1) { 
    return 0;
 }
  int n  = atoi(argv[1]);
  //int n=4097;
  int threads=1024;
  int blocks=(n+threads-1)/threads;
  int size = n * sizeof(float);

  float *d_A, *d_B, *d_C;
  float *h_A, *h_B, *h_C;

  h_A = (float*)malloc(n*sizeof(float));
  h_B = (float*)malloc(n*sizeof(float));
  h_C = (float*)malloc(n*sizeof(float));


  for(int i=0;i<n;i++)
  {
    h_A[i]=(float)(rand() % 65535);
    h_B[i]=(float)(rand() % 65535);
  }

  cudaMalloc((void **) &d_A, size);    
  cudaMalloc((void **) &d_B, size);
  cudaMalloc((void **) &d_C, size);
  cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
  cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);
/*
  cudaMallocManaged(&a,N*sizeof(int));
  cudaMallocManaged(&b,N*sizeof(int));
  cudaMallocManaged(&c,N*sizeof(int));
*/



  kernel<<<blocks,threads>>>(d_A,d_B,d_C, size);
  cudaDeviceSynchronize();


  cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);
  /*for(int i=0;i<n;i++)
    printf("%f\n",h_C[i]);*/
  cudaFree(d_A); cudaFree(d_B); cudaFree (d_C);
  cudaCheckError();
  return 0;
}
