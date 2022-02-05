#include <iostream>
#include <stdio.h>
#include "image_ppm.h"
#include <stdlib.h>
#include <vector>
#include <unistd.h>
#include <sys/stat.h>
using namespace std;

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
void histoDensiteRepartition(std::vector<std::vector<int>> image,int nW, int nH)
{
    unsigned int occurence [256]={0};
    float sum[256] = {0.};
    for (int i=0; i < nH; i++)
    {
        for (int j=0; j < nW ; j++)
        {
            occurence[image[i][j]]++;

        }
    }
     FILE * file;
    file = fopen( "data/densite.dat", "wb+" );
    for (int i=0; i < 256; i++)
     	fprintf(file,"%d %f\n",i, (float)occurence[i]/(float)(nW*nH) );
     fclose( file );

    file = fopen( "data/histogramme.dat", "wb+" );
    for (int i=0; i < 256; i++)
     	fprintf(file,"%d %d\n",i, occurence[i]);
     fclose( file );


    file = fopen( "data/repartition.dat", "wb+" );
    sum[0]=occurence[0]/(float)(nW*nH);
    for (int i=1; i < 256; i++)
    {
        sum[i]=sum[i-1]+(float)occurence[i]/(float)(nW*nH);
        fprintf(file,"%d %f\n",i, sum[i]);
        
    }
}
void histoDensiteRepartitionRGB(std::vector<std::vector<couleur>> image,int nW, int nH)
{
    unsigned int occurenceR [256]={0};
    unsigned int occurenceG [256]={0};
    unsigned int occurenceB [256]={0};
    float sumR[256] = {0.};
    float sumG[256] = {0.};
    float sumB[256] = {0.};

    FILE * file;
    for (int i=0; i < nH; i++)
    {
        for (int j=0; j < nW ; j++)
        {
            occurenceR[image[i][j].r]++;
            occurenceG[image[i][j].g]++;
            occurenceB[image[i][j].b]++;


        }
    }
    file = fopen( "data/histogrammeRGB.dat", "wb+" );
    for (int i=0; i < 256; i++)
     	fprintf(file,"%d %d %d %d\n",i, occurenceR[i], occurenceG[i], occurenceB[i]);
    fclose( file );
    file = fopen( "data/densiteRGB.dat", "wb+" );
    for (int i=0; i < 256; i++)
     	fprintf(file,"%d %f %f %f\n",i, (float)occurenceR[i]/(float)(nW*nH), (float)occurenceG[i]/(float)(nW*nH), (float)occurenceB[i]/(float)(nW*nH) );
    fclose( file );
    sumR[0]=(float)occurenceR[0]/ (float) (nW*nH);
    sumG[0]=(float)occurenceG[0]/ (float) (nW*nH);
    sumB[0]=(float)occurenceB[0]/ (float) (nW*nH);
        file = fopen( "data/repartitionRGB.dat", "wb+" );
    for (int i=1; i < 256; i++)
    {
        sumR[i]=sumR[i-1]+(float)occurenceR[i]/(float)(nW*nH);
        sumG[i]=sumG[i-1]+(float)occurenceG[i]/(float)(nW*nH);
        sumB[i]=sumB[i-1]+(float)occurenceB[i]/(float)(nW*nH);

        fprintf(file,"%d %f %f %f\n",i, sumR[i], sumG[i], sumB[i]);
    }
    fclose( file );

    

}
bool IsPathExist(const std::string &s)
{
  struct stat buffer;
  return (stat (s.c_str(), &buffer) == 0);
}
int main(int argc, char* argv[]) {
    char inputName[250];
    int nH, nW;
    char * folderIn = (char*)"res/";
    char * folderOut = (char*)"out/"; 
     int color ;

    if (argc != 2) 
    {
        printf("Usage: ImageIn.pgm/ppm \n"); 
        exit (1) ;
    }

    sscanf (argv[1],"%s",inputName) ;

    string s = inputName;
    std::string token = s.substr(0, s.find("."));
    s.erase(0, s.find(".") + string(".").length());
     token = s.substr(0, s.find("."));

    color = (token == "pgm")?0:1;

    char * pathIn = makePath(inputName,folderIn);
   
    std::vector<std::vector<couleur>> imageRGB;
    std::vector<std::vector<int>> imageG;
    if(!IsPathExist(string("data")))
        system("mkdir data");
    
    if(color)
    {
        
        lire_nb_lignes_colonnes_image_ppm(pathIn, &nH, &nW);
        imageRGB.resize(nH);
        for(auto & line: imageRGB)
            line.resize(nW);
        
        loadImage(pathIn,imageRGB,nW, nH);
        histoDensiteRepartitionRGB(imageRGB,nW,nH);
    }
    else
    {
        
        lire_nb_lignes_colonnes_image_pgm(pathIn, &nH, &nW);
        imageG.resize(nH);
        for(auto & line: imageG)
            line.resize(nW);
        
        loadImage(pathIn,imageG,nW, nH);

        histoDensiteRepartition(imageG,nW,nH);


    }
    



    return 1;
}