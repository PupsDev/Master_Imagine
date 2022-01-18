// test_couleur.cpp : Seuille une image en niveau de gris

#include <stdio.h>
#include "image_ppm.h"

void seuilAutomatique(char * cNomImgLue, char* cNomImgEcrite,int* S, int nbSeuil)
{
  int nH, nW, nTaille;
   OCTET *ImgIn, *ImgOut;
   
   lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
  
   allocation_tableau(ImgIn, OCTET, nTaille);
   lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgOut, OCTET, nTaille);



   for (int i=0; i < nH; i++)
   for (int j=0; j < nW; j++)
     {
       
       if( ImgIn[i*nW+j]<S[0])ImgOut[i*nW+j]=0; 
      for(int s = 1 ; s < nbSeuil-1; s++)
      {

       if( ImgIn[i*nW+j]<S[s])
        {
          ImgOut[i*nW+j]=S[s];
          break;
        }
      }
       if( ImgIn[i*nW+j]>S[nbSeuil-1])
        {
         ImgOut[i*nW+j]=255; 
       }


     }
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
