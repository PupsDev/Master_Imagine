// test_couleur.cpp : Seuille une image en niveau de gris

#include <stdio.h>
#include "image_ppm.h"

void profil(char * cNomImgLue, int choix, int indice)
{
  int nH, nW, nTaille;
  OCTET *ImgIn, *ImgOut;
  FILE* f_image;
   
  lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;
  
  allocation_tableau(ImgIn, OCTET, nTaille);
  lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
  allocation_tableau(ImgOut, OCTET, nTaille);
  if( (f_image = fopen("profil.dat", "wb")) == NULL)
  {
    exit(EXIT_FAILURE);
  }             
      

  
  if(choix == 0) // ligne
  {

      for (int j=0; j < nW; j++)
      {
        fprintf(f_image,"%d %d\n", j, ImgIn[indice*nW+j]);

      }

  }
  else // colonne
  {
      for (int i=0; i < nH; i++)
      {
        fprintf(f_image,"%d %d\n", i, ImgIn[i*nW+indice]);

      }

  }


  free(ImgIn);
   fclose(f_image);

}


int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250],cNomImgEcrite2[250];
  int nH, nW, nTaille;
  int choix;
  int indice;
  int nbSeuil=3;
  int* S = (int*)malloc((nbSeuil)*sizeof(int)); 
  
  if (argc != 4) 
     {
       printf("Usage: ImageIn.pgm LOUC indice  \n"); 
       exit (1) ;
     }
   
   sscanf (argv[1],"%s",cNomImgLue) ;
   sscanf (argv[2],"%d",&choix);
   sscanf (argv[3],"%d",&indice);
   //sscanf (argv[3],"%d",&S);

   profil(cNomImgLue, choix, indice);



   
   return 1;
}
