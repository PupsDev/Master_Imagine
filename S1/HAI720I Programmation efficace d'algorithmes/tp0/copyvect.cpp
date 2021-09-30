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