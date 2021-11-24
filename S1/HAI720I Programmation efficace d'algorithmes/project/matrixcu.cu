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

__global__ void addMatrices(float *a,float *b, float *c, int N, int M) {
  int col=blockIdx.x*blockDim.x+threadIdx.x;
  int row=blockIdx.y*blockDim.y+threadIdx.y;
  int idx = row*N + col;

  if( (row<N) && (col <N))
    c[idx]=a[idx]+b[idx];

}

void caddMatrices(float *a, float *b, float*c,int N, int M)
{

  for(int i =0;i<N*M;i++)
  {

    for(int j=0;j<M;j++)
    { 
      int idx = N*i + j;
        c[idx]=a[idx]+b[idx];
    }

  }
}

int main(int argc, char *argv[]) {
  srand( time( NULL ) );
  
  if(argc < 2) { 
    return 0;
 }
  int n  = atoi(argv[1]);
  int m  = atoi(argv[2]);

  int threads=4;
  int size = m*n * sizeof(float);

  float *d_A, *d_B, *d_C;
  float *h_A, *h_B, *h_C;
  float * result;

  dim3 DimGrid((n-1)/threads+1, (m-1)/threads+1,1);
  dim3 DimBlock(threads, threads,1);

  cudaMalloc((void **) &d_A, size);    
  cudaMalloc((void **) &d_B, size);
  cudaMalloc((void **) &d_C, size);

  h_A = (float*)malloc(size);
  h_B = (float*)malloc(size);
  h_C = (float*)malloc(size);
  result = (float*)malloc(size);


  for(int i=0;i<n*m;i++)
  {
    h_A[i]=(float)(rand() % 65535);
    h_B[i]=(float)(rand() % 65535);
  }
  /*for(int i=0;i<n*m;i++)
    printf("a: %f : b: %f \n",h_A[i],h_B[i]);*/

  cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
  cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);


  addMatrices<<<DimGrid,DimBlock>>>(d_A,d_B,d_C, n,m);
  cudaDeviceSynchronize();


  cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);
  caddMatrices(h_A,h_B,result,n,m);

  /*for(int i=0;i<(n*m);i++)
    printf("%f\n",h_C[i]);

  for(int i=0;i<(n*m);i++)
    printf("%f\n",result[i]);*/
  int b=1;
  for(int i=0;i<n*m;i++)
  {
    if(h_C[i]!=result[i])
      b =0;
  }
  
  if(b)
    printf("Tout ok !");


  cudaFree(d_A); cudaFree(d_B); cudaFree (d_C);
  free(h_A);  free(h_B);  free(h_C); free(result);



  cudaCheckError();
  return 0;
}
