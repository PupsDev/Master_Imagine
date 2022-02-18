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
            out[i][j]= min(255,max(0,newpix+128));
            
        }
    }
        for(int i = 0 ; i < size;i++)
        for(int j = 0 ; j < size;j++)
        {
            image[i][j]=out[i][j];
        }
}
double map(double x, double b1, double b2, double b3, double b4){
  return(((x-b1)/(b2-b1))*(b4-b3)+b3);
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
            
            if( (px>0 &&pxD<0) || (px<0 &&pxD>0)) // passage zero V
            {
                res =255;
            }
            else if( (px>0 &&pxB<0) || (px<0 &&pxB>0)) // passage zero H
            {
                res =255;
            }
            int Ai = pxB-px;
            int Aj = pxD-px;
            //int G = sqrtAi*Ai + Aj*Aj
            /*float D;
           
            //cout<<D<<"\n";
            */
            out[i][j]= max(Ai,Aj);
            /*
            float D;
             if(Aj!=0)
                D= atan2(Ai,Aj);
            out[i][j]= map(D,0,2*M_PI,0,255);
            */
        }

    }
    char * folderOut = (char*)"out/"; 
        for(int i = 0 ; i < size;i++)
        for(int j = 0 ; j < size;j++)
        {
            image[i][j]=out[i][j];
        }
        saveImage( folderOut, (char*)"Passage2",(char*)"test.pgm",out);


}
void passage2(ImageG &image)
{
    int size = image.size();
    ImageG out;
    resize(out,image.size(),image.size());

    for(int i = 1 ; i < size-1; i++)
    {
        for(int j = 1 ; j < size-1; j++)
        {
            int newpix=0;
            int test=0;
            for(int u = -1 ; u < 2; u++)
                for(int v = -1 ; v < 2; v++)
                {
                    newpix = 1 * image[i][j]*image[i+u][j+v];
                    if(newpix>0)test=1;
                }
            if(test)
            {
                  out[i][j]= 255;
            }
            else
            {
               
                out[i][j]= 0;
            }
          
            
        }
    }
    char * folderOut = (char*)"out/"; 
        for(int i = 0 ; i < size;i++)
        for(int j = 0 ; j < size;j++)
        {
            image[i][j]=out[i][j];
        }
        saveImage( folderOut, (char*)"Passage2",(char*)"test.pgm",out);


}
void passageSpatola(ImageG &image, ImageG gradient)
{
    int size = image.size();
    ImageG out;
    resize(out,image.size(),image.size());
    /*std::vector<std::pair<int,int>> issou = {make_pair(0,1),
                                    make_pair(-1,1),
                                    make_pair(-1,0),
                                    make_pair(-1,-1),

                                    make_pair(0,-1),
                                    make_pair(1,-1),
                                    make_pair(1,0),
                                    make_pair(1,1)};
                                    */
    
        for(int i = 0 ; i< image.size()-1;i++)
        for(int j =  0 ; j< image.size()-1;j++)
        {
            int px = image[i][j];
            double h  = image[i+1][j] -px;
            double v  = image[i][j+1] -px;

            
            
            float angle;
             if(v!=0)
                angle= atan2(h,v);
            int voisin;
            if( angle > 7*(M_PI_4 /2) && angle < (M_PI_4 /2) )
            {
                 voisin = image[i][j+1];
            }
            else if( angle > (M_PI_4 /2) && angle < 2*(M_PI_4 /2) )
            {
                 voisin = image[i-1][j+1];
            }
            else if( angle > 2*(M_PI_4 /2) && angle < 3*(M_PI_4 /2) )
            {
                 voisin = image[i-1][j];
            }
            else if( angle > 3*(M_PI_4 /2) && angle < 4*(M_PI_4 /2) )
            {
                 voisin = image[i-1][j-1];
            }
            else if( angle > 4*(M_PI_4 /2) && angle < 5*(M_PI_4 /2) )
            {
                 voisin = image[i-1][j];
            }
            else if( angle > 5*(M_PI_4 /2) && angle < 6*(M_PI_4 /2) )
            {
                 voisin = image[i+1][j-1];
            }
            else if( angle > 6*(M_PI_4 /2) && angle < 7*(M_PI_4 /2) )
            {
                 voisin = image[i+1][j];
            }
            else
            {
                voisin = image[i+1][j+1];
            }
            image[i][j]-=128;
            voisin-=128;
            if( (image[i][j]*voisin < 0) && (voisin !=0 && image[i][j]!=0))
            {
                out[i][j]=gradient[i][j];
            }
            else
            {
                out[i][j]=0;
            }
            //out[i][j]= map(D,0,2*M_PI,0,255);
            /*
            int angle = D+ M_PI/8; 
             out[i][j]=0;
            for(int k = 0 ; k < 8;k++)
            {
                if( angle > M_PI_4 *k &&  angle < M_PI_4*(k+1))
                {
                    
                    if( image[i][j]*image[i+issou[k].first][j+issou[k].second]<0)
                        out[i][j]= 255;
                }
            }*/

        }

    char * folderOut = (char*)"out/"; 
        for(int i = 0 ; i < size;i++)
        for(int j = 0 ; j < size;j++)
        {
            image[i][j]=out[i][j];
        }
        /*
        saveImage( folderOut, (char*)"Passage2",(char*)"test.pgm",out);*/


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
    ImageG imageGradient;
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
        resize(imageGradient,nW,nH);

        loadImage(pathIn,imageRGB,nW, nH);
        saveImage( folderOut, (char*)"originale_",inputName,imageRGB);
        
        wololo(imageRGB,imageG);
        //wololo(imageRGB,imageGradient);
        //gradient(imageGradient);

         int threshold = OtsuMethod(imageG);
        cout<<threshold<<"\n";

        imageGradient = seuillage(imageGradient,threshold);

        
        //hysterique(imageGradient,0.4*threshold,threshold,imageH);
        //saveImage( folderOut, (char*)"SeuilH04",inputName,imageH);

        //blur(imageG,kernel);
         //blur(imageG,kernel);
        //saveImage( folderOut, (char*)"FlouMoyenneur",inputName,imageG);

        gradient(imageG);
        saveImage( folderOut, (char*)"Gradient_",inputName,imageG);
        profil(imageG, 42);
        /*
        //int threshold = OtsuMethod(imageG);
        //cout<<threshold<<"\n";
        imageG = seuillage(imageG,threshold);

        saveImage( folderOut, (char*)"Seuil",inputName,imageG);


        
        convolution(imageG,laplacien);
        //saveImage( folderOut, (char*)"Laplace",(char*)"test.pgm",imageG);

        passageSpatola(imageG,imageGradient);
        saveImage( folderOut, (char*)"PassageSpatola",(char*)"test.pgm",imageG);
        //int threshold = OtsuMethod(imageG);
        hysterique(imageG,0.4*threshold,threshold,imageH);
        saveImage( folderOut, (char*)"PassageSeuilH",inputName,imageH);
        */
    }

    



    return 1;
}