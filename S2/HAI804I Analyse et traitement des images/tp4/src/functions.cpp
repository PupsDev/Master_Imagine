#include "image_ppm.h"
#include <stdlib.h>
#include <vector>
#include <string>

struct couleur
{
  int r,g,b;
};
typedef couleur couleur;
typedef std::vector<std::vector<int>> ImageG;
typedef std::vector<std::vector<couleur>> ImageRGB;
char * makePath(char* path, char * folder)
{
    char * pathIn = (char*)malloc(sizeof(char) * (strlen(path)+strlen(folder)+2));

    strcpy( pathIn, folder );
    strcat(pathIn,path);
    return pathIn;
}

float psnr(OCTET* ImgIn,OCTET* ImgOut, int nH, int nW)
{
  int d =256-1;
  float diff =0.;
  for(int i =0; i < nH*nW ; i++)
  {
    diff += sqrt( (ImgIn[i]-ImgOut[i])*(ImgIn[i]-ImgOut[i]) + (ImgIn[i+1]-ImgOut[i+1])* (ImgIn[i+1]-ImgOut[i+1]) +(ImgIn[i+2]-ImgOut[i+2])*(ImgIn[i+2]-ImgOut[i+2]));
  }

  float EQM = 1./(nH*nW) * diff;
  return 10*log10(3*d*d / EQM);
}

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
void resize(ImageG& image, int nW,int nH)
{
    image.resize(nH);
    for(auto & line: image)
        line.resize(nW);
}
void resize(ImageRGB& image, int nW,int nH)
{
    image.resize(nH);
    for(auto & line: image)
        line.resize(nW);
}
bool ppmOrPgm(char * inputName)
{
    std::string s = inputName;
    std::string token = s.substr(0, s.find("."));
    s.erase(0, s.find(".") + std::string(".").length());
     token = s.substr(0, s.find("."));

    return (token == "pgm")?0:1;
}
void saveImage(char * pathOut,char * salt,char * inputName, ImageG image )
{
    saveImage(makeFinalPath( pathOut, salt,inputName), image.size(),image[0].size(),image);
}
void saveImage(char * pathOut,char * salt,char * inputName,ImageRGB image )
{
    saveImage(makeFinalPath( pathOut, salt,inputName), image.size(),image[0].size(),image);
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