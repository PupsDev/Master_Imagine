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
double convert(int c)
{
    double csrgb = (double)c/256.;
    if(csrgb<= 0.04045)
    {
        return csrgb / 12.92;
    }
    else
    {
        return pow( (csrgb+0.055)/1.055,2.4);
    }

}
void convertToGreyscalePreservation(ImageRGB image, ImageG &out)
{
    int i =0;
    for(auto & line : image)
    {
        int j =0;
        for(auto &px : line)
        {
            double y = 0.2126 * convert(px.r) + 0.7152 * convert(px.g)+0.0722 * convert(px.b);
            out[i][j] =  255.*y;
            j++;
        }
        i++;
    }
}
void convertToGreyscaleLuminance(ImageRGB image, ImageG &out)
{
    int i =0;
    for(auto & line : image)
    {
        int j =0;
        for(auto &px : line)
        {
            double y = 0.299 * px.r + 0.587 * px.g+0.114 * px.b;
            out[i][j] =  y;
            j++;
        }
        i++;
    }
}
void convertToGreyscaleAverage(ImageRGB image, ImageG &out)
{
    int i =0;
    for(auto & line : image)
    {
        int j =0;
        for(auto &px : line)
        {
            double y = (px.r + px.g+px.b)/3.;
            out[i][j] =  y;
            j++;
        }
        i++;
    }
}
int  OtsuMethod(ImageG &image)
{
    unsigned int occurence [256]={0};
    float sum[256] = {0.};
    for(auto & line : image)
    {
        for(auto &px : line)
        {
            occurence[px]++ ;

        }
    }
     float avgValue=0;        
     for(int i=0; i < 256; i++)      
     {    
         
         avgValue += i * occurence[i] /(double)(image.size()*image.size()) ;  
     }         
   
     int threshold=0;          
     float maxVariance=0;        
     float w = 0, u = 0;    
 
     for(int i = 0; i < 256; i++)       
     {        
         w += occurence[i]/(double)(image.size()*image.size());  
         u += i * occurence[i]/(double)(image.size()*image.size());  
   
         float t = avgValue * w - u; 
         float variance = (t * t) / (w * (1 - w) );   
         if(variance > maxVariance)       
         {        
             maxVariance = variance;        
             threshold = i;        
         }        
     }        
   
    return threshold;     
}
int  HistoMethod(ImageG &image)
{
    unsigned int occurence [256]={0};
    float sum[256] = {0.};
    for(auto & line : image)
    {
        for(auto &px : line)
        {
            occurence[px]++ ;

        }
    }
     sum[0]  = occurence[0] /(double)(image.size()*image.size());
     for(int i=1; i < 256; i++)      
     {    
         
         sum[i] = sum[i-1] + occurence[i] /(double)(image.size()*image.size()) ;  
     }   
     int threshold=0;   
    for(int i=1; i < 256; i++)      
     {    
         
         if( sum[i] >0.5)
         {
            threshold = i;
            break;
         }
     }
    return threshold;
      
}
ImageG seuillage(ImageG image,int seuil)
{
    ImageG out;
    resize(out,image.size(),image.size());

    std::cout<<seuil<<std::endl;
    for(int i = 0 ; i < image.size();i++)
        for(int j = 0 ; j < image.size();j++)
        {
            
            if(image[i][j]<seuil)
            {
                //std::cout<<image[i][j]<<std::endl;
                out[i][j]=0;
            }
            else
                out[i][j]=255;
        }
        return out;

}