#include <iostream>
#include <stdio.h>
#include "image_ppm.h"
#include <stdlib.h>
#include <vector>
#include <cassert>

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
  int d =2-1;
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
float distance(int r, int r1)
{
    return sqrt( (r-r1)*(r-r1));
}
int nearestClust(int *couleurArr, int c)
{
  int min=-1;
  float dmin = 10000000.;
  for(int i =0 ; i<2;i++)
  {
    if(distance(couleurArr[i],c)<dmin)
    {
      min = i;
      dmin = distance(couleurArr[i],c);
    }
  }
  return min;
}
void seuilAuto(std::vector<std::vector<int>> &image,int nW,int nH)
{
    int classNumber=2;

    int* classColors= (int*)malloc(classNumber * sizeof(int));

    int* classe= (int*)malloc(nH*nW * sizeof(int));


    int* moyenne= (int*)malloc((classNumber+1) * sizeof(int));
    int* somme= (int*)malloc( (classNumber+1) * sizeof(int));

    for(int it =0 ;it <1 ;it++)
    {
        for(int i=0 ; i <classNumber; i++)
        {
            int p = rand()%nH;
            int q = rand()%nW;

            classColors[i] = image[p][q];
            moyenne[i]=0;
            somme[i]=0;
        }
        for (int i=0; i < nH; i++)
        {
            for (int j=0; j < nW ; j++)
            {
                int c = image[i][j];
                int k = nearestClust(classColors, c); // pres de ck
                
                image[i][j]=classColors[k];
                moyenne[k]+=c;
                somme[k]++;
               
            }
        }
        for(int j=0 ; j <classNumber; j++)
      {
        if(somme[j]!=0)
        {
          moyenne[j]/=somme[j];
        }

      }
      
      for(int j=0 ; j <classNumber; j++)
      {
        classe[j]=classColors[j];
        classColors[j]=moyenne[j];
        moyenne[j]=0;
        somme[j]=0;
      }
    }
    
    int b =0;
    b = (classe[0]<classe[1])? 0 :255;

    for (int i=0; i < nH; i++)
    {
        for (int j=0; j < nW ; j++)
        {

            if(image[i][j]==classe[0])
                image[i][j]=b;
            else if(image[i][j]==classe[1])
                image[i][j]=255-b;
            
        }
    }
      
}
void erosion(std::vector<std::vector<int>> &image, int nH, int nW, int c)
{
    std::vector<std::vector<int>> erosionMatrix {
                { 0, 1, 0 },
                { 1, 1, 1 },
                { 0, 1, 0 }
            };


    int boundary = erosionMatrix.size()/2;
    std::vector<std::vector<int>> image2;
    image2.resize(nH);
    for(auto & line: image2)
        line.resize(nW);
    for (int i=0; i < nH; i++)
    {
        for (int j=0; j < nW ; j++)
        {
          
           image2[i][j] =image[i][j];

        }
    }

    for(int i = boundary ; i < image.size()-(boundary+1); i++)
    {
        int s = image[i].size();
        for(int j = boundary ; j < s-(boundary+1); j++)
        {

            // compute erosion
            for(int u = -boundary ; u < (boundary+1); u++)
            {
                for(int v = -boundary ; v < (boundary+1); v++)
                {
                    int p = i+u;
                    int k = j+v;
                    if( (p>=0 && p<nH) && (k>=0 && k<nW))
                    {
                        if( erosionMatrix[boundary+u][boundary+v] && image[p][k]==c )
                        {
                            image2[i][j]=c;
                        }
                    }

                }
            }
        }
    }

    for (int i=0; i < nH; i++)
    {
        for (int j=0; j < nW ; j++)
        {
          
           image[i][j] =image2[i][j];

        }
    }

}
void invert(std::vector<std::vector<int>> &image, int nH, int nW)
{
    for(int i = 0 ; i < image.size();i++)
    {
         int s = image[i].size();
        for(int j = 0 ; j < s;j++)
        {
            image[i][j]=255-image[i][j];
        }
    }

}

void dilatation(std::vector<std::vector<int>> &image, int nH, int nW, int c)
{

    invert(image,nH,nW);
    erosion(image,nH,nW,c);
    invert(image,nH,nW);

}
void ouverture(std::vector<std::vector<int>> &image, int nH, int nW)
{
    dilatation(image,nH,nW,0);
    erosion(image,nH,nW,0);

}
void fermeture(std::vector<std::vector<int>> &image, int nH, int nW)
{
    erosion(image,nH,nW,0);
    dilatation(image,nH,nW,0);
    
}
void ouverture363(std::vector<std::vector<int>> &image, int nH, int nW)
{
    dilatation(image,nH,nW,0);
    dilatation(image,nH,nW,0);
    dilatation(image,nH,nW,0);

    erosion(image,nH,nW,0);
    erosion(image,nH,nW,0);
    erosion(image,nH,nW,0);

    erosion(image,nH,nW,0);
    erosion(image,nH,nW,0);
    erosion(image,nH,nW,0);

    dilatation(image,nH,nW,0);
    dilatation(image,nH,nW,0);
    dilatation(image,nH,nW,0);
}
void difference(std::vector<std::vector<int>> &image,std::vector<std::vector<int>> &imageDilate, int nH, int nW)
{

    for (int i=0; i < nH; i++)
    {
        for (int j=0; j < nW ; j++)
        {
          
           image[i][j] =255-(imageDilate[i][j]-image[i][j]);

        }
    }
}
void difference(std::vector<std::vector<couleur>> &image,std::vector<std::vector<couleur>> &imageDilate, int nH, int nW)
{

    for (int i=0; i < nH; i++)
    {
        for (int j=0; j < nW ; j++)
        {
          
           image[i][j].r =255-(imageDilate[i][j].r-image[i][j].r);
           image[i][j].g =255-(imageDilate[i][j].g-image[i][j].g);
           image[i][j].b =255-(imageDilate[i][j].b-image[i][j].b);

        }
    }
}
void erosionGreyscale(std::vector<std::vector<int>> &image, int nH, int nW)
{
    std::vector<std::vector<int>> erosionMatrix {
                { 0, 1, 0 },
                { 1, 1, 1 },
                { 0, 1, 0 }
            };

     /*std::vector<std::vector<int>> erosionMatrix {
      {0, 0, 0, 1, 0, 0, 0}, 
      {0, 0, 0, 1, 0, 0, 0}, 
      {0, 0, 0, 1, 0, 0, 0}, 
      {1, 1, 1, 1, 1, 1, 1}, 
      {0, 0, 0, 1, 0, 0, 0}, 
      {0, 0, 0, 1, 0, 0, 0}, 
      {0, 0, 0, 1, 0, 0, 0}
    };*/
    /*
    std::vector<std::vector<int>> erosionMatrix {
      {0, 0, 0, 0, 1, 0, 0, 0, 0}, 
      {0, 0, 1, 1, 1, 1, 1, 0, 0}, 
      {0, 1, 1, 1, 1, 1, 1, 1, 0}, 
      {0, 1, 1, 1, 1, 1, 1, 1, 0}, 
      {1, 1, 1, 1, 1, 1, 1, 1, 1}, 
      {0, 1, 1, 1, 1, 1, 1, 1, 0}, 
      {0, 1, 1, 1, 1, 1, 1, 1, 0}, 
      {0, 0, 1, 1, 1, 1, 1, 0, 0},
      {0, 0, 0, 0, 1, 0, 0, 0, 0}
    };*/

    int boundary = erosionMatrix.size()/2;
    std::vector<std::vector<int>> image2;
    image2.resize(nH);
    for(auto & line: image2)
        line.resize(nW);
    for (int i=0; i < nH; i++)
    {
        for (int j=0; j < nW ; j++)
        {
          
           image2[i][j] =image[i][j];

        }
    }

    for(int i = boundary ; i < image.size()-(boundary+1); i++)
    {
        int s = image[i].size();
        for(int j = boundary ; j < s-(boundary+1); j++)
        {

            //compute min
            int minv = 256;
            for(int u = -boundary ; u < (boundary+1); u++)
            {
                for(int v = -boundary ; v < (boundary+1); v++)
                {
                    int p = i+u;
                    int k = j+v;
                    if( (p>=0 && p<nH) && (k>=0 && k<nW))
                    {
                        if(image[p][k] < minv)minv = image[p][k];
                    }
                }
            }

            // compute erosion
            for(int u = -boundary ; u < (boundary+1); u++)
            {
                for(int v = -boundary ; v < (boundary+1); v++)
                {
                    int p = i+u;
                    int k = j+v;
                    if( (p>=0 && p<nH) && (k>=0 && k<nW))
                    {
                        if( erosionMatrix[boundary+u][boundary+v] && image[p][k]!=255 )
                        {
                            image2[i][j]=minv;
                        }
                    }

                }
            }
        }
    }

    for (int i=0; i < nH; i++)
    {
        for (int j=0; j < nW ; j++)
        {
          
           image[i][j] =image2[i][j];

        }
    }

}
float dotCouleur(couleur c)
{
    return sqrt(c.r*c.r+c.g*c.g+c.b*c.b);
}
void erosionColor(std::vector<std::vector<couleur>> &image, int nH, int nW)
{
    std::vector<std::vector<int>> erosionMatrix {
                { 0, 1, 0 },
                { 1, 1, 1 },
                { 0, 1, 0 }
            };


    int boundary = erosionMatrix.size()/2;
    std::vector<std::vector<couleur>> image2;
    image2.resize(nH);
    for(auto & line: image2)
        line.resize(nW);
    for (int i=0; i < nH; i++)
    {
        for (int j=0; j < nW ; j++)
        {
          
           image2[i][j] =image[i][j];

        }
    }

    for(int i = boundary ; i < image.size()-(boundary+1); i++)
    {
        int s = image[i].size();
        for(int j = boundary ; j < s-(boundary+1); j++)
        {

            //compute min
            int minv = sqrt(256*256+256*256+256*256);
            int indi,indj;

            for(int u = -boundary ; u < (boundary+1); u++)
            {
                for(int v = -boundary ; v < (boundary+1); v++)
                {
                    int p = i+u;
                    int k = j+v;
                    if( (p>=0 && p<nH) && (k>=0 && k<nW))
                    {
                        if( dotCouleur(image[p][k]) < minv)
                        {
                            minv = dotCouleur(image[p][k]);
                            indi=p;
                            indj=k;
                        }
                    }
                }
            }

            // compute erosion
            for(int u = -boundary ; u < (boundary+1); u++)
            {
                for(int v = -boundary ; v < (boundary+1); v++)
                {
                    int p = i+u;
                    int k = j+v;
                    if( (p>=0 && p<nH) && (k>=0 && k<nW))
                    {
                        if( erosionMatrix[boundary+u][boundary+v] && dotCouleur(image[p][k])>minv )
                        {
                            image2[i][j]=image[indi][indj];
                        }
                    }

                }
            }
        }
    }

    for (int i=0; i < nH; i++)
    {
        for (int j=0; j < nW ; j++)
        {
          
           image[i][j] =image2[i][j];

        }
    }

}
void dilatationGreyscale(std::vector<std::vector<int>> &image, int nH, int nW)
{

    invert(image,nH,nW);
    erosionGreyscale(image,nH,nW);
    invert(image,nH,nW);

}
void invert(std::vector<std::vector<couleur>> &image, int nH, int nW)
{
    for(int i = 0 ; i < image.size();i++)
    {
         int s = image[i].size();
        for(int j = 0 ; j < s;j++)
        {
            image[i][j].r=255-image[i][j].r;
            image[i][j].g=255-image[i][j].g;
            image[i][j].b=255-image[i][j].b;
        }
    }

}
void dilatationColor(std::vector<std::vector<couleur>> &image, int nH, int nW)
{

    invert(image,nH,nW);
    erosionColor(image,nH,nW);
    invert(image,nH,nW);

}
int main(int argc, char* argv[]) {
    char inputName[250],inputName2[250],  outputName[250];
    int nH, nW, nTaille;
    char actualpath [20+1];
    char *ptr;
        int composante =3;
    if (argc != 4) 
    {
        printf("Usage: ImageIn.ppm composante img.ppm  \n"); 
        exit (1) ;
    }

    sscanf (argv[1],"%s",inputName) ;
    sscanf (argv[2],"%d",&composante);
     sscanf (argv[3],"%s",inputName2) ;

    char * folderIn = (char*)"";
    char * folderOut = (char*)"out/"; 

    char * pathIn = makePath(inputName,folderIn);
    char * pathIn2 = makePath(inputName2,folderIn);
    char * pathOut = makePath(inputName,folderOut);

    ptr = realpath(pathIn, actualpath);
    printf("input : %s \n", pathIn);
    printf("output : %s \n", pathOut);

    OCTET *ImgIn, *ImgTrans, *ImgOut;
/*
    lire_nb_lignes_colonnes_image_pgm(pathIn, &nH, &nW);
    nTaille = nH * nW;

    int nTaille3 = nTaille * 3;
    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(pathIn, ImgIn, nH * nW);
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


    std::vector<std::vector<int>> imageDilate;
    imageDilate.resize(nH);
    for(auto & line: imageDilate)
        line.resize(nW);



        //seuilAuto(image,nH,nW);
        seuillage(image,140);
        for (int i=0; i < nH; i++)
        {
            for (int j=0; j < nW ; j++)
            {
                imageDilate[i][j]=image[i][j];

            }
        }
        //seuilAuto(imageDilate,nH,nW);

        char * pathOriginal = makePath((char*)"original_",folderOut);
        char * pathSeuillage = makePath((char*)"seuil_",folderOut);

        char * pathErosion = makePath((char*)"erosion_",folderOut);
        char * pathDilatation = makePath((char*)"dilatation_",folderOut);


        char * pathFermeture = makePath((char*)"fermeture_",folderOut);
        char * pathOuverture = makePath((char*)"ouverture_",folderOut);

        char * pathD = makePath((char*)"difference_",folderOut);

        //char *pathFile = makePath(inputName,(char*)".ppm");

        char * path = makePath(inputName,pathSeuillage);
        // SEUIL
        saveImage(path, nH,nW,image );

        erosion(image,nH,nW,0);
        path = makePath(inputName,pathErosion);
        //EROSION
        saveImage(path, nH,nW,image );

        dilatation(imageDilate,nH,nW,0);
        path = makePath(inputName,pathDilatation);
        // DILATATION
        saveImage(path, nH,nW,imageDilate );

        // DIFFERENCE
        difference(image,imageDilate, nH,nW);
        path = makePath(inputName,pathD);
        saveImage(path, nH,nW,image );


        for (int i=0; i < nH; i++)
        {
            for (int j=0; j < nW ; j++)
            {
                image[i][j]=ImgIn[i *nW +j  ];

            }
        }
        seuillage(image,140);

        // OUVERTURE
        ouverture(image,nH,nW);
        path = makePath(inputName,pathOuverture);
        saveImage(path, nH,nW,image );

        for (int i=0; i < nH; i++)
        {
            for (int j=0; j < nW ; j++)
            {
                imageDilate[i][j]=ImgIn[i *nW +j  ];

            }
        }
        seuillage(imageDilate,140);

        // FERMETURE
        fermeture(imageDilate,nH,nW);
        path = makePath(inputName,pathFermeture);
        saveImage(path, nH,nW,imageDilate );

        


        
        // PART 2 363 et 66
        
        for (int i=0; i < nH; i++)
        {
            for (int j=0; j < nW ; j++)
            {
                image[i][j]=ImgIn[i *nW +j  ];

            }
        }
        seuillage(image,140);

        for (int i=0; i < nH; i++)
        {
            for (int j=0; j < nW ; j++)
            {
                imageDilate[i][j]=ImgIn[i *nW +j  ];

            }
        }
        seuillage(imageDilate,140);

        fermeture(image,nH,nW);
        ouverture(image,nH,nW);
        saveImage((char*)makePath(inputName,makePath((char*)"fermeture_ouverture_",folderOut)), nH,nW,image );
            
        ouverture363(imageDilate,nH,nW);
        saveImage((char*)makePath(inputName,makePath((char*)"3_6_3_dil_ero_dil_",folderOut)), nH,nW,imageDilate );

                for (int i=0; i < nH; i++)
        {
            for (int j=0; j < nW ; j++)
            {
                image[i][j]=ImgIn[i *nW +j  ];

            }
        }
        seuillage(image,140);

        for (int i=0; i < nH; i++)
        {
            for (int j=0; j < nW ; j++)
            {
                imageDilate[i][j]=ImgIn[i *nW +j  ];

            }
        }
        seuillage(imageDilate,140);

        for(int i =0 ; i < 6;i++)
            erosion(imageDilate,nH,nW,0);
        for(int i =0 ; i < 6;i++)
            dilatation(imageDilate,nH,nW,0);
        saveImage((char*)makePath(inputName,makePath((char*)"6_6_erosion_dilatation_",folderOut)), nH,nW,imageDilate );

        for (int i=0; i < nH; i++)
        {
            for (int j=0; j < nW ; j++)
            {
                image[i][j]=ImgIn[i *nW +j  ];

            }
        }
        erosionGreyscale(image,nH,nW);
        saveImage((char*)makePath(inputName,makePath((char*)"erosion_NDG_",folderOut)), nH,nW,image );
        
        for (int i=0; i < nH; i++)
        {
            for (int j=0; j < nW ; j++)
            {
                imageDilate[i][j]=ImgIn[i *nW +j  ];

            }
        }

        dilatationGreyscale(imageDilate,nH,nW);
        saveImage((char*)makePath(inputName,makePath((char*)"dilatation_NDG_",folderOut)), nH,nW,imageDilate );

        difference(image,imageDilate, nH,nW);
        saveImage((char*)makePath(inputName,makePath((char*)"difference_NDG_",folderOut)), nH,nW,image );

        */
          ptr = realpath(pathIn2, actualpath);
        printf("input : %s \n", pathIn2);

        lire_nb_lignes_colonnes_image_ppm(pathIn2, &nH, &nW);
        nTaille = nH * nW;

        int nTaille3 = nTaille * 3;
        OCTET *imgColor ,*imgColorOut;
        
        allocation_tableau(imgColor, OCTET, nTaille3);
      
        lire_image_ppm(pathIn2, imgColor, nTaille);
        allocation_tableau(imgColorOut, OCTET, nTaille3);

        std::vector<std::vector<couleur>> imagec;
        imagec.resize(nH);
        for(auto & line: imagec)
            line.resize(nW);

            int tnW=3*nW;

            for (int i=0; i < nH; i++)
                {
                    for (int j=0, p=0; j < nW , p<tnW; j++,p+=3)
                    {
                        imagec[i][j].r=imgColor[i *tnW +p  ];
                        imagec[i][j].g=imgColor[i *tnW +p+1];
                        imagec[i][j].b=imgColor[i *tnW +p+2];
                    }
                }
        std::vector<std::vector<couleur>> imagec2;
        imagec2.resize(nH);
        for(auto & line: imagec2)
            line.resize(nW);

            //int tnW=3*nW;

            for (int i=0; i < nH; i++)
                {
                    for (int j=0, p=0; j < nW , p<tnW; j++,p+=3)
                    {
                        imagec2[i][j].r=imgColor[i *tnW +p  ];
                        imagec2[i][j].g=imgColor[i *tnW +p+1];
                        imagec2[i][j].b=imgColor[i *tnW +p+2];
                    }
                }
        erosionColor(imagec,nH,nW);
        saveImage((char*)makePath(inputName2,makePath((char*)"erosion_COLOR_",folderOut)), nH,nW,imagec );

        dilatationColor(imagec2,nH,nW);
        saveImage((char*)makePath(inputName2,makePath((char*)"dilatation_COLOR_",folderOut)), nH,nW,imagec2 );

        difference(imagec,imagec2,nH,nW);
        saveImage((char*)makePath(inputName2,makePath((char*)"difference_COLOR_",folderOut)), nH,nW,imagec );

    
        
}       