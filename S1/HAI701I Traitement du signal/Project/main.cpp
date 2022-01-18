#include <iostream>
#include <fstream>
#include <string>
#include "wave.hpp"
#include <math.h>
using namespace std;


void IDFT(double *signal, double *partieR, double *partieI,int N);
/*
880
830,609
783,991
739,989
698,456
659,255
622,254
587,33
554,365
523,251
493,883
466,164
440
*/
void DFT(double *signal, double *partieR, double *partieI,int N)
{

  double coeff = 2*M_PI;
  for(int i =0 ; i < N; i++)
  {
      partieR[i]=0;
      partieI[i]=0;
      for(int n =0 ; n < N; n++)
      {
          double theta = coeff*i*n/(double)N;
          partieR[i]+=signal[n]*cos(theta);
          partieI[i]-=signal[n]*sin(theta);
      }
      
  }

}
void TFD(double *signal, double *partieR, double *partieI,int N)
{

  double coeff = 2*M_PI;
  
  for(int i =0 ; i < N; i++)
  {
      signal[i]=0;
      for(int n =0 ; n < N; n++)
      {
        double theta = (coeff*i*n)/(double)N;
        signal[i]+= (partieR[n]*cos(theta)-partieI[n]*sin(theta));
      }
      signal[i]/=(double)N;

  }

}
// 0 - 255 -127 +128
// -1 -  1
// 65535
// 32767
double shortToDouble(short s)
{

  return (double)(((double)s/32767.) );

}
short doubleToShort(double s)
{/*
  if(s >=1.0)
    return 5999;
  else if( s <=-1.0)
    return -5999;
  else if(s < 0)
   return (short)floor((s+1.0) *3000);
  else*/
  	return (short)(s *32767);

}
double ucharToDouble(unsigned char s)
{
	return (double)(s/127.)-1.;
}
unsigned char doubleToUchar(double s)
{
	if(s >=1.0)
    	return 127;
  	else if( s <=-1.0)
    	return -127;
  	else
   		return (unsigned char)floor((s+1.0) *127);
}
int FFT(int dir,int m,double *x,double *y)
{
  int n,i,i1,j,k,i2,l,l1,l2;
  double c1,c2,tx,ty,t1,t2,u1,u2,z;
  
  /* Calculate the number of points */
  n = 1;
  for (i=0;i<m;i++) 
    n *= 2;
  
  /* Do the bit reversal */
  i2 = n >> 1;
  j = 0;
  for (i=0;i<n-1;i++) {
    if (i < j) {
      tx = x[i];
      ty = y[i];
      x[i] = x[j];
      y[i] = y[j];
      x[j] = tx;
      y[j] = ty;
    }
    k = i2;
    while (k <= j) {
      j -= k;
      k >>= 1;
    }
    j += k;
  }
  
  /* Compute the FFT */
  c1 = -1.0; 
  c2 = 0.0;
  l2 = 1;
  for (l=0;l<m;l++) {
    l1 = l2;
    l2 <<= 1;
    u1 = 1.0; 
    u2 = 0.0;
    for (j=0;j<l1;j++) {
      for (i=j;i<n;i+=l2) {
        i1 = i + l1;
        t1 = u1 * x[i1] - u2 * y[i1];
        t2 = u1 * y[i1] + u2 * x[i1];
        x[i1] = x[i] - t1; 
        y[i1] = y[i] - t2;
        x[i] += t1;
        y[i] += t2;
      }
      z =  u1 * c1 - u2 * c2;
      u2 = u1 * c2 + u2 * c1;
      u1 = z;
    }
    c2 = sqrt((1.0 - c1) / 2.0);
    if (dir == 1) 
      c2 = -c2;
    c1 = sqrt((1.0 + c1) / 2.0);
  }
  
  /* Scaling for forward transform */
  if (dir == 1) {
    for (i=0;i<n;i++) {
      x[i] /= n;
      y[i] /= n;
    }
  }
  
  return(1);
}

int computeM(int N )
{
  int result =0;
  for(int i = 0 ; i < N ; i++)
  {
    if( (2<<i) > N)
      return i;
  }
  return result;
}
void printSignal(double * signal, int data_nb)
{
  for(int i=0 ; i< data_nb;i++)
  {

    printf("%f\n",signal[i]);
  }
}
  double notes[13] = 
  { 
  880,      //  LA  0
  830.609,  //  LAB 1
  783.991,  //  SOL 2  
  739.989,  //  SOLB 3
  698.456,  //  FA 4
  659.255,  //  MI 5
  622.254,  //  MIB 6
  587.33,   //  RE 7
  554.365,  //  REB 8
  523.251,  //  Do 9 
  493.883,  //  SI 10
  466.164,  //  SIB 11
  440};     //  LA 12


void sawsignal2(short * data16, int data_nb)
{
  double temps=10.;
  double sampling_freq = 882; 
  double freq = notes[12];
  short amplitude = 30000;
  int a =0;
  float pp =(float)2./(float)M_PI;
  int N = 25;
  for(int i = 0 ; i < data_nb ; i ++)
  {
    data16[i]=0;
    
    for(int k = 0 ; k< N; k++)
    {
      data16[i]=+ pow(-1,k+1)*amplitude*sin(2*M_PI*k*freq*i/sampling_freq)/k;

        freq=notes[12];
    }
    data16[i]= data16[i]*pp;
  }

}
short  computeSignalSquare(short amplitude, double freq ,double t)
{
    short data16;

    data16=amplitude*sin(2*M_PI*freq*t);

    //if(data16>=0) data16=amplitude;
    //else data16=-amplitude;

    return data16;
}
short  computeSignalTriangle(short amplitude, double freq ,double t)
{

    short data16=0;
    int N = 50;
    float pp =(float)2./(float)M_PI;
    for(int k = 0 ; k< N; k++)
    {
      data16=+ pow(-1,k+1)*amplitude*sin(2*M_PI*k*freq*t)/k;
      printf("%d\n",data16);
    }
    return data16*pp;
}

void squaresignal(short * data16, int data_nb)
{
  double temps=1.;
  double sampling_freq = 44100; 
  double freq = notes[12];
  double t;
  short amplitude = 30000;
  int a =0;
  float pp =(float)2./(float)M_PI;
  freq=notes[12];

  for(int i = 0 ; i < data_nb ; i ++)
  {
    t = i/sampling_freq;
    data16[i] = computeSignalSquare(amplitude,freq,t);

  
    if(i%(int)(sampling_freq/10)==0 && i!=0)
    {
      freq=notes[12-a];
      a++;
      if(a>9)a=0;

    }
    

  }

}
void makeANote(short * data16, size_t note, int data_nb)
{
  double temps=1.;
  double sampling_freq = 44100; 
  double freq=notes[note];
  double t;
  short amplitude = 30000;
  int a =0;
  float pp =(float)2./(float)M_PI;
  
  for(int i = 0 ; i < data_nb ; i ++)
  {
    t = i/sampling_freq;
    data16[i] = computeSignalSquare(amplitude,freq,t);   

  }

}
void filter(int windowSize,double sampling_freq, double *real,double *imaginary)
{

  int halfFFTsize = windowSize / 2;
  float lowpassFreq1 = 700.0;
  float lowpassFreq2 = 880.0;
  for (int i = 0; i < halfFFTsize; i++)
  {
      int ineg = windowSize - 1 - i;
      float freq = (float)i * (sampling_freq / (float)halfFFTsize);
      if (freq >= lowpassFreq1 )
      {

          real[i] = 0;
          imaginary[i] = 0;
          real[ineg] = 0;
          imaginary[ineg] = 0;
      }
      else if (freq >= lowpassFreq2)
      {

          float mult = 1.0 - ((freq - lowpassFreq1) / (lowpassFreq2 - lowpassFreq1));
          real[i] *= mult;
          imaginary[i] *= mult;
          real[ineg] *= mult;
          imaginary[ineg] *= mult;
      }

  }
}

void discFilter()
{
  //float omega = 2*M_PI*freq;
}
int main() {

  Wave *waouwave = new Wave;
  unsigned char** data;
  int size=32670;
  char filename[20] = "GammePiano.wav";
  char filename2[20] = "NotesPiano.wav";
  char filename3[20] = "NotesPiano2.wav";



  double temps=1.;
  double sampling_freq = 44100.;    
  long int data_nb=(long int)(temps*sampling_freq);
  short data16[(int)(temps*(int)sampling_freq)]; // -> int
  double freq = notes[12];
  short amplitude = 30000;
  //int a =0;
  /*
  for(int i = 0 ; i < data_nb ; i ++)
  {
    data16[i]=amplitude*sin(2*M_PI*freq*i/sampling_freq);
    if(i%(int)sampling_freq==0)
    {
      freq=notes[a];
      a--;
      if(a<0)a=0;

    }
  }*/
  /*
  int a =0;
  for(int i = 0 ; i < data_nb ; i ++)
  {
    data16[i]=amplitude*sin(2*M_PI*freq*i/sampling_freq);
    if(i%(int)sampling_freq==0 && i!=0)
    {
      freq=notes[12-sheetmusic[a]];
      a++;
      if(a>9)a=0;

    }
  }
  */
  //squaresignal(data16, data_nb);
  short data162[(int)(temps*(int)sampling_freq)];
  short data163[(int)(temps*(int)sampling_freq)];

  makeANote(data16, 12, data_nb);
  makeANote(data162, 9, data_nb);
  makeANote(data163, 5, data_nb);

  Wave *La = new Wave(data16,data_nb,1,sampling_freq);
  int m = computeM(data_nb); 
  int dir = 1;
  //short data162[(int)(temps*(int)sampling_freq)];

  char filenameSoundBefore[20] = "SoundBefore.wav";
  char filenameSoundAfter[20] = "SoundAfter.wav";


  double *partieR= (double*)malloc(data_nb*sizeof(double));
  double *partieI= (double*)malloc(data_nb*sizeof(double));

  double *partieR2= (double*)malloc(data_nb*sizeof(double));
  double *partieI2= (double*)malloc(data_nb*sizeof(double));

  double *partieR3= (double*)malloc(data_nb*sizeof(double));
  double *partieI3= (double*)malloc(data_nb*sizeof(double));

  La->write(filenameSoundBefore);
  for(int i=0 ; i< data_nb;i++)
  {

    partieR[i]= shortToDouble(data16[i]);
  }
    for(int i=0 ; i< data_nb;i++)
  {

    partieR2[i]= shortToDouble(data162[i]);
  }
    for(int i=0 ; i< data_nb;i++)
  {

    partieR3[i]= shortToDouble(data163[i]);
  }

  FFT(dir, m,partieR,partieI);
  FFT(dir, m,partieR2,partieI2);
  FFT(dir, m,partieR3,partieI3);

  //filter(44100, sampling_freq, partieR,partieI);
  /*for(int i=0 ; i< data_nb;i++)
  {
    printf("%f\n",partieR[i]);
  }*/
  for(int i=0 ; i< data_nb;i++)
  {

    partieR[i]+= partieR2[i] + partieR3[i] ;
    partieR[i]/=3.;
  }


  FFT(-dir, m,partieR,partieI);

  for(int i=0 ; i< data_nb;i++)
  {
    data162[i]= doubleToShort(partieR[i]);
  }

  Wave *La2 = new Wave(data162,data_nb,1,sampling_freq);

  La2->write(filenameSoundAfter);


} 