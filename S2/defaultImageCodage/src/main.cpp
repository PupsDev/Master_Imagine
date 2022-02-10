#include <iostream>
#include <stdio.h>
#include "../src/functions.cpp"



int main(int argc, char* argv[]) {
    char inputName[250];
    int nH, nW;
   char * folderIn = (char*)"../../res/";
    char * folderOut = (char*)"out/"; 

    if (argc != 2) 
    {
        printf("Usage: ImageIn.ppm   \n"); 
        exit (1) ;
    }

    sscanf (argv[1],"%s",inputName) ;

    char * pathIn = makePath(inputName,folderIn);
    int color = ppmOrPgm(inputName);
    ImageRGB imageRGB;
    ImageG imageG;

    if(color)
    {
        
        lire_nb_lignes_colonnes_image_ppm(pathIn, &nH, &nW);
        resize(imageRGB,nW,nH);
        
        loadImage(pathIn,imageRGB,nW, nH);

        saveImage( folderOut, (char*)"originale_",inputName,imageRGB);
    }
    else
    {
        
        lire_nb_lignes_colonnes_image_pgm(pathIn, &nH, &nW);
        resize(imageG,nW,nH);
        
        loadImage(pathIn,imageG,nW, nH);
        saveImage( folderOut, (char*)"originale_",inputName,imageG);


    }
    



    return 1;
}