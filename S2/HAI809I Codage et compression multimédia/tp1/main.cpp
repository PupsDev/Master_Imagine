// test_couleur.cpp : Seuille une image couleur 

#include <stdio.h>
#include "image_ppm.h"
#include <time.h>
#include <cmath>
#include <map>
#include <bits/stdc++.h>
using namespace std;
#define PALETTE 2
struct couleur
{
  int r,g,b;
};
typedef couleur couleur;

float distance(int r, int g, int b, int r1,int g1,int b1)
{
    return sqrt( (r-r1)*(r-r1) + (b-b1)*(b-b1)+ (g-g1)*(g-g1));
}
float distance(couleur c1,couleur c2)
{
    return distance(c1.r,c1.g,c1.b, c2.r,c2.g,c2.b);
}
int nearestClust(couleur *couleurArr, couleur c)
{
  int min;
  float dmin = 10000000.;
  for(int i =0 ; i<PALETTE;i++)
  {
    if(distance(couleurArr[i],c)<dmin)
    {
      min = i;
      dmin = distance(couleurArr[i],c);
    }
  }
  return min;
}
void print(couleur c)
{
  printf("%d %d %d \n", c.r,c.g,c.b);
}
bool equalCouleur(couleur c1, couleur c2)
{
  return c1.r==c2.r&&c1.g==c2.g&&c1.b==c2.b;
}
float psnr(OCTET* ImgIn,OCTET* ImgOut, int nH, int nW)
{
  int d =PALETTE-1;
  float diff =0.;
  for(int i =0; i < nW*nW ; i++)
  {
    diff += sqrt( (ImgIn[i]-ImgOut[i])*(ImgIn[i]-ImgOut[i]) + (ImgIn[i+1]-ImgOut[i+1])* (ImgIn[i+1]-ImgOut[i+1]) +(ImgIn[i+2]-ImgOut[i+2])*(ImgIn[i+2]-ImgOut[i+2]));
  }

  float EQM = 1./(nW*nW) * diff;
  return 10*log10(3*d*d / EQM);
}
bool cmp(pair<int,couleur* >& a,
         pair<int,couleur* >& b)
{
    
    return (a.second->r + a.second->g+a.second->b)/3 <(b.second->r + b.second->g+b.second->b)/3;
}
void sort(std::map<int,couleur* >& M)
{
  
    // Declare vector of pairs
    std::vector<pair<int,couleur* > > A;
  
    // Copy key-value pair from Map
    // to vector of pairs
    for (auto& it : M) {
        A.push_back(it);
    }
  
    // Sort using comparator function
    sort(A.begin(), A.end(), cmp);
    for (auto& it : A) {
          std::cout << it.first<<"->" // string's value 
          << it.second->r<< ':' <<it.second->g<< ':' <<it.second->b

          << std::endl;

    }

}

int main(int argc, char* argv[])
{

    char cNomImgLue[250],cNomImgEcrite[250];
    int nH, nW, nTaille;
    srand( time( NULL ) );
    OCTET *ImgIn, *ImgOut,*ImgOutNdg,*ImgOutNdgAvg,*ImgPalette;
    if (argc != 3) 
    {
        printf("Usage: ImageIn.ppm ImageOut.ppm\n"); 
        exit (1) ;
    }

    sscanf (argv[1],"%s",cNomImgLue) ;
    sscanf (argv[2],"%s",cNomImgEcrite);
    

    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;
    int nTaille3 = nTaille * 3;
    allocation_tableau(ImgIn, OCTET, nTaille3);
      
    lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
    allocation_tableau(ImgOut, OCTET, nTaille3);
    allocation_tableau(ImgOutNdg, OCTET, nTaille);
     allocation_tableau(ImgOutNdgAvg, OCTET, nTaille);

    allocation_tableau(ImgPalette, OCTET, nTaille3);
    int classNumber=PALETTE;

    couleur* classColors= (couleur*)malloc(classNumber * sizeof(couleur));

    int* classe= (int*)malloc(nTaille3 * sizeof(int));
    int* moyenneNDG= (int*)malloc(classNumber * sizeof(int));

    couleur* moyenne= (couleur*)malloc(classNumber * sizeof(couleur));
    int* somme= (int*)malloc(classNumber * sizeof(int));

     std::map<int,couleur*> M;

    for(int i=0 ; i <classNumber; i++)
    {
         int p = rand()%nH;
         int q = rand()%nW;
         p = p*q;
         //printf("test:%d\n",p);
         classColors[i].r = ImgIn[p];
         classColors[i].g = ImgIn[p+1];
         classColors[i].b = ImgIn[p+2];
         //print(classColors[i]);
         //M.insert(std::pair<couleur*,int>(&classColors[i],(classColors[i].r + classColors[i].g+classColors[i].b)/3));
         moyenne[i].r=0;
         moyenne[i].g=0;
         moyenne[i].b=0;
         somme[i]=0;
    }

     for(int it =0 ;it <1 ;it++)
    {
      for (int i=0; i < nTaille3; i+=3)
      {
          couleur c;
          c.r = ImgIn[i];
          c.g = ImgIn[i+1];
          c.b = ImgIn[i+2];

          int k = nearestClust(classColors, c); // pres de ck
          //printf("classe:%d\n",k);
          //ImgOutNdg[i/3]=k;
          //ImgOutNdgAvg[i/3]=(classColors[k].r + classColors[k].g+classColors[k].b)/3;
          ImgOut[i]=classColors[k].r;
          ImgOut[i+1]=classColors[k].g; 
          ImgOut[i+2]=classColors[k].b;

          moyenne[k].r+=c.r;
          moyenne[k].g+=c.g;
          moyenne[k].b+=c.b;
          somme[k]++;
          classe[i]=k;
      }
      for(int j=0 ; j <classNumber; j++)
      {
        if(somme[j]!=0)
        {
          moyenne[j].r/=somme[j];
          moyenne[j].g/=somme[j];
          moyenne[j].b/=somme[j];

        }
        //printf("itération : %d\n", it);

        //print(classColors[j]);
        //print(moyenne[j]);
      }
      
      for(int j=0 ; j <classNumber; j++)
      {
        classColors[j].r=moyenne[j].r;
        classColors[j].g=moyenne[j].g;
        classColors[j].b=moyenne[j].b;
        moyenne[j].r=0;
        moyenne[j].g=0;
        moyenne[j].b=0;
        somme[j]=0;
      }
    }

    


    FILE *f_palette;

    if( (f_palette = fopen("out/palette.palet", "wb")) == NULL)
   {
      exit(EXIT_FAILURE);
   }

    for(int i = 0 ; i < PALETTE ; i ++)                        
      fprintf(f_palette,"%d %d %d %d\n", i, classColors[i].r,classColors[i].g,classColors[i].b) ;
    fclose(f_palette);

    for(int i=0 ; i <classNumber; i++)
    {

         M.insert(std::pair<int,couleur*>(i,&classColors[i]));
      }
    int k=0;
    //printMap(M);
    printf("\nSORT\n");
        std::vector<pair<int,couleur* > > A;
  
    // Copy key-value pair from Map
    // to vector of pairs
    for (auto& it : M) {
        A.push_back(it);
    }
  
    // Sort using comparator function
    sort(A.begin(), A.end(), cmp);
       /* for (auto& it : A) {
          std::cout << it.first<<"->" // string's value 
          << it.second->r<< ':' <<it.second->g<< ':' <<it.second->b

          << std::endl;

    }*/
    std::vector<int> indices;
    for (auto& it : A) {
        indices.push_back(it.first);
    }
    //printMap(M);
   auto it = M.begin();
    for (int i=0; i < nTaille3; i+=3)
    { 

          if(i/1536<PALETTE)
          {
            std::advance(it, i/1536);

            ImgPalette[i]=  classColors[indices[i/1536]].r;
            ImgPalette[i+1]=classColors[indices[i/1536]].g; 
            ImgPalette[i+2]=classColors[indices[i/1536]].b;
            k++;
             //it = M.begin();

          }

    }

    ecrire_image_pgm(cNomImgEcrite, ImgOutNdg,  nH, nW);
    for (int i=0; i < nTaille3; i+=3)
    {  
      ImgOutNdg[i/3]=indices[ ImgOutNdg[i/3]];
    }
    
    char cNomNDG[250]="out/sorted.pgm";
    ecrire_image_pgm(cNomNDG, ImgOutNdg,  nH, nW);

    char cNomNDGAvg[250]="out/average.pgm";
    ecrire_image_pgm(cNomNDGAvg, ImgOutNdgAvg,  nH, nW);

    printf("palette :%d\n",k/512);
    char cNomImgPalette[250]="out/palette.ppm";
    char cNomImgColor[250]="out/color.ppm";
    ecrire_image_ppm(cNomImgPalette, ImgPalette,  nH, nW);
    ecrire_image_ppm(cNomImgColor, ImgOut,  nH, nW);
    printf("PSNR:%f\n", psnr(ImgIn,ImgOut,nH,nW));
    free(ImgIn);
    return 1;

}
