#include <iostream>
#include <stdio.h>
#include "image_ppm.h"
#include <stdlib.h>
#include <vector>
#define PALETTE 256
struct couleur
{
  int r,g,b;
};
typedef couleur couleur;
char * makePath(char* path, char * folder)
{
    char * pathIn = (char*)malloc(sizeof(char) * (strlen(path)+strlen(folder)+2));

    strcpy( pathIn, folder );
    strcat(pathIn,path);
    return pathIn;
}
float psnr(OCTET* ImgIn,OCTET* ImgOut, int nH, int nW)
{
  int d =PALETTE-1;
  float diff =0.;
  for(int i =0; i < nH*nW ; i++)
  {
    diff += sqrt( (ImgIn[i]-ImgOut[i])*(ImgIn[i]-ImgOut[i]) + (ImgIn[i+1]-ImgOut[i+1])* (ImgIn[i+1]-ImgOut[i+1]) +(ImgIn[i+2]-ImgOut[i+2])*(ImgIn[i+2]-ImgOut[i+2]));
  }

  float EQM = 1./(nH*nW) * diff;
  return 10*log10(3*d*d / EQM);
}
void seuillage(std::vector<std::vector<int>> &image,int seuil)
{
    for(auto & line : image)
        for(auto& pixel :line)
        {
            if(pixel< seuil)
            {
                pixel =0;
            }
            else pixel =255;
        }

}
void processImage(std::vector<std::vector<int>> &image, int nH, int nW)
{
    std::vector<std::vector<int>> erosionMatrix {
                { 0, 1, 0 },
                { 1, 1, 1 },
                { 0, 1, 0 }
            };
            /*
                std::vector<std::vector<int>> erosionMatrix {
                { 1, 1, 1, 1,  1,1, 1 },
                { 1, 1, 1, 1,  1,1, 1 },
                { 1, 1, 1, 1,  1,1, 1 },

                { 1, 1, 1, 1,  1,1, 1 },

                { 1, 1, 1,  1,  1,1, 1 },
                { 1, 1, 1,  1,  1,1, 1 },
                { 1, 1, 1,  1,  1,1, 1 }
            };*/
    int boundary = erosionMatrix.size()/2;
    
    for(int i = boundary ; i < image.size()-(boundary+1); i++)
    {
        int s = image[i].size();
        for(int j = boundary ; j < s-(boundary+1); j++)
        {
            if(image[i][j]!=0)
            {
                for(int u = -boundary ; u < (boundary+1); u++)
                {
                    for(int v = -boundary ; v < (boundary+1); v++)
                    {
                        int p = i+u;
                        int k = j+v;

                        if(image[p][k]<erosionMatrix[boundary+u][boundary+v])
                        {
                            image[i][j]=128;
                        }

                    }
                }

            }

        }
    }
        for(int i = 1 ; i < image.size()-1; i++)
    {
        int s = image[i].size();
        for(int j = 1 ; j < s-1; j++)
        {
            if(image[i][j]==128)
            {
                image[i][j]=0;
            }
        }
    }



}
int main(int argc, char* argv[]) {
    char inputName[250], outputName[250];
    int nH, nW, nTaille;
    char actualpath [20+1];
    char *ptr;
        int composante =3;
    if (argc != 3) 
    {
        printf("Usage: ImageIn.ppm composante  \n"); 
        exit (1) ;
    }

    sscanf (argv[1],"%s",inputName) ;
    sscanf (argv[2],"%d",&composante);

    char * folderIn = (char*)"res/";
    char * folderOut = (char*)"out/"; 

    char * pathIn = makePath(inputName,folderIn);
    char * pathOut = makePath(inputName,folderOut);

    ptr = realpath(pathIn, actualpath);
    printf("input : %s \n", pathIn);
    printf("output : %s \n", pathOut);

    OCTET *ImgIn, *ImgTrans, *ImgOut;

    lire_nb_lignes_colonnes_image_pgm(ptr, &nH, &nW);
    nTaille = nH * nW;

    int nTaille3 = nTaille * 3;
    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(ptr, ImgIn, nH * nW);
    allocation_tableau(ImgOut, OCTET, nTaille);


    std::vector<std::vector<int>> image;
    image.resize(nH);
    for(auto & line: image)
        line.resize(nW);

        for (int i=0; i < nH; i++)
        {
            for (int j=0; j < nW ; j++)
            {
                image[i][j]=ImgIn[i *nW +j  ];

            }
        }
        int seuil=140;
        seuillage(image,seuil);

        processImage(image,nH,nW);
        
        /*processImage(image,nH,nW);
         processImage(image,nH,nW);
         processImage(image,nH,nW);
         processImage(image,nH,nW);
         processImage(image,nH,nW);*/
        for (int i=0; i < nH; i++)
        {
            for (int j=0; j < nW; j++)
            {
                ImgOut[i *nW +j]=image[i][j];

            }
        }
         ecrire_image_pgm(pathOut, ImgOut,  nH, nW);    


    free(ImgIn);
    return 1;
}