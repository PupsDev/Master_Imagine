#include <iostream>
#include <fstream>
#include <string>
#include "Wave.hpp"
#include <math.h>
using namespace std;

void DFT(double *signal, double *partieR, double *partieI,int N);
void IDFT(double *signal, double *partieR, double *partieI,int N);

int main() {
  // Create and open a text file
  Wave *waouwave = new Wave;
  unsigned char** data;
  int size=32670;
  char filename[20] = "GammePiano.wav";
  char filename2[20] = "LaPiano.wav";
  //waouwave->read(filename);
  //waouwave->getData8(data, &size);
  /*for(int i = 0 ; i < size ; i++)
    cout<<data[i]<<endl;*/
  // Y = (unsigned char)floor(127.5(x+1.0));
  // X = ((double y-127.5))127.5 = ((double )y/127.5)-1.0
       
  long int data_nb=6*44100;
  short data16[6*44100];
  int freq = 440;
  short amplitude = 6000;

  for(int i = 0 ; i < data_nb ; i ++)
  {
    data16[i]=amplitude*sin(2*M_PI*freq*i/44100);
    if(i%44100==0)
      freq*=2;
  }

  Wave *La = new Wave(data16,data_nb,1,44100);
  La->write(filename2);

  cout<<"Salut les waou waves"<<endl;

} 