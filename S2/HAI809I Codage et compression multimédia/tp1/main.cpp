// test_couleur.cpp : Seuille une image couleur 

#include <stdio.h>
#include "image_ppm.h"
#include <time.h>
#include <cmath>

struct couleur
{
  int r,g,b;
};
typedef couleur couleur;

float distance(int r, int g, int b, int r1,int g1,int b1)
{
  return sqrt( (r-r1)*(r-r1) + (b-b1)*(b-b1)+ (g-g1)*(g-g1));
}
int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250],cNomImgEcrite2[250];
  int nH, nW, nTaille,S,nR,nG,nB,ite;
  srand( time( NULL ) );
  
  if (argc != 5) 
     {
       printf("Usage: ImageIn.ppm  ImageIn.ppm  ImageIn.ppm\n"); 
       exit (1) ;
     }
   
   sscanf (argv[1],"%s",cNomImgLue) ;
   sscanf (argv[2],"%s",cNomImgEcrite);
   sscanf (argv[3],"%s",cNomImgEcrite2);
   sscanf (argv[4],"%d",&ite);
   //sscanf (argv[3],"%d",&S);

   OCTET *ImgIn, *ImgOut,*ImgOut2;
   
   lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
  
   int nTaille3 = nTaille * 3;
   allocation_tableau(ImgIn, OCTET, nTaille3);
   lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgOut, OCTET, nTaille3);
   allocation_tableau(ImgOut2, OCTET, nTaille3);
	
   // On choisit C1 et C2 au hasard 
   int p = rand()%nTaille3;
   int p2 = rand()%nTaille3;

   int n2R = ImgIn[p];
   int n2G = ImgIn[p+1];
   int n2B = ImgIn[p+2];

   int n3R = ImgIn[p2];
   int n3G = ImgIn[p2+1];
   int n3B = ImgIn[p2+2];

   int m1r,m1g,m1b,m2r,m2g,m2b;


   int *classe = (int*)malloc(nTaille3 * sizeof(int));
    
    char cIt[250];

   
   for(int it =0; it<10;it++)
   {
     char cNomImg[250]="out/iterationMoy";
     char cNomImg2[250]="out/iteration2Mea";
    sprintf(cIt, "%d", it);
    strcat(cNomImg,cIt);
    strcat(cNomImg,".ppm");

    strcat(cNomImg2,cIt);
    strcat(cNomImg2,".ppm");

    printf("C1: %d %d %d C2 %d %d %d \n", n2R,n2G,n2B, n3R,n3G,n3B);
    
       int s1=0,s2=0;
   m1r=m1g=m1b=m2r=m2g=m2b=0;
    for (int i=0; i < nTaille3; i+=3)
     {
      nR = ImgIn[i];
      nG = ImgIn[i+1];
      nB = ImgIn[i+2];
      if(distance(nR,nG,nB,n2R,n2G,n2B)<distance(nR,nG,nB,n3R,n3G,n3B)) // pres de c1
      {
         ImgOut[i]=n2R;
         ImgOut[i+1]=n2G; 
         ImgOut[i+2]=n2B;

         m1r+=nR;
         m1g+=nG;
         m1b+=nB;
         s1++;
         classe[i]=0;
      }
      else
      {
         ImgOut[i]=n3R;
         ImgOut[i+1]=n3G; 
         ImgOut[i+2]=n3B;

         m2r+=nR;
         m2g+=nG;
         m2b+=nB;
         s2++;
         classe[i]=1;
      }
     }

        m1r/=s1;
        m1g/=s1;
        m1b/=s1;

        m2r/=s2;
        m2g/=s2;
        m2b/=s2;
        for (int i=0; i < nTaille3; i+=3)
       {
        
        if(classe[i])
        {
          


          ImgOut2[i]=m1r;
          ImgOut2[i+1]=m1g; 
          ImgOut2[i+2]=m1b;

        }
        else
        {
          ImgOut2[i]=m2r;
          ImgOut2[i+1]=m2g; 
          ImgOut2[i+2]=m2b;

        }
        

       }
        printf("C1: %d %d %d C2 %d %d %d \n\n", m1r,m1g,m1b, m2r,m2g,m2b);
        n2R = m1r;
        n2G = m1g;
        n2B = m1b;

        n3R = m2r;
        n3G = m2g;
        n3B = m2b;
        
        ecrire_image_ppm(cNomImg, ImgOut,  nH, nW);
        ecrire_image_ppm(cNomImg2, ImgOut2,  nH, nW);
   }
   /*
      for (int i=0; i < nTaille3; i+=3)
     {
      
      if(classe[i])
      {
        


        ImgOut2[i]=n2R;
        ImgOut2[i+1]=n2G; 
        ImgOut2[i+2]=n2B;

      }
      else
      {
        ImgOut2[i]=n3R;
        ImgOut2[i+1]=n3G; 
        ImgOut2[i+2]=n3B;

      }
      

     }*/
   /*
   
   for (int i=0; i < nTaille3; i+=3)
     {
      
      if(classe[i])
      {
        


        ImgOut2[i]=m1r;
        ImgOut2[i+1]=m1g; 
        ImgOut2[i+2]=m1b;

      }
      else
      {
        ImgOut2[i]=m2r;
        ImgOut2[i+1]=m2g; 
        ImgOut2[i+2]=m2b;

      }
      

     }*/
     
   

   //ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);
    //ecrire_image_ppm(cNomImgEcrite2, ImgOut2,  nH, nW);
   free(ImgIn);
   return 1;
}
