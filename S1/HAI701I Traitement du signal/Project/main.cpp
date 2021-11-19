#include <iostream>
#include <fstream>
#include <string>
#include "Wave.hpp"
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
double shortToDouble(short s)
{

  return (double)(((double)s/32767.) -1.0);

}
short doubleToShort(double s)
{
  if(s >=1.0)
    return 255;
  else if( s <=-1.0)
    return 0;
  else
   return (short)floor((s+1.0) *127.5);

}
int main() {
  // Create and open a text file
  Wave *waouwave = new Wave;
  unsigned char** data;
  int size=32670;
  char filename[20] = "GammePiano.wav";
  char filename2[20] = "NotesPiano.wav";
  //waouwave->read(filename);
  //waouwave->getData8(data, &size);
  /*for(int i = 0 ; i < size ; i++)
    cout<<data[i]<<endl;*/
  // Y = (unsigned char)floor(127.5(x+1.0));
  // X = ((double y-127.5))127.5 = ((double )y/127.5)-1.0

  double notes[13] = { 880,
830.609,
783.991,
739.989,
698.456,
659.255,
622.254,
587.33,
554.365,
523.251,
493.883,
466.164,
440};


  int temps=1;
  //int sampling_freq = 44100;  
  int sampling_freq = 4410;    
  long int data_nb=temps*sampling_freq;
  short data16[temps*sampling_freq]; // -> int
  double freq = notes[12];
  short amplitude = 6000;
  int a =11;
  for(int i = 0 ; i < data_nb ; i ++)
  {
    data16[i]=amplitude*sin(2*M_PI*freq*i/sampling_freq);
    if(i%sampling_freq==0)
    {
      cout<<"Changement de ton"<<a<<endl;
      freq=notes[a];
      a--;
      if(a<0)a=0;

    }
  }

  //Wave *La = new Wave(data16,data_nb,1,sampling_freq);

  Wave *gamme = new Wave();//data16,data_nb,1,sampling_freq);
  gamme->read(filename);
  //data_nb = 4;
  sampling_freq = 11025;

  unsigned char **datag (unsigned char **)malloc(data_nb*sizeof(unsigned char *));;
  gamme->getData8(datag, &sampling_freq);

  double *signal = (double*)malloc(data_nb*sizeof(double));
  double *signal2 = (double*)malloc(data_nb*sizeof(double));
  double *partieR= (double*)malloc(data_nb*sizeof(double));
  double *partieI= (double*)malloc(data_nb*sizeof(double));

  for(int i = 0 ; i < sampling_freq;i++)
  {
    printf("%d\n",datag[i]);

  }

  /*for(int i = 0 ; i < data_nb;i++)
  {
    signal[i]=(double)(i+1)*(i+1);

  }*/
/*
  for(int i=0 ; i< data_nb;i++)
  {
    signal[i]= shortToDouble(data16[i]);
  }*/
  /*
  DFT(signal, partieR, partieI,data_nb);
  TFD(signal2, partieR, partieI,data_nb);*/
  /*for(int i=0 ; i< data_nb;i++)
  {
    printf("%f ->",signal[i]);
    printf("%f\n",signal2[i]);
  }*/

  //La->write(filename2);

  //cout<<"Salut les waou waves"<<endl;

} 