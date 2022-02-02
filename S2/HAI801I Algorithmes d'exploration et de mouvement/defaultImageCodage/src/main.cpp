#include <iostream>
#include <stdio.h>
#include "image_ppm.h"
#include <stdlib.h>
#include <vector>

typedef  std::vector<std::vector<int>> NDG;

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
void printImage(NDG image, int nW,int nH)
{
   char * symb="$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";
    char *gscale2 = "@%#*+=-:. ";
 

        int step =2;

        for (int i=0; i < nH/step; i+=step)
        {
            for (int j=0; j < nW/step ; j+=step)
            {
                std::cout<<gscale2[image[i][j]%10]<<" ";

            }
            std::cout<<"\n";
        }

}


int main(int argc, char* argv[]) {
    char inputName[250];
    int nH, nW;
    char * folderIn = (char*)"res/";
    char * folderOut = (char*)"out/"; 

    if (argc != 2) 
    {
        printf("Usage: ImageIn.pgm   \n"); 
        exit (1) ;
    }

    sscanf (argv[1],"%s",inputName) ;

    char * pathIn = makePath(inputName,folderIn);

    std::vector<std::vector<int>> imageG;

        
    lire_nb_lignes_colonnes_image_pgm(pathIn, &nH, &nW);
    imageG.resize(nH);
    for(auto & line: imageG)
        line.resize(nW);
    
    loadImage(pathIn,imageG,nW, nH);
    printImage( imageG, nW,nH);

    



    return 1;
}