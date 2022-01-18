// test_couleur.cpp : Seuille une image en niveau de gris

#include <stdio.h>
#include "image_ppm.h"

void seuilAutomatique(char * cNomImgLue, char* cNomImgEcrite,int* S, int nbSeuil)
{
  int nH, nW, nTaille;
   OCTET *ImgIn, *ImgIn2, *ImgOut;
   
   lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
  
   allocation_tableau(ImgIn, OCTET, nTaille);
   lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgOut, OCTET, nTaille);

   allocation_tableau(ImgIn2, OCTET, nTaille);
   lire_image_pgm(cNomImgLue, ImgIn2, nH * nW);
unsigned int occurence [16]={0};

  for (int i=0; i < nH; i++)
  for (int j=0; j < nW; j++)
    ImgIn[i*nW+j]=(ImgIn[i*nW+j]/16);

  for (int i=0; i < nTaille; i++){
       occurence[ImgIn[i]]++;  
     }
  int *s = (int*)malloc((16)*sizeof(int)); 
  int k =0;
  
   for (int i=0; i < nH; i++)
   for (int j=0; j < nW; j++)
     {
       if(j>0 && j<(nW-1))
       {
        if(ImgIn[i*nW+j]<ImgIn[i*nW+j-1] && ImgIn[i*nW+j]<ImgIn[i*nW+j+1])
        {
          s[k]=ImgIn[i*nW+j];
          k++;
          printf("%d ",ImgIn[i*nW+j] );

        }
          
      }


     }
     
    for (int i=0; i < nH; i++)
   for (int j=0; j < nW; j++)
     {
      for(int p = 0;p<k;p++)
      {
        if(ImgIn[i*nW+j]<s[k-p-1])ImgOut[i*nW+j]=16*s[k-p-1];
      }
     }
     
  FILE *f_image;
        if( (f_image = fopen("histoAuto.dat", "wb")) == NULL)
   {
      exit(EXIT_FAILURE);
   }

      
    for(int i = 0 ; i < 16 ; i ++)                        
      fprintf(f_image,"%d %d\n", i, occurence[i]) ;
  ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
  free(ImgIn);

}


int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250],cNomImgEcrite2[250];
  int nH, nW, nTaille;
  int nbSeuil;
  
  
  if (argc != 4) 
     {
       printf("Usage: ImageIn.pgm ImageOut.pgm nbSeuil \n"); 
       exit (1) ;
     }
   
   sscanf (argv[1],"%s",cNomImgLue) ;
   sscanf (argv[2],"%s",cNomImgEcrite);
   sscanf (argv[3],"%d",&nbSeuil);
   //sscanf (argv[3],"%d",&S);

   int* S = (int*)malloc((nbSeuil)*sizeof(int)); 
  
   for(int i = 0 ; i < nbSeuil;i++)
   {
      S[i] = i* 256/nbSeuil;
      printf("%d \n", S[i]);

   }

   seuilAutomatique(cNomImgLue, cNomImgEcrite, S,  nbSeuil);
   
   return 1;
}
