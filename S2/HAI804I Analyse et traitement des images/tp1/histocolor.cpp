
#include <stdio.h>
#include "image_ppm.h"

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250];
  int nH, nW, nTaille, S;
  unsigned int occurenceR [256]={0};
  unsigned int occurenceG [256]={0};
  unsigned int occurenceB [256]={0};

  if (argc != 2) 
     {
       printf("Usage: ImageIn.pgm\n"); 
       exit (1) ;
     }
   sscanf (argv[1],"%s",cNomImgLue) ;

   OCTET *ImgIn, *ImgOut;
   
   lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
    printf("%d %d\n", nH , nW); 
  
   allocation_tableau(ImgIn, OCTET, nTaille*3);
   lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgOut, OCTET, nTaille*3);
    int nTaille3 = nTaille*3;
	 for (int i=0; i < nTaille3; i+=3)
     {

       occurenceR[ImgIn[i]]++;
       occurenceG[ImgIn[i+1]]++;
       occurenceB[ImgIn[i+2]]++;

     }

    
    for (int i=0; i < 256; i++)
    {
     	printf("%d ", occurenceR[i]);
        printf("%d ", occurenceG[i]);
        printf("%d\n", occurenceB[i]);
    }


    FILE *f_image;

   if( (f_image = fopen("histoCouleur.dat", "wb")) == NULL)
   {
      exit(EXIT_FAILURE);
   }

      
	for(int i = 0 ; i < 256 ; i ++)                        
	 	fprintf(f_image,"%d %d %d %d\n", i, occurenceR[i],occurenceG[i],occurenceB[i]) ;
        

	fclose(f_image);
   free(ImgIn); free(ImgOut);

   return 1;
}
