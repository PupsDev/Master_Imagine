int main() {
  // Create and open a text file
  Wave *waouwave = new Wave;
  unsigned char** data;
  int size=32670;
  char filename[20] = "GammePiano.wav";
  char filename2[20] = "NotesPiano.wav";
  char filename3[20] = "NotesPiano2.wav";
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
  int sampling_freq = 882;    
  long int data_nb=temps*sampling_freq;
  short data16[temps*sampling_freq]; // -> int
  double freq = notes[12];
  short amplitude = 30000;
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

  Wave *La = new Wave(data16,data_nb,1,sampling_freq);
  unsigned char *data2 = new unsigned char[32670];
  int data_nb2;
  Wave *gamme = new Wave();
  gamme->read(filename);
  data2 = gamme->getData8(&data_nb2);
  //printf("data_nb:%d", data_nb2);

  /*for(int i=0 ; i< data_nb2;i++)
  {
  	printf("%d\n ", data2[i]-127);
  }*/
/*
  double *signal = (double*)malloc(data_nb*sizeof(double));
  double *signal2 = (double*)malloc(data_nb*sizeof(double));
  double *partieR= (double*)malloc(data_nb*sizeof(double));
  double *partieI= (double*)malloc(data_nb*sizeof(double));
*/
  sampling_freq = 5012;
  data_nb = data_nb2;
  double *signal = (double*)malloc(data_nb*sizeof(double));
  double *signal2 = (double*)malloc(data_nb*sizeof(double));
  double *partieR= (double*)malloc(data_nb*sizeof(double));
  double *partieI= (double*)malloc(data_nb*sizeof(double));

  short *data162=(short*)malloc(data_nb*sizeof(short));

  /*for(int i=0 ; i< data_nb;i++)
  {
  	//printf("%d ", data16[i]);
    signal[i]= shortToDouble(data16[i]);
    //printf("%f\n", signal[i]);
    //printf("%d\n", doubleToShort(signal[i]));
  }*/
   for(int i=0 ; i< data_nb;i++)
  {

    signal[i]= ucharToDouble(data2[i]);
  }
  
  //DFT(signal, partieR, partieI,data_nb);
  //TFD(signal2, partieR, partieI,data_nb);

   /*for(int i=0 ; i< data_nb;i++)
  {
    data16[i]= doubleToShort(signal2[i]);
  }
  */
  for(int i=0 ; i< data_nb;i++)
  {
  	//printf("%f->", signal2[i]);
    data162[i]= doubleToShort(signal2[i])/2;
    //printf("%d\n",data162[i]);
  }
  
   Wave *La2 = new Wave(data162,data_nb,1,sampling_freq);
   /*
  for(int i=0 ; i< data_nb;i++)
  {
    printf("%f ->",signal[i]);
    printf("%f\n",signal2[i]);
  }*/

  La->write(filename2);
  La2->write(filename3);
  printf("%ld\n",computeM(3201));



} 