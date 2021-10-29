#include<iostream>
using namespace std;
double * MatrixTrans_naif(double *A, int n)
{
	double *B = new double[n*n];
	for( int i = 0 ; i < n ; i++)
	{
		for( int j = 0 ; j < n ; j++)
		{
			B[i*n+j]=A[j*n+i];
		}
	}
	return B;
}
double * MatrixTrans_blocksize(double *A,int blocksize, int n)
{
	double *B = new double[n*n];
	for (int i = 0; i < n; i += blocksize) {
	    for (int j = 0; j < n; j += blocksize) {
	        for (int k = i; k < i + blocksize; k++) {
	            for (int l = j; l < j + blocksize; l++) {
	                B[k + l*n] = A[l + k*n];
	            }
	        }
	    }
	}
	return B;
}
void print_submatrix(const double * M, size_t m, size_t n , size_t s )
{
	for(size_t i=0; i<m;i++)
	{
		for(size_t j=0;j<n;j++)
		{
			cout<<M[i*s+j]<<" ";

		}
		cout<<endl;
	}
}
void MatrixTrans_obliv(size_t m, size_t n, const double* A, size_t sa, double * B, size_t sb)
{
	if(n==2 && m==2)
	{
		B[0] = A[0];
		B[1] = A[2];
		B[2] = A[1];
		B[3] = A[3];
	}
	else
	{
		cout<<"lol"<<endl;
		print_submatrix(A, m/2,n/2 , n/2 );
		print_submatrix(A+ n/2, n/2 ,n/2,n  );
		print_submatrix(A+ n*n/2, n/2 ,n/2,n  );
		print_submatrix(A+ n*n/2+n/2, n/2 ,n/2,n  );

		MatrixTrans_obliv(m/2, n/2, A, n/2,  B, n/2);
		MatrixTrans_obliv(m/2, n/2, A+n/2, n/2,  B+n/2, n/2);
		MatrixTrans_obliv(m/2, n/2, A+ n*n/2, n/2,  B+ n*n/2, n/2);
		MatrixTrans_obliv(m/2, n/2, A+ n*n/2+n/2, n/2,  B+ n*n/2+n/2, n/2);

	}


}

void printMatrix(double *A, int n )
{
	for( int i = 0 ; i < n ; i++)
	{
		for( int j = 0 ; j < n ; j++)
		{
			cout<<" "<<A[i*n+j]<<" ";
		}
		cout<<endl;
	}
}
void print_submatrix(double *A, int n )
{

		cout<<"Sub: 1"<<endl;
		print_submatrix(A+ 0, n/2 ,n/2,n  );
		cout<<"Sub: 2"<<endl;
		print_submatrix(A+ n/2, n/2 ,n/2,n  );
		cout<<"Sub: 3"<<endl;
		print_submatrix(A+ n*n/2, n/2 ,n/2,n  );
		cout<<"Sub: 4"<<endl;
		print_submatrix(A+ n*n/2+n/2, n/2 ,n/2,n  );

	
}
int main()
{	
	/*

	A

	1 2 3
	4 5 6 
	7 8 9

	B

	1 4 7
	2 5 8
	3 6 9

	B = A^T 

	*/
	//int n=455;
	int n=16; 
	double *A = new double[n*n];
	for(int i = 0 ; i <n*n;i++)A[i]=(double)i;
	double *B = new double[n*n];
	//B= MatrixTrans_naif(A, n);
	//B= MatrixTrans_blocksize(A,2, n);
/*	print_submatrix(A+ 0, 8 ,8,16  );
	print_submatrix(A+ 8, 8 ,8,16  );
	print_submatrix(A+ 16*8, 8 ,8,16  );
	print_submatrix(A+ 16*8+8, 8 ,8,16  );*/
	//print_submatrix(A,n);
	//printMatrix(A,n);
	MatrixTrans_obliv(16, 16,A, 16, B, 16);
	printMatrix(B,n);
	return 0;

}