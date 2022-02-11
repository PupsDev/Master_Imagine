#include <iostream>
#include <stdio.h>
#include "../src/functions.cpp"
#include <iostream>
#include <string>
#include <sstream>

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
            if(seuil[i][j]==0)
                image[i][j]=out[i][j];
        }
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
    file = fopen( "densite.dat", "wb+" );
    for (int i=0; i < 256; i++)
     	fprintf(file,"%d %f\n",i, (float)occurence[i]/(float)(nW*nH) );
     fclose( file );

    file = fopen( "histogramme.dat", "wb+" );
    for (int i=0; i < 256; i++)
     	fprintf(file,"%d %d\n",i, occurence[i]);
     fclose( file );


    file = fopen( "repartition.dat", "wb+" );
    sum[0]=occurence[0]/(float)(nW*nH);
    for (int i=1; i < 256; i++)
    {
        sum[i]=sum[i-1]+(float)occurence[i]/(float)(nW*nH);
        fprintf(file,"%d %f\n",i, sum[i]);
        
    }
}
float carteDeVerite(ImageG seuil, ImageG seuilGimp, float &x, float &y,float &recall,float &precision,float &f1)
{
    float VP=0,VN=0,FP=0,FN=0;

    for(int i = 0 ; i < seuil.size();i++)
        for(int j = 0 ; j < seuil.size();j++)
        {

            if( (seuil[i][j] == seuilGimp[i][j])&&(seuilGimp[i][j]==0) ) //VP
            {
                VP++;
            }
            if( (seuil[i][j] == seuilGimp[i][j])&&(seuilGimp[i][j]==255) ) //VN
            {
                VN++;
            }
            if( (seuil[i][j] !=seuilGimp[i][j]) &&(seuilGimp[i][j]==0) ) // FOND FP
            {
                FP++;
            }
            if( (seuil[i][j] !=seuilGimp[i][j]) &&(seuilGimp[i][j]==255) ) // OBJET FN
            {   
                FN++;

            }
        }
        //std::cout<<VP<<" "<<VN<<" "<<FP<<" "<<FN<<std::endl;
        y = VP/(VP+FN);
        x = 1.-VN/(VN+FP);
        recall=VP/(VP+FN);
        precision=VP/(VP+FP);
        f1 = (2*recall+precision)/(recall+precision);
        //std::cout<<"recall "<<VP/(VP+FN)<<"precision "<<VP/(VP+FP)<<std::endl;
         //std::cout<<"F1 "<<(2*recall+precision)/(recall+precision)<<std::endl;


        float d = sqrt((x)*(x)+(y-1.)*(y-1.));
        return d;
}
std::string toString(auto &i) {
    std::stringstream ss;
    ss << i;
 
    return ss.str();
}
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
    char * pathIn2 = makePath((char*)"gimp_seuil.pgm",folderIn);
    
    int color = ppmOrPgm(inputName);
    ImageRGB imageRGB;
    ImageG imageG;
    ImageG imageG2;

    ImageG imageGimp;


    if(color)
    {
        
        lire_nb_lignes_colonnes_image_ppm(pathIn, &nH, &nW);
        resize(imageRGB,nW,nH);
        resize(imageG,nW,nH);
        resize(imageG2,nW,nH);

        resize(imageGimp,nW,nH);    
        loadImage(pathIn2,imageGimp,nW, nH);

        loadImage(pathIn,imageRGB,nW, nH);
        saveImage( folderOut, (char*)"originale_",inputName,imageRGB);

        convertToGreyscalePreservation(imageRGB,imageG);
        saveImage( folderOut, (char*)"originale_",(char*)"greyPreservation.pgm",imageG);
/*
        histoDensiteRepartition(imageG, nW,  nH);

        convertToGreyscaleAverage(imageRGB,imageG);
        saveImage( folderOut, (char*)"originale_",(char*)"greyAverage.pgm",imageG);


        convertToGreyscaleLuminance(imageRGB,imageG);
        saveImage( folderOut, (char*)"originale_",(char*)"greyLuminance.pgm",imageG);
        */

        int threshold = OtsuMethod(imageG);
        //int threshold = HistoMethod(imageG);
        float d =1.,d2;
        int max=0;
        float x,y;
        float recall,precision,f1;
        float recall2,precision2,f12;

        FILE *file = fopen( "roc.dat", "wb+" );
        for(int i = 0 ; i < 256;i++)
        {
            imageG2 = seuillage(imageG,i );
            //erosion(imageG, nH, nW,  0);
            //dilatation(imageG, nH, nW,  0);
            //std::string test = string("seuillage");
            std::string s = "seuillage" + toString(i)+".pgm";
            //outPath = makePath(".pgm", outPath);
            //std::cout<<outPath<<std::endl;
            //char * outpah = s.str();
            char *cstr = new char[s.length() + 1];
            strcpy(cstr, s.c_str());
            // do stuff
            
            //saveImage( folderOut, (char*)"originale_",cstr,imageG2);

            d2 = carteDeVerite(imageG2, imageGimp,x,y,recall,precision,f1);
            std::cout<<x <<y<<std::endl;
            fprintf(file,"%f %f\n",x, y);
        
            if(d2<d)
            {
                d=d2;
                recall2=recall;
                precision2=precision;
                f12=f1;
                max = i;
            }

        } fclose( file );
        std::cout<<"final "<<d<<" "<<max<< std::endl;
        std::cout<<recall2<<" "<<precision2<<" "<<f12<<" "<< std::endl;

        ImageG kernel = {
                { 1, 1, 1 },
                { 1, 1, 1 },
                { 1, 1, 1 }
            };
        //blur(imageRGB,kernel);
         //blur(imageRGB,kernel);

        //saveImage( folderOut, (char*)"blured_",inputName,imageRGB);


        blur(imageRGB,imageG,kernel);
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