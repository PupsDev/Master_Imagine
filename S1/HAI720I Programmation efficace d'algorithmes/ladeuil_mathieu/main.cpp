#include<iostream>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include "EvalPerf.h"
#include <math.h>

#include <vector>
using namespace std;
double * MatrixTrans_naif(double *A, size_t n)
{
	double *B = new double[n*n];
	for( size_t i = 0 ; i < n ; i++)
	{
		for( size_t j = 0 ; j < n ; j++)
		{
			B[i*n+j]=A[j*n+i];
		}
	}
	return B;
}
double * MatrixTrans_blocksize(double *A,size_t blocksize, size_t n)
{
	double *B = new double[n*n];
	for (size_t i = 0; i < n; i += blocksize) {
	    for (size_t j = 0; j < n; j += blocksize) {
	        for (size_t k = i; k < i + blocksize; k++) {
	            for (size_t l = j; l < j + blocksize; l++) {
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
			if(M[i*s+j]<10)
			{
				cout<<" "<<M[i*s+j]<<" ";
			}
			else
				cout<<M[i*s+j]<<" ";

		}
		cout<<endl;
	}
}

void printMatrix(const double *A, size_t n )
{
	for( size_t i = 0 ; i < n ; i++)
	{
		for( size_t j = 0 ; j < n ; j++)
		{
			cout<<" "<<A[i*n+j]<<" ";
		}
		cout<<endl;
	}
}
void printMatrixNC(const double *A, size_t m, size_t n )
{
	for( size_t i = 0 ; i < m ; i++)
	{
		for( size_t j = 0 ; j < n ; j++)
		{
			cout<<" "<<A[i*n+j]<<" ";
		}
		cout<<endl;
	}
}
void MatrixTransC_obliv(size_t n, const double* A, size_t s, double * B)
{
	if(n==2)
	{

		for( size_t i = 0 ; i < n ; i++)
		{
				for( size_t j = 0 ; j < n ; j++)
				{
					B[j*s+i]=A[i*s+j];
				}
				
		}
	}
	else
	{
		size_t m1,n1;
		m1=n1=n/2;
		

		MatrixTransC_obliv(n/2,A, s,  B);
		MatrixTransC_obliv(n/2,A+n/2, s,  B+s*n/2);
		MatrixTransC_obliv(n/2,A+ s*n/2, s,  B+ n/2);
		MatrixTransC_obliv(n/2,A+ s*n/2+n/2, s,  B+ s*n/2+n/2);
	}
}
void MatrixTrans_obliv(size_t m, size_t n, const double* A, size_t sa, double * B, size_t sb)
{
	if(n==2 || m==2)
	{

		for( size_t i = 0 ; i < m ; i++)
		{
				for( size_t j = 0 ; j < n ; j++)
				{
					B[j*sb+i]=A[i*sa+j];
				}
				
		}
	}
	else
	{
		size_t m1,n1;
		m1=m/2;
		n1=n/2;


		MatrixTrans_obliv(m1,n1,A, sa,  B,sb);
		MatrixTrans_obliv(m1,n1,A+n1, sa,  B+sb*n1,sb);
		MatrixTrans_obliv(m1,n1,A+ sa*m1, sa,  B+ m1,sb);
		MatrixTrans_obliv(m1,n1,A+ sa*m1+n1, sa,  B+ sb*n1+m1,sb);
	}

}
void print_submatrix(double *A, size_t n )
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
void Eval1(size_t compteurLimt, size_t n, bool rando, bool verbose)
{


	double *A = new double[n*n];
	double *B = new double[n*n];
	EvalPerf PE;

	unsigned int  compteur =0;
	unsigned int  nbrcycles=0, milliseconds=0, cpi=0;
	float seconds=0;
	if(!rando)
		for(size_t i = 0 ; i <n*n;i++)A[i]=(double)i;
	else
	{
		  srand (time(NULL));
		  for(size_t i = 0 ; i <n*n;i++)A[i]=(double)(rand() % n + 1);
	}

	while(compteur<compteurLimt)
	{
		
		if(verbose)
		{
			std::cout<<"Evaluation num." << compteur<<"de taille N = "<<n<<std::endl;
			std::cout<<"Matrix A" <<endl;
			printMatrix(A, n );

		}

		PE.start();
		B = MatrixTrans_naif(A, n);
		PE.stop();
		if(verbose)
		{
			std::cout<<"Matrix B" <<endl;
			printMatrix(B, n );

			std::cout<<"nbrcycles:"<<PE.nb_cycle()<<std::endl;
			std::cout<<"nb seconds : "<<PE.second()<<std::endl;
			std::cout<<"nb milliseconds : "<<PE.millisecond()<<std::endl;
			std::cout<<"CPI = "<<PE.CPI(n)<<std::endl;
		}

		nbrcycles+=PE.nb_cycle();
		seconds+=PE.second();
		milliseconds+=PE.millisecond();
		cpi+= PE.CPI(n);
		compteur++;
		
	}
	std::cout<<std::endl;
	std::cout<<"Average nbrcycles:"<<nbrcycles/compteurLimt<<std::endl;
	std::cout<<"Average nb seconds : "<<seconds/compteurLimt<<std::endl;
	std::cout<<"Average nb milliseconds : "<<milliseconds/compteurLimt<<std::endl;
	std::cout<<"Average CPI = "<<cpi/compteurLimt<<std::endl;

}
void Eval2(size_t compteurLimt, size_t n,size_t blocksize, bool rando, bool verbose)
{


	double *A = new double[n*n];
	double *B = new double[n*n];
	EvalPerf PE;

	unsigned int  compteur =0;
	unsigned int  nbrcycles=0, milliseconds=0, cpi=0;
	float seconds=0;
	if(!rando)
		for(size_t i = 0 ; i <n*n;i++)A[i]=(double)i;
	else
	{
		  srand (time(NULL));
		  for(size_t i = 0 ; i <n*n;i++)A[i]=(double)(rand() % n + 1);
	}

	while(compteur<compteurLimt)
	{
		
		if(verbose)
		{
			std::cout<<"Evaluation num." << compteur<<"de taille N = "<<n<<std::endl;
			std::cout<<"Matrix A" <<endl;
			printMatrix(A, n );

		}

		PE.start();
		B = MatrixTrans_blocksize(A,blocksize, n);
		PE.stop();
		if(verbose)
		{
			std::cout<<"Matrix B" <<endl;
			printMatrix(B, n );

			std::cout<<"nbrcycles:"<<PE.nb_cycle()<<std::endl;
			std::cout<<"nb seconds : "<<PE.second()<<std::endl;
			std::cout<<"nb milliseconds : "<<PE.millisecond()<<std::endl;
			std::cout<<"CPI = "<<PE.CPI(n)<<std::endl;
		}

		nbrcycles+=PE.nb_cycle();
		seconds+=PE.second();
		milliseconds+=PE.millisecond();
		cpi+= PE.CPI(n);
		compteur++;
		
	}
	std::cout<<std::endl;
	std::cout<<"Average nbrcycles:"<<nbrcycles/compteurLimt<<std::endl;
	std::cout<<"Average nb seconds : "<<seconds/compteurLimt<<std::endl;
	std::cout<<"Average nb milliseconds : "<<milliseconds/compteurLimt<<std::endl;
	std::cout<<"Average CPI = "<<cpi/compteurLimt<<std::endl;

}
void Eval3(size_t compteurLimt, size_t n,size_t m, bool rando, bool verbose)
{


	double *A = new double[m*n];
	double *B = new double[m*n];
	EvalPerf PE;

	unsigned int  compteur =0;
	unsigned int  nbrcycles=0, milliseconds=0, cpi=0;
	float seconds=0;
	if(!rando)
		for(size_t i = 0 ; i <m*n;i++)A[i]=(double)i;
	else
	{
		  srand (time(NULL));
		  for(size_t i = 0 ; i <m*n;i++)A[i]=(double)(rand() % (m*n) + 1);
	}

	while(compteur<compteurLimt)
	{
		
		if(verbose)
		{
			std::cout<<"Evaluation num." << compteur<<"de taille N = "<<n<<std::endl;
			std::cout<<"Matrix A" <<endl;
			printMatrixNC(A, m,n );

		}

		PE.start();
		MatrixTrans_obliv(m, n,A, n, B, m);
		PE.stop();
		if(verbose)
		{
			std::cout<<"Matrix B" <<endl;
			printMatrixNC(B, n,m );

			std::cout<<"nbrcycles:"<<PE.nb_cycle()<<std::endl;
			std::cout<<"nb seconds : "<<PE.second()<<std::endl;
			std::cout<<"nb milliseconds : "<<PE.millisecond()<<std::endl;
			std::cout<<"CPI = "<<PE.CPI(n)<<std::endl;
		}

		nbrcycles+=PE.nb_cycle();
		seconds+=PE.second();
		milliseconds+=PE.millisecond();
		cpi+= PE.CPI(n);
		compteur++;
		
	}
	std::cout<<std::endl;
	std::cout<<"Average nbrcycles:"<<nbrcycles/compteurLimt<<std::endl;
	std::cout<<"Average nb seconds : "<<seconds/compteurLimt<<std::endl;
	std::cout<<"Average nb milliseconds : "<<milliseconds/compteurLimt<<std::endl;
	std::cout<<"Average CPI = "<<cpi/compteurLimt<<std::endl;
	std::cout<<std::endl;

}

void counting_sort_naif(size_t *R, const size_t *T, size_t n , size_t min, size_t max)
{
    size_t range = max - min + 1;
 
    size_t * count=new size_t[range]();

    for (size_t i = 0; i < n; i++)
        count[T[i] - min]++;
 
    for (size_t i = 1; i < range; i++)
        count[i] += count[i - 1];
 
    for (int i = n - 1; i >= 0; i--) {
        R[count[T[i] - min] - 1] = T[i];
        count[T[i] - min]--;
    }
 

}
void counting_sort_2(size_t *R, const size_t *T, size_t n , size_t min, size_t max)
{
    size_t range = max - min + 1;
 
    size_t * count=new size_t[range]();

    for (size_t i = 0; i < n; i++)
        count[T[i] - min]++;
 
    for (size_t i = 1; i < range; i++)
        count[i] += count[i - 1];
 
    for (int i = n - 1; i >= 0; i--) {
        R[count[T[i] - min] - 1] = T[i];
        count[T[i] - min]--;
    }
 


}
void counting_sort_bucket(size_t *R, const size_t *T, size_t n , size_t min, size_t max)
{
 	size_t bucketNb;
 	if(n>= 128)
 		bucketNb=8;
 	else
 		bucketNb = 4<n ? 4 : n; 
    size_t * bucket=new size_t[bucketNb]();

    for (size_t i = 0; i < n; i++)
    {
    	size_t q = (bucketNb*T[i] - min)/max;
    	bucket[q]++;
    }
 
    for (size_t i = 1; i < bucketNb; i++)
        bucket[i] += bucket[i - 1];
 
    for (int i = n - 1; i >= 0; i--) {
    	size_t q = (bucketNb*T[i] - min)/max;
        R[bucket[q]] = T[i];
        bucket[q]--;
    }
 

}
void EvalCountingSortNaif(int compteurLimt, size_t n, bool rando, bool verbose)
{


	size_t * A=new size_t[n];
	size_t * B=new size_t[n];

	EvalPerf PE;

	unsigned int  compteur =0;
	unsigned int  nbrcycles=0, milliseconds=0, cpi=0;
	float seconds=0;
	if(!rando)
		for(size_t i = 0 ; i <n;i++)A[i]=(size_t)i;
	else
	{
		  srand (time(NULL));
		  for(size_t i = 0 ; i <n;i++)A[i]=(size_t)(rand() % n + 1);
	}
	size_t min = n+1;
	int max = -1;
	for(size_t i = 0 ; i < n ;i ++)
	{
		if(A[i]<min)min=A[i];
		if((int)A[i]>max)max=(int)A[i];
	}

	while(compteur<compteurLimt)
	{
		
		if(verbose)
		{
			std::cout<<"Evaluation num." << compteur<<"de taille N = "<<n<<std::endl;
			std::cout<<"Array" <<endl;
			for(size_t i = 0 ; i < n; i ++)
		 	{	
		 		cout<<A[i]<<endl;
		 	}

		}

		PE.start();
		counting_sort_naif(B, A,  n , min, (size_t)max);
		PE.stop();
		if(verbose)
		{
			std::cout<<"Sorted" <<endl;
			for(size_t i = 0 ; i < n; i ++)
		 	{	
		 		cout<<B[i]<<endl;
		 	}

			std::cout<<"nbrcycles:"<<PE.nb_cycle()<<std::endl;
			std::cout<<"nb seconds : "<<PE.second()<<std::endl;
			std::cout<<"nb milliseconds : "<<PE.millisecond()<<std::endl;
			std::cout<<"CPI = "<<PE.CPI(n)<<std::endl;
		}

		nbrcycles+=PE.nb_cycle();
		seconds+=PE.second();
		milliseconds+=PE.millisecond();
		cpi+= PE.CPI(n);
		compteur++;
		
	}
	std::cout<<std::endl;
	std::cout<<"Average nbrcycles:"<<nbrcycles/compteurLimt<<std::endl;
	std::cout<<"Average nb seconds : "<<seconds/compteurLimt<<std::endl;
	std::cout<<"Average nb milliseconds : "<<milliseconds/compteurLimt<<std::endl;
	std::cout<<"Average CPI = "<<cpi/compteurLimt<<std::endl;
	std::cout<<std::endl;

}
void EvalCountingSortBucket(int compteurLimt, size_t n, bool rando, bool verbose)
{


	size_t * A=new size_t[n];
	size_t * B=new size_t[n];

	EvalPerf PE;

	unsigned int  compteur =0;
	unsigned int  nbrcycles=0, milliseconds=0, cpi=0;
	float seconds=0;
	if(!rando)
		for(size_t i = 0 ; i <n;i++)A[i]=(size_t)i;
	else
	{
		  srand (time(NULL));
		  for(size_t i = 0 ; i <n;i++)A[i]=(size_t)(rand() % n + 1);
	}
	size_t min = n+1;
	int max = -1;
	for(size_t i = 0 ; i < n ;i ++)
	{
		if(A[i]<min)min=A[i];
		if((int)A[i]>max)max=(int)A[i];
	}

	while(compteur<compteurLimt)
	{
		
		if(verbose)
		{
			std::cout<<"Evaluation num." << compteur<<"de taille N = "<<n<<std::endl;
			std::cout<<"Array" <<endl;
			for(size_t i = 0 ; i < n; i ++)
		 	{	
		 		cout<<A[i]<<endl;
		 	}

		}

		PE.start();
		counting_sort_bucket(B, A,  n , min, (size_t)max);
		PE.stop();
		if(verbose)
		{
			std::cout<<"Sorted" <<endl;
			for(size_t i = 0 ; i < n; i ++)
		 	{	
		 		cout<<B[i]<<endl;
		 	}

			std::cout<<"nbrcycles:"<<PE.nb_cycle()<<std::endl;
			std::cout<<"nb seconds : "<<PE.second()<<std::endl;
			std::cout<<"nb milliseconds : "<<PE.millisecond()<<std::endl;
			std::cout<<"CPI = "<<PE.CPI(n)<<std::endl;
		}

		nbrcycles+=PE.nb_cycle();
		seconds+=PE.second();
		milliseconds+=PE.millisecond();
		cpi+= PE.CPI(n);
		compteur++;
		
	}
	std::cout<<std::endl;
	std::cout<<"Average nbrcycles:"<<nbrcycles/compteurLimt<<std::endl;
	std::cout<<"Average nb seconds : "<<seconds/compteurLimt<<std::endl;
	std::cout<<"Average nb milliseconds : "<<milliseconds/compteurLimt<<std::endl;
	std::cout<<"Average CPI = "<<cpi/compteurLimt<<std::endl;
	std::cout<<std::endl;

}


void merge(size_t *A, size_t low, size_t high, size_t mid)
{
    size_t i, j, k, c[50];
    i = low;
    k = low;
    j = mid + 1;
    while (i <= mid && j <= high) {
        if (A[i] < A[j]) {
            c[k] = A[i];
            k++;
            i++;
        }
        else  {
            c[k] = A[j];
            k++;
            j++;
        }
    }
    while (i <= mid) {
        c[k] = A[i];
        k++;
        i++;
    }
    while (j <= high) {
        c[k] = A[j];
        k++;
        j++;
    }
    for (i = low; i < k; i++)  {
        A[i] = c[i];
    }
}
void merge_sort(size_t *A, size_t low, size_t high)
{
    size_t mid;
    if (low < high){

        mid=(low+high)/2;
        merge_sort(A,low,mid);
        merge_sort(A,mid+1,high);

        merge(A,low,high,mid);
    }
}
int main()
{	

	//Ex 20
	
	size_t compteurLimt = 5;
	bool useRandom = false;
	bool verbose = false;

	size_t n = 7000;

	size_t blocksize = 2;
	for(size_t i = 7;i<9;i++)
	{
		cout<<"Matrix Transpose blocksize "<<compteurLimt<<" iterations ; "<<"n= "<<pow(2,i)<<"; blocksize= "<<blocksize<<endl;
		Eval2(compteurLimt, pow(2,i), blocksize, useRandom, verbose);

	}
	blocksize = 4;
	for(size_t i = 9;i<11;i++)
	{
		cout<<"Matrix Transpose blocksize "<<compteurLimt<<" iterations ; "<<"n= "<<pow(2,i)<<"; blocksize= "<<blocksize<<endl;
		Eval2(compteurLimt, pow(2,i), blocksize, useRandom, verbose);

	}


	cout<<"Matrix Transpose naive "<<compteurLimt<<" iterations ; "<<"n= "<<n<<endl;
	Eval1(compteurLimt, n, useRandom, verbose);

	blocksize = 100;
	cout<<"Matrix Transpose blocksize "<<compteurLimt<<" iterations ; "<<"n= "<<n<<"; blocksize= "<<blocksize<<endl;
	Eval2(compteurLimt, n, blocksize, useRandom, verbose);

	n=6000;
	size_t m = 3000;
	cout<<"Matrix Transpose recursive "<<compteurLimt<<" iterations ; "<<"n= "<<n<<"; m= "<<m<<endl;
	Eval3(compteurLimt, n, m, useRandom, verbose);
	
	//Ex21
	/*
	size_t n=10;
	size_t * A=new size_t[n];
	size_t * B=new size_t[n];
	size_t * C=new size_t[n];
	srand (time(NULL));
	for(size_t i = 0 ; i < n ; i ++)A[i]=(size_t)(rand()%n+1);
	for(size_t i = 0 ; i < n ; i ++)cout<<A[i]<<endl;

	size_t min = n+1;
	int max = -1;
	for(size_t i = 0 ; i < n ;i ++)
	{
		if(A[i]<min)min=A[i];
		if((int)A[i]>max)max=(int)A[i];
	}

	cout<<"algo"<<endl;
	cout<<"min"<<min<<endl;
	cout<<"max"<<max<<endl;
	
	counting_sort_naif(B, A,  n , min, (size_t)max);
	cout<<"sorted"<<endl;

 	for(size_t i = 0 ; i < n; i ++)
 	{	
 		cout<<B[i]<<endl;
 	}
 	counting_sort_bucket(C, A,  n , min, (size_t)max);
 	cout<<"sorted"<<endl;

 	for(size_t i = 0 ; i < n; i ++)
 	{	
 		cout<<B[i]<<endl;
 	}
 	*/
 	
 	cout<<"Counting sort naif:"<<endl;
 	for(size_t k = 16 ; k< 25;k++)
 	{
 		n = pow(2,k);

 		cout<<"tableau de taille n = "<<n<<endl;
 		EvalCountingSortNaif(5, n, true , false);
 	}
 	cout<<"Counting sort bucket:"<<endl;
 	for(size_t k = 16 ; k< 25;k++)
 	{
 		n = pow(2,k);
 		cout<<"tableau de taille n = "<<n<<endl;
 		EvalCountingSortBucket(5, n, true , false);
 	}

	cout<<"Merge sort:"<<endl;
	n=10;
	size_t * A=new size_t[n];
	for(size_t i = 0 ; i < n ; i ++)A[i]=(size_t)(rand()%n+1);
		for(size_t i = 0 ; i < n ; i ++)cout<<A[i]<<endl;
 	merge_sort(A,  0, n);
 	cout<<"sorted"<<endl;
 	for(size_t i = 0 ; i < n ; i ++)cout<<A[i]<<endl;
 	



	return 0;

}