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
// out/specific_monfichier.ppm

char *makeFinalPath( char *folderOut, char *specificName,char* inputName)
{
    return (char*)makePath(inputName,makePath(specificName,folderOut));
}
void saveImage(char * pathOut, int nH,int nW,std::vector<std::vector<int>> image )
{
    OCTET *ImgOut;
    allocation_tableau(ImgOut, OCTET, nH*nW);
    for (int i=0; i < nH; i++)
    {
        for (int j=0; j < nW; j++)
        {
            ImgOut[i *nW +j]=image[i][j];

        }
    }
    ecrire_image_pgm(pathOut, ImgOut,  nH, nW);
}
void saveImage(char * pathOut, int nH,int nW,std::vector<std::vector<couleur>> image )
{
    OCTET *ImgOut;
    allocation_tableau(ImgOut, OCTET, 3*nH*nW);
    int tnW = 3*nW;
        for (int i=0; i < nH; i++)
        {
            for (int j=0, p=0; j < nW , p<tnW; j++,p+=3)
            {


                ImgOut[i *tnW +p  ]=image[i][j].r;
                ImgOut[i *tnW +p+1]=image[i][j].g;
                ImgOut[i *tnW +p+2]=image[i][j].b;
            }
        }
    ecrire_image_ppm(pathOut, ImgOut,  nH, nW);
}
void loadImage(char* pathIn, std::vector<std::vector<couleur>> &image,int nW, int nH)
{
    OCTET *imgColor;
    int nTaille = nW*nH;
    int nTaille3 = nTaille*3;

    allocation_tableau(imgColor, OCTET, nTaille3);
    lire_image_ppm(pathIn, imgColor, nTaille);
    
    int tnW=3*nW;

    for (int i=0; i < nH; i++)
        {
            for (int j=0, p=0; j < nW , p<tnW; j++,p+=3)
            {
                image[i][j].r=imgColor[i *tnW +p  ];
                image[i][j].g=imgColor[i *tnW +p+1];
                image[i][j].b=imgColor[i *tnW +p+2];
            }
        }
    free(imgColor);

}
void loadImage(char* pathIn, std::vector<std::vector<int>> &image,int nW, int nH)
{
    OCTET *imgColor;
    int nTaille = nW*nH;

    allocation_tableau(imgColor, OCTET, nTaille);
    lire_image_pgm(pathIn, imgColor, nTaille);
    
    int tnW=3*nW;

    for (int i=0; i < nH; i++)
        {
            for (int j=0; j < nW ; j++)
            {
                image[i][j]=imgColor[i *nW +j  ];

            }
        }
    free(imgColor);

}
void histo(std::vector<std::vector<int>> image,int nW, int nH)
{
    unsigned int occurence [256]={0};
    float sum =0.;
    for (int i=0; i < nH; i++)
    {
        for (int j=0; j < nW ; j++)
        {
            occurence[image[i][j]]++;

        }
    }
    
    for (int i=0; i < 256; i++)
     	printf("%d %f\n",i, (float)occurence[i]/(float)(nW*nH)*100. );
    
}
void difference(std::vector<std::vector<int>> &image,int nW, int nH)
{

    std::vector<std::vector<int>> image2;
    image2.resize(nH);
        for(auto & line: image2)
            line.resize(nW);
    unsigned int average =0;

    /*for (int i=1; i < nH-1; i++)
    {
        for (int j=1; j < nW-1 ; j++)
        {
            int average;
            for (int u=-1; u < 2; u++)
                for (int v=-1; v < 2 ; v++)
                    average+=image[i+u][j+v];
            average/=9;

        }
    }*/
    for (int i=0; i < nH; i++)
    {
        for (int j=0; j < nW ; j++)
        {
           average+=image[i][j];

        }
    }
    average/=(nW*nH);
    for (int i=0; i < nH; i++)
    {
        for (int j=0; j < nW ; j++)
        {
           image2[i][j]=(image[i][j]-average)+128;

        }
    }
    for (int i=0; i < nH; i++)
    {
        for (int j=0; j < nW ; j++)
        {
           image[i][j]=image2[i][j];

        }
    }
    //printf("avergae : %d\n",average);
}
int main(int argc, char* argv[]) {
    char inputName[250];
    int nH, nW;
    char * folderIn = (char*)"res/";
    char * folderOut = (char*)"out/"; 

    if (argc != 2) 
    {
        printf("Usage: ImageIn.ppm   \n"); 
        exit (1) ;
    }

    sscanf (argv[1],"%s",inputName) ;

    char * pathIn = makePath(inputName,folderIn);
    int color = false;
    std::vector<std::vector<couleur>> imageRGB;
    std::vector<std::vector<int>> imageG;

    if(color)
    {
        
        lire_nb_lignes_colonnes_image_ppm(pathIn, &nH, &nW);
        imageRGB.resize(nH);
        for(auto & line: imageRGB)
            line.resize(nW);
        
        loadImage(pathIn,imageRGB,nW, nH);

        saveImage(makeFinalPath( folderOut, (char*)"_originale_",inputName), nH,nW,imageRGB);
    }
    else
    {
        
        lire_nb_lignes_colonnes_image_pgm(pathIn, &nH, &nW);
        imageG.resize(nH);
        for(auto & line: imageG)
            line.resize(nW);
        
        loadImage(pathIn,imageG,nW, nH);
        saveImage(makeFinalPath( folderOut, (char*)"_originale_",inputName), nH,nW,imageG);
        //histo(imageG,nW,  nH);
        difference(imageG,nW,nH);
        histo(imageG,nW,  nH);
        saveImage(makeFinalPath( folderOut, (char*)"_diff_",inputName), nH,nW,imageG);
    }
    



    return 1;
}