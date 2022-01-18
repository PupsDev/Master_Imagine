
#include <stdio.h>
#include "image_ppm.h"

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250];
  int nH, nW, nTaille, S;
  unsigned int occurence [256]={0};
  
  if (argc != 1) 
     {
       printf("Usage: ImageIn.pgm\n"); 
       exit (1) ;
     }
   sscanf (argv[1],"%s",cNomImgLue) ;

   OCTET *ImgIn, *ImgOut;
   
   lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
    printf("%d %d\n", nH , nW); 
  
   allocation_tableau(ImgIn, OCTET, nTaille);
   lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgOut, OCTET, nTaille);
	
   for (int i=0; i < nTaille; i++){
       occurence[ImgIn[i]]++;  
     }
    
    for (int i=0; i < 256; i++)
     	printf("%d\n", occurence[i]);


    FILE *f_image;

   if( (f_image = fopen("histo.dat", "wb")) == NULL)
   {
      exit(EXIT_FAILURE);
   }

      
	  for(int i = 0 ; i < 256 ; i ++)                        
	 		fprintf(f_image,"%d %d\n", i, occurence[i]) ;

	fclose(f_image);
   ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
   free(ImgIn); free(ImgOut);

   return 1;
}
