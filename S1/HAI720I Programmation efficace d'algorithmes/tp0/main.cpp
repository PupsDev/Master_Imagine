#include <xmmintrin.h>
#include "EvalPerf.h"

/* Only needed for the sake of this example. */
#include <iostream>
#include <thread>
#include <math.h>
#include <cmath>
#include "reduce.hpp"
#include "slowperformance.hpp"


/*
__m512i loadu_si512(void *x) { return _mm512_loadu_si512(x); }
__m512i load_epi64(void *x)  {  return _mm512_load_epi64(x); }
//__m512i loadu_epi64(void *x) {  return _mm512_loadu_epi64(x); }

__m512i loadu_maskz(void *x) { return _mm512_maskz_loadu_epi64(0xf0, x); }
__m512i loadu_mask(void *x)  { return _mm512_mask_loadu_epi64(_mm512_setzero_si512(), 0xf0, x); }

*/

void copyVect(double * A , const double * B, size_t n)
{
	size_t i;
	__m256d R;
	for(i=0;i<n-3;i+=4)
	{
		R= __mm256_loadu_pd(B+i);
		__mm256_storeu_pd(A+i,R);
	}
	for(;i<n;i++)
		A[i]=B[i];
}	

void long_operation()
{
    /* Simulating a long, heavy operation. */

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(225ms);
}
void printTab(int tab[], int N)
{
	std::cout<<"[";
	for (int i =0 ;i<N;i++ )
	{
		if(i<N-1)
			std::cout<<tab[i]<<",";
		else
			std::cout<<tab[i];
	}
	std::cout<<"]"<<std::endl;
}
void sommePrefixe(int tab[], int N)
{
	
	int tab2[N];
	tab2[0]=tab[0];
	for (int i =0 ;i<N;i++ )
	{
		tab2[i+1]=tab2[i]+tab[i+1];
	}
	//printTab(tab2,N);
}
int calculPolynome(int tab[], int N, int alpha)
{	int result=tab[0];
	for(int i =1; i< N; i++)
	{
		result+=(tab[i]*pow(alpha,i));
	}
	return result;
}

int main()
{
	/*
	int N = 1000000;
	int tab[1000000];
	for (int i =0 ;i<N;i++ )
	{
		tab[i]=i;
	}
	int compteur =0;
	EvalPerf PE;

	int nbrcycles=0, milliseconds=0, cpi=0;
	float seconds=0;

	while(compteur<4)
	{
		std::cout<<"Evaluation num." << compteur<<"de taille N = "<<N<<std::endl;
		PE.start();
		//printTab(tab,N);
		std::cout<<"Après sommePrefixe :"<<std::endl;
		sommePrefixe(tab, N);
		PE.stop();

		std::cout<<"nbrcycles:"<<PE.nb_cycle()<<std::endl;
		std::cout<<"nb seconds : "<<PE.second()<<std::endl;
		std::cout<<"nb milliseconds : "<<PE.millisecond()<<std::endl;
		std::cout<<"CPI = "<<PE.CPI(N)<<std::endl;

		nbrcycles+=PE.nb_cycle();
		seconds+=PE.second();
		milliseconds+=PE.millisecond();
		cpi+= PE.CPI(N);
		compteur++;
		std::cout<<std::endl;
	}
	std::cout<<"Average nbrcycles:"<<nbrcycles/4<<std::endl;
	std::cout<<"Average nb seconds : "<<seconds/4<<std::endl;
	std::cout<<"Average nb milliseconds : "<<milliseconds/4<<std::endl;
	std::cout<<"Average CPI = "<<cpi/4<<std::endl;
	*/

	
	return 0;
}

