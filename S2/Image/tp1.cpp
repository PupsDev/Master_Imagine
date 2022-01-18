// test_couleur.cpp : Seuille une image en niveau de gris

#include <stdio.h>
#include "image_ppm.h"

void seuilTroisParties(char * cNomImgLue, char* cNomImgEcrite,int* S)
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
       if ( ImgIn[i*nW+j] < S[0]) ImgOut[i*nW+j]=0; 
       else if ( ImgIn[i*nW+j] < S[1]) ImgOut[i*nW+j]=128; 
       else ImgOut[i*nW+j]=255;

     }
  ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
  free(ImgIn);

}
void seuilQuatreParties(char * cNomImgLue, char* cNomImgEcrite,int* S)
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
       if ( ImgIn[i*nW+j] < S[0]) ImgOut[i*nW+j]=0; 
       else if ( ImgIn[i*nW+j] < S[1]) ImgOut[i*nW+j]=64; 
       else if ( ImgIn[i*nW+j] < S[2]) ImgOut[i*nW+j]=128; else ImgOut[i*nW+j]=255;

     }
  ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
  free(ImgIn);

}

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250],cNomImgEcrite2[250];
  int nH, nW, nTaille;
  int nbSeuil=3;
  int* S = (int*)malloc((nbSeuil)*sizeof(int)); 
  
  if (argc != 4) 
     {
       printf("Usage: ImageIn.pgm ImageOut.pgm Seuil \n"); 
       exit (1) ;
     }
   
   sscanf (argv[1],"%s",cNomImgLue) ;
   sscanf (argv[2],"%s",cNomImgEcrite);
   sscanf (argv[3],"%s",cNomImgEcrite2);
   //sscanf (argv[3],"%d",&S);
  
   /*
   S[0]=100;
   S[1]=120;
   S[2]=140;
  */
   S[0]=200;
   S[1]=220;
   S[2]=230;

   seuilTroisParties(cNomImgLue, cNomImgEcrite,S);
   seuilQuatreParties(cNomImgLue, cNomImgEcrite2,S);


   
   return 1;
}
