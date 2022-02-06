#include <iostream>
#include <stdio.h>
#include "../src/functions.cpp"


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
void seuillage(ImageG &image,int seuil)
{
    std::cout<<seuil<<std::endl;
    for(int i = 0 ; i < image.size();i++)
        for(int j = 0 ; j < image.size();j++)
        {
            
            if(image[i][j]<seuil)
            {
                //std::cout<<image[i][j]<<std::endl;
                image[i][j]=0;
            }
            else
                image[i][j]=255;
        }


}
void blur(ImageRGB &image, ImageG kernel)
{
    int size = image.size();
    int boundary = kernel.size()/2;
    int kernelSize = kernel.size();
    int div = kernelSize*kernelSize;
    ImageRGB out;
    resize(out,image.size(),image.size());

    for(int i = boundary ; i < size-(boundary+1); i++)
    {
        for(int j = boundary ; j < size-(boundary+1); j++)
        {

            // compute kernel
            couleur newpix;
            newpix.r =0;
            newpix.g =0;
            newpix.b =0;
            
            for(int u = -boundary ; u < (boundary+1); u++)
            {
                for(int v = -boundary ; v < (boundary+1); v++)
                {
                    newpix.r += kernel[boundary+u][boundary+v] * image[u+i][v+j].r;
                    newpix.g += kernel[boundary+u][boundary+v] * image[u+i][v+j].g;
                    newpix.b += kernel[boundary+u][boundary+v] * image[u+i][v+j].b;
                    
                }
            }
            newpix.r/=div;
            newpix.g/=div;
            newpix.b/=div;
            out[i][j]=newpix;
            
        }
    }
        for(int i = 0 ; i < size;i++)
        for(int j = 0 ; j < size;j++)
        {
            image[i][j]=out[i][j];
        }
}
void blur(ImageRGB &image, ImageG seuil, ImageG kernel)
{
    int size = image.size();
    int boundary = kernel.size()/2;
    int kernelSize = kernel.size();
    int div = kernelSize*kernelSize;
    ImageRGB out;
    resize(out,image.size(),image.size());

    for(int i = boundary ; i < size-(boundary+1); i++)
    {
        for(int j = boundary ; j < size-(boundary+1); j++)
        {

            // compute kernel
            couleur newpix;
            newpix.r =0;
            newpix.g =0;
            newpix.b =0;
            
            for(int u = -boundary ; u < (boundary+1); u++)
            {
                for(int v = -boundary ; v < (boundary+1); v++)
                {
                    newpix.r += kernel[boundary+u][boundary+v] * image[u+i][v+j].r;
                    newpix.g += kernel[boundary+u][boundary+v] * image[u+i][v+j].g;
                    newpix.b += kernel[boundary+u][boundary+v] * image[u+i][v+j].b;
                    
                }
            }
            newpix.r/=div;
            newpix.g/=div;
            newpix.b/=div;
            out[i][j]=newpix;
            
        }
    }
        for(int i = 0 ; i < size;i++)
        for(int j = 0 ; j < size;j++)
        {
            if(seuil[i][j]==255)
                image[i][j]=out[i][j];
        }
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
    int color = ppmOrPgm(inputName);
    ImageRGB imageRGB;
    ImageG imageG;

    if(color)
    {
        
        lire_nb_lignes_colonnes_image_ppm(pathIn, &nH, &nW);
        resize(imageRGB,nW,nH);
        resize(imageG,nW,nH);
        
        loadImage(pathIn,imageRGB,nW, nH);
        saveImage( folderOut, (char*)"originale_",inputName,imageRGB);
        convertToGreyscalePreservation(imageRGB,imageG);
        saveImage( folderOut, (char*)"originale_",(char*)"greyPreservation.pgm",imageG);

        convertToGreyscaleAverage(imageRGB,imageG);
        saveImage( folderOut, (char*)"originale_",(char*)"greyAverage.pgm",imageG);


        convertToGreyscaleLuminance(imageRGB,imageG);
        saveImage( folderOut, (char*)"originale_",(char*)"greyLuminance.pgm",imageG);

        int threshold = OtsuMethod(imageG);
        //int threshold = HistoMethod(imageG);
        seuillage(imageG,threshold );
        saveImage( folderOut, (char*)"originale_",(char*)"seuillage.pgm",imageG);

        ImageG kernel = {
                { 1, 1, 1 },
                { 1, 1, 1 },
                { 1, 1, 1 }
            };
        //blur(imageRGB,kernel);

        //saveImage( folderOut, (char*)"blured_",inputName,imageRGB);


        blur(imageRGB,imageG,kernel);

        saveImage( folderOut, (char*)"bluredSeuil_",inputName,imageRGB);

         
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