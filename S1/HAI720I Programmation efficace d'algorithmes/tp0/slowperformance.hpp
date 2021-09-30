#define C1 0.2
#define C2 0.3 

void slowperformance1 (float *x, const float *y, const float *z, int n)
{
	for(int i=0; i< n-2; i++)
	{
		x[i] = x[i] / M_SQRT2 + y[i] * C1;
		x[i+1] += z[(i%4)*10] * C2;
		x[i+2] += sin((2*M_PI *i) / 3)*y[i+2];
	}
}
void slowperformance2 (float *x, const float *y, const float *z, int n)
{
	for(int i=0; i< n-2; i++)
	{
		x[i] = x[i] / M_SQRT2 + y[i] * C1;
		
	}
}

void slowperformance3 (float *x, const float *y, const float *z, int n)
{
	for(int i=0; i< n-2; i+=3)
	{
		x[i] = x[i] / M_SQRT2 + y[i] * C1;
		x[i+1] += z[(i%4)*10] * C2;
		x[i+2] += sin((2*M_PI *i) / 3)*y[i+2];

		x[i] = x[i] / M_SQRT2 + y[i] * C1;
		x[i+1] += z[(i%4)*10] * C2;
		x[i+2] += sin((2*M_PI *i) / 3)*y[i+2];

		x[i] = x[i] / M_SQRT2 + y[i] * C1;
		x[i+1] += z[(i%4)*10] * C2;
		x[i+2] += sin((2*M_PI *i) / 3)*y[i+2];
		
	}
}