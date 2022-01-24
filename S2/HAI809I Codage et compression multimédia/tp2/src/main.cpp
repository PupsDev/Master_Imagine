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
    if (argc != 3) 
    {
        printf("Usage: ImageIn.ppm ImageOut.ppm  \n"); 
        exit (1) ;
    }

    sscanf (argv[1],"%s",inputName) ;
    sscanf (argv[2],"%s",outputName);

    char * folderIn = (char*)"res/";
    char * folderOut = (char*)"out/"; 

    char * pathIn = makePath(inputName,folderIn);
    char * pathOut = makePath(inputName,folderOut);

    ptr = realpath(pathIn, actualpath);
    printf("input : %s \n", pathIn);
    printf("output : %s \n", pathOut);
    //printf("%s \n", ptr);

    OCTET *ImgIn, *ImgTrans, *ImgOut;

    lire_nb_lignes_colonnes_image_ppm(ptr, &nH, &nW);
    nTaille = nH * nW;

    int nTaille3 = nTaille * 3;
    allocation_tableau(ImgIn, OCTET, nTaille3);
    lire_image_ppm(ptr, ImgIn, nH * nW);
    allocation_tableau(ImgOut, OCTET, nTaille3);
    allocation_tableau(ImgTrans, OCTET, nTaille3);

    // SUBSAMBLING RGB
    /*
    for (int i=0; i < nTaille3; i+=3)
    {
        ImgOut[i  ]=ImgIn[i ];

    }
    for (int i=0; i < nTaille3; i+=6)
    {
        ImgTrans[i+1]=ImgIn[i+1]; 
        ImgTrans[i+2]=ImgIn[i+2];
    }
    
    for (int i=0; i < nTaille3; i+=6)
    {
        ImgOut[i+1]=ImgTrans[i+1]; 
        ImgOut[i+2]=ImgTrans[i+2];
        int k = i+=3;
        ImgOut[k+1]=ImgTrans[i+1]; 
        ImgOut[k+2]=ImgTrans[i+2];
    }
    */
    // SUBSAMBLING CHROMILUMI
    /*
    OCTET *luminance, *chrom;
    allocation_tableau(luminance, OCTET, nTaille3);
    allocation_tableau(chrom, OCTET, nTaille3);

    for (int i=0; i < nTaille3; i+=3)
    {
        luminance[i  ]=0.299*ImgIn[i ]+0.587*ImgIn[i+1]+0.114*ImgIn[i+2];

    }
    for (int i=0; i < nTaille3; i+=6)
    {
        //CB
        chrom[i+2]= (ImgIn[i+2]-ImgOut[i])/1.772; 
        //CR
        chrom[i]= (ImgIn[i]-ImgOut[i])/1.402;
        int k = i+=3;
        chrom[k]=chrom[i]; 
        chrom[k+2]=chrom[i+2];

    }
    for (int i=0; i < nTaille3; i+=3)
    {
        ImgOut[i+0]=  chrom[i]*1.772 + luminance[i ];
        ImgOut[i+2]=  chrom[i+2]*1.402 + luminance[i ];
        ImgOut[i+1] = (luminance[i ] -  ImgOut[i+0]*0.299 - ImgOut[i+2]*0.114)/0.587;

    }

*/
    // SUBSAMBLING RGB
    std::vector<std::vector<couleur>> image;
    image.resize(nH);
    for(auto& line : image)
        line.resize(nW);
    
    std::vector<std::vector<couleur>> image2;
    image2.resize(nH);
    for(auto& line : image2)
        line.resize(nW);
    
    std::vector<std::vector<couleur>> image3;
    image3.resize(nH);
    for(auto& line : image3)
        line.resize(nW);
    OCTET *ImgGB;
    allocation_tableau(ImgGB, OCTET, nTaille3);

    bool RGB = false; 
    if(RGB)
    {
            int tnW = 3*nW;
    for (int i=0; i < nH; i++)
    {
        for (int j=0, p=0; j < nW , p<tnW; j++,p+=3)
        {
            image[i][j].r=ImgIn[i *tnW +p  ];
            image[i][j].g=ImgIn[i *tnW +p+1];
            image[i][j].b=ImgIn[i *tnW +p+2];


            image2[i][j].r=ImgIn[i *tnW +p  ];

        }
    }
    // SUBSAMBLING RGB
    

    for (int i=0; i < nH; i+=2)
    {
        for (int j=0; j < nW; j+=2)
        { 

            image3[i][j].g= image[i][j].g;
            image3[i][j].b= image[i][j].b;

        }

    }
    for (int i=0; i < nH; i+=2)
    {
        for (int j=0; j < nW; j+=2)
        { 

            for(int u = 0 ; u<2;u++)
                for(int v =0 ; v<2;v++)
                {
                    image3[i+u][j+v].g= image[i][j].g;
                    image3[i+u][j+v].b= image[i][j].b;
                }

        }

    }
    // END SUBSAMBLING RGB
    for (int i=0; i < nH; i++)
    {
        for (int j=0, p=0; j < nW , p<tnW; j++,p+=3)
        {
            ImgOut[i *tnW +p  ]=image2[i][j].r;
            ImgOut[i *tnW +p+1]=image3[i][j].g;
            ImgOut[i *tnW +p+2]=image3[i][j].b;
        }
    }
    for (int i=0; i < nH; i++)
    {
        for (int j=0, p=0; j < nW , p<tnW; j++,p+=3)
        {
            ImgGB[i *tnW +p  ]=0;
            ImgGB[i *tnW +p+1]=image3[i][j].g;
            ImgGB[i *tnW +p+2]=image3[i][j].b;
        }
    }
        ecrire_image_ppm(pathOut, ImgOut,  nH, nW);
    char * pathGB = makePath((char*)"gbsub.ppm",folderOut);
    ecrire_image_ppm(pathGB, ImgGB,  nH, nW);
    printf("PSNR : %f\n",psnr(ImgIn,ImgOut, nH,  nW));


    }
    else
    {
        //image 2 luminance
        //image3 chromian


        int tnW = 3*nW;
        for (int i=0; i < nH; i++)
        {
            for (int j=0, p=0; j < nW , p<tnW; j++,p+=3)
            {
                image[i][j].r=ImgIn[i *tnW +p  ];
                image[i][j].g=ImgIn[i *tnW +p+1];
                image[i][j].b=ImgIn[i *tnW +p+2];

            }
        }
        //LUMINANCE
        for (int i=0; i < nH; i++)
        {
            for (int j=0; j < nW; j++)
            { 
                image2[i][j].r= 0.299* image[i][j].r +0.587*image[i][j].g+0.114*image[i][j].b;
                image2[i][j].g=  image2[i][j].r;
                image2[i][j].b=  image2[i][j].r;

            }

        }
        //CHROMINANCE
        for (int i=0; i < nH; i++)
        {
            for (int j=0; j < nW; j++)
            { 
                image3[i][j].r=  (image[i][j].r-image2[i][j].r)/1.402;
                image3[i][j].g= 0;
                image3[i][j].b=  (image[i][j].b-image2[i][j].r)/1.772;
                

            }

        }
        // SUBSAMPLING
        for (int i=0; i < nH; i+=2)
        {
            for (int j=0; j < nW; j+=2)
            { 

                image3[i][j].r= image3[i][j].r;
                image3[i][j].b= image3[i][j].b;

            }

        }
            for (int i=0; i < nH; i+=2)
        {
            for (int j=0; j < nW; j+=2)
            { 

                for(int u = 0 ; u<2;u++)
                    for(int v =0 ; v<2;v++)
                    {
                        image3[i+u][j+v].r= image3[i][j].r;
                        image3[i+u][j+v].b= image3[i][j].b;
                    }

            }

        }
        //END
        char * pathLumi = makePath((char*)"luminance.ppm",folderOut);
        for (int i=0; i < nH; i++)
        {
            for (int j=0, p=0; j < nW , p<tnW; j++,p+=3)
            {
                ImgOut[i *tnW +p  ]=image2[i][j].r;
                ImgOut[i *tnW +p+1]=image2[i][j].g;
                ImgOut[i *tnW +p+2]=image2[i][j].b;
            }
        }
         ecrire_image_ppm(pathLumi, ImgOut,  nH, nW);
        /*
        char * pathChromi = makePath((char*)"chrominance.ppm",folderOut);
        for (int i=0; i < nH; i++)
        {
            for (int j=0, p=0; j < nW , p<tnW; j++,p+=3)
            {
                ImgTrans[i *tnW +p  ]=image3[i][j].r;
                ImgTrans[i *tnW +p+1]=image3[i][j].g;
                ImgTrans[i *tnW +p+2]=image3[i][j].b;
            }
        }
         ecrire_image_ppm(pathChromi, ImgTrans,  nH, nW);
         */
        
        char * pathChromi = makePath((char*)"sard.ppm",folderOut);
        for (int i=0; i < nH; i++)
        {
            for (int j=0, p=0; j < nW , p<tnW; j++,p+=3)
            {
                int r = image3[i][j].r*1.402 + image2[i][j].r;
                int b = image3[i][j].b*1.772 + image2[i][j].r;
                int g =  (image2[i][j].r -r*0.299 -b*0.114)/0.587;

                ImgTrans[i *tnW +p  ]=r;
                ImgTrans[i *tnW +p+1]=g;
                ImgTrans[i *tnW +p+2]=b;
            }
        }
         ecrire_image_ppm(pathChromi, ImgTrans,  nH, nW);
         printf("PSNR : %f\n",psnr(ImgIn,ImgTrans, nH,  nW));

    }


    free(ImgIn);
    return 1;
}