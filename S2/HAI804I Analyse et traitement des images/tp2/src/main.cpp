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

    lire_nb_lignes_colonnes_image_ppm(ptr, &nH, &nW);
    nTaille = nH * nW;

    int nTaille3 = nTaille * 3;
    allocation_tableau(ImgIn, OCTET, nTaille3);
    lire_image_ppm(ptr, ImgIn, nH * nW);
    allocation_tableau(ImgOut, OCTET, nTaille3);
    allocation_tableau(ImgTrans, OCTET, nTaille3);

    //char * pathRGB = makePath((char*)"rgb.ppm",folderOut);
    
    //ecrire_image_ppm(pathRGB, ImgOut,  nH, nW);

    std::vector<std::vector<couleur>> image;
    image.resize(nH);
    for(auto & line: image)
        line.resize(nW);
    int tnW = 3* nW;
        for (int i=0; i < nH; i++)
        {
            for (int j=0, p=0; j < nW , p<tnW; j++,p+=3)
            {
                image[i][j].r=ImgIn[i *tnW +p  ];
                image[i][j].g=ImgIn[i *tnW +p+1];
                image[i][j].b=ImgIn[i *tnW +p+2];

            }
        }

        for (int i=0; i < nH; i++)
        {
            for (int j=0, p=0; j < nW , p<tnW; j++,p+=3)
            {
                ImgTrans[i *tnW +p  ]=image[i][j].r;
                ImgTrans[i *tnW +p+1]=image[i][j].g;
                ImgTrans[i *tnW +p+2]=image[i][j].b;
            }
        }
         ecrire_image_ppm(pathOut, ImgTrans,  nH, nW);
         printf("PSNR : %f\n",psnr(ImgIn,ImgTrans, nH,  nW));

    


    free(ImgIn);
    return 1;
}