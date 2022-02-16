#include <iostream>
#include <stdio.h>
#include "../src/functions.cpp"
using namespace std;

void gradient(ImageG &image)
{
        ImageG imageG;
        resize(imageG,image.size(),image.size());
        char * folderOut = (char*)"out/"; 

        for(int i = 0 ; i< image.size()-1;i++)
        for(int j = 0 ; j< image.size()-1;j++)
        {
            int px = image[i][j];
            double h  = image[i+1][j] -px;
            double v  = image[i][j+1] -px;

            imageG[i][j] = sqrt(h*h + v*v);
            
        }
        //saveImage( folderOut, (char*)"originale_",(char *)"gradient.pgm",imageG);

        for(int i = 0 ; i< image.size();i++)
        for(int j = 0 ; j< image.size();j++)
        {
            image[i][j]=imageG[i][j];
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
void profil(ImageG image, int l)
{
    FILE *fp = fopen("profilLigne.dat","w+");
    for (int j=0; j < image.size(); j++)
    {
    fprintf(fp,"%d %d\n", j, image[l][j]);

    }
    fclose(fp);

}
void wololo(ImageRGB image, ImageG &out)
{
    for(int i = 0 ; i< image.size();i++)
        for(int j = 0 ; j< image.size();j++)
        {
            out[i][j] = (image[i][j].r+image[i][j].g+image[i][j].b)/3.;
        }

}
void hysterique(ImageG image, int SH, int SB,ImageG &out)
{

    for(int i = 0 ; i< image.size();i++)
    for(int j = 0 ; j< image.size();j++)
    {
        int px = image[i][j]; 
        if ( px>SH)
            px = 255;
        if(px<SB)
            px =0;
        if( px<= SH && px >= SB)
        {
            int test =0;
            for(int k = -1 ; k<2;k++)
                for(int p=-1;p<2;p++)
                {
                    if( ((i+k)>=0 && (i+k)<=image.size()) &&  ((j+p)>=0 && (j+p)<=image.size()))
                    {
                        if(image[i+k][j+p]==255) test=1;
                    }
                 }
            if(test)
                px = 255;
            else
                px =0;
        }
        out[i][j]=px; 

    }
}
void blur(ImageG &image, ImageG kernel)
{
    int size = image.size();
    int boundary = kernel.size()/2;
    int kernelSize = kernel.size();
    int div = kernelSize*kernelSize;
    ImageG out;
    resize(out,image.size(),image.size());

    for(int i = boundary ; i < size-(boundary+1); i++)
    {
        for(int j = boundary ; j < size-(boundary+1); j++)
        {
            int newpix=0;
            for(int u = -boundary ; u < (boundary+1); u++)
                for(int v = -boundary ; v < (boundary+1); v++)
                    newpix += 1./div * kernel[boundary+u][boundary+v] * image[u+i][v+j];
            out[i][j]= newpix;
            
        }
    }
        for(int i = 0 ; i < size;i++)
        for(int j = 0 ; j < size;j++)
        {
            if(out[i][j])
                image[i][j]=out[i][j];
        }
}
void convolution(ImageG &image, ImageG kernel)
{
    int size = image.size();
    int boundary = kernel.size()/2;
    int kernelSize = kernel.size();
    int div = kernelSize*kernelSize;
    ImageG out;
    resize(out,image.size(),image.size());

    for(int i = boundary ; i < size-(boundary+1); i++)
    {
        for(int j = boundary ; j < size-(boundary+1); j++)
        {
            int newpix=0;
            for(int u = -boundary ; u < (boundary+1); u++)
                for(int v = -boundary ; v < (boundary+1); v++)
                    newpix += kernel[boundary+u][boundary+v] * image[u+i][v+j];
            out[i][j]= newpix;//+128;
            //cout<<newpix<<"\n";
            
        }
    }
        for(int i = 0 ; i < size;i++)
        for(int j = 0 ; j < size;j++)
        {
            image[i][j]=out[i][j];
        }
}
void passage(ImageG &image)
{
    int size = image.size();
    ImageG out;
    resize(out,image.size(),image.size());

    for(int i = 0 ; i < size-1; i++)
    {
        for(int j = 0 ; j < size-1; j++)
        {
            int px = image[i][j]; 
            int pxD = image[i][j+1]; 
            int pxB = image[i+1][j]; 
            int res =0;
            /*
            if( (px>0 &&pxD<0) || (px<0 &&pxD>0)) // passage zero V
            {
                res =255;
            }
            else if( (px>0 &&pxB<0) || (px<0 &&pxB>0)) // passage zero H
            {
                res =255;
            }*/
            int Ai = abs(pxB-px);
            int Aj = abs(pxD-px);
            /*float D;
            if(Ai!=0)
                D= atan(Aj/Ai);
            //cout<<D<<"\n";
            */
            out[i][j]= max( Ai,Aj);
            //out[i][j]= 50;//*D;
        }

    }
        for(int i = 0 ; i < size;i++)
        for(int j = 0 ; j < size;j++)
        {
            image[i][j]=out[i][j];
        }
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
    int color = ppmOrPgm(inputName);


    ImageRGB imageRGB;
    ImageG imageG;
    resize(imageG,nW,nH);

    ImageG imageH;
    ImageG kernel = {
        { 1, 1, 1 },
        { 1, 1, 1 },
        { 1, 1, 1 }
    };
    ImageG laplacien = {
        { 0, 1, 0 },
        { 1, 0, -1 },
        { 0, -1, 0 }
    };
        
    if(color)
    {
        
        lire_nb_lignes_colonnes_image_ppm(pathIn, &nH, &nW);
        resize(imageRGB,nW,nH);
        resize(imageG,nW,nH);
        resize(imageH,nW,nH);

        loadImage(pathIn,imageRGB,nW, nH);
        saveImage( folderOut, (char*)"originale_",inputName,imageRGB);
        
        wololo(imageRGB,imageG);
    /*
        blur(imageG,kernel);
         blur(imageG,kernel);
        saveImage( folderOut, (char*)"FlouMoyenneur",inputName,imageG);

        gradient(imageG);
        saveImage( folderOut, (char*)"Gradient_",inputName,imageG);
        profil(imageG, 42);
        */
        int threshold = OtsuMethod(imageG);
        //imageG = seuillage(imageG,threshold);
        ///saveImage( folderOut, (char*)"Seuil",inputName,imageG);
        convolution(imageG,laplacien);
        saveImage( folderOut, (char*)"Laplace",(char*)"test.pgm",imageG);
        passage(imageG);
        saveImage( folderOut, (char*)"Passage",(char*)"test.pgm",imageG);

        //hysterique(imageG,threshold-5,threshold+5,imageH);
        //saveImage( folderOut, (char*)"SeuilH",inputName,imageH);
    }

    



    return 1;
}