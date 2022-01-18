// test_couleur.cpp : Seuille une image en niveau de gris

#include <stdio.h>
#include "image_ppm.h"

void seuilCouleur(char * cNomImgLue, char* cNomImgEcrite,int* S, int composante)
{
  /*
  R 0
  G 1
  B 2
  */
  int nH, nW, nTaille;
   OCTET *ImgIn, *ImgOut;
   
   lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
  
   allocation_tableau(ImgIn, OCTET, nTaille*3);
   lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgOut, OCTET, nTaille*3);



   int nHT = nH*3;
   int nWT = nW*3;
   for (int i=0; i < nHT; i+=3)
   for (int j=0; j < nWT; j+=3)
     {
      if ( ImgIn[i*nW+j+composante] < S[0])
      {
          ImgOut[i*nW+j+composante]=0;
          ImgOut[i*nW+j+(composante+1)%3]=0;
          ImgOut[i*nW+j+(composante+2)%3]=0;
      }
      else
      {
          ImgOut[i*nW+j+composante]=255;
        ImgOut[i*nW+j+(composante+2)%3]=255;
        ImgOut[i*nW+j+(composante+1)%3]=255;
     }

     }
  ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);
  free(ImgIn);

}


int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250],cNomImgEcrite2[250];
  int nH, nW, nTaille;
  int Seuil,composante;
  int nbSeuil=3;
  int* S = (int*)malloc((nbSeuil)*sizeof(int)); 
  
  if (argc != 5) 
     {
       printf("Usage: ImageIn.pgm ImageOut.pgm Seuil Composante \n"); 
       exit (1) ;
     }
   
   sscanf (argv[1],"%s",cNomImgLue) ;
   sscanf (argv[2],"%s",cNomImgEcrite);
   sscanf (argv[3],"%d",&Seuil);
   sscanf (argv[4],"%d",&composante);
   //sscanf (argv[3],"%d",&S);
  
   /*
   S[0]=100;
   S[1]=120;
   S[2]=140;
  */

   S[0]=100;


   seuilCouleur(cNomImgLue, cNomImgEcrite,S,composante);


   
   return 1;
}
