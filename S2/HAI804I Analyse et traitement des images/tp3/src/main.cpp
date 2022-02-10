#include <iostream>
#include <stdio.h>
#include "image_ppm.h"
#include <stdlib.h>
#include <vector>
#include <math.h>
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
void histo(std::vector<std::vector<int>> image,int nW, int nH)
{
    unsigned int occurence [256]={0};
    float sum =0.;
    for (int i=0; i < nH; i++)
    {
        for (int j=0; j < nW ; j++)
        {
            occurence[image[i][j]]++;

        }
    }
     FILE * file;
    file = fopen( "histoBlack.dat", "wb+" );
    for (int i=0; i < 256; i++)
     	fprintf(file,"%d %f\n",i, (float)occurence[i]/(float)(nW*nH)*100. );
     fclose( file );
    
}
void histo(std::vector<std::vector<couleur>> image,int nW, int nH)
{
    unsigned int occurenceR [256]={0};
    unsigned int occurenceG [256]={0};
    unsigned int occurenceB [256]={0};

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
    file = fopen( "histoR.dat", "wb+" );
    for (int i=0; i < 256; i++)
     	fprintf(file,"%d %f\n",i, (float)occurenceR[i]/(float)(nW*nH) *100.);
    fclose( file );
        file = fopen( "histoG.dat", "wb+" );
    for (int i=0; i < 256; i++)
     	fprintf(file,"%d %f\n",i, (float)occurenceG[i]/(float)(nW*nH) *100. );
    fclose( file );
        file = fopen( "histoB.dat", "wb+" );
    for (int i=0; i < 256; i++)
     	fprintf(file,"%d %f\n",i, (float)occurenceB[i]/(float)(nW*nH) *100. );
    fclose( file );
    
}
void proba(std::vector<std::vector<int>> image,int nW, int nH)
{
    unsigned int occurence [256]={0};
    float sum =0.;
    for (int i=0; i < nH; i++)
    {
        for (int j=0; j < nW ; j++)
        {
            occurence[image[i][j]]++;

        }
    }
     FILE * file;
    file = fopen( "histoBlack.dat", "wb+" );
    for (int i=0; i < 256; i++)
     	fprintf(file,"%d %d\n",i, occurence[i]);
     fclose( file );
    
}
void repartition(std::vector<std::vector<int>> image,int nW, int nH)
{
    unsigned int occurence [256]={0};
    float sum =0.;
    for (int i=0; i < nH; i++)
    {
        for (int j=0; j < nW ; j++)
        {
            occurence[image[i][j]]++;

        }
    }

     FILE * file;
    file = fopen( "repartition.dat", "wb+" );
    for (int i=0; i < 256; i++)
    {
        fprintf(file,"%d %f\n",i, sum);
        sum+=(float)occurence[i]/(float)(nW*nH);
    }
     	
     fclose( file );
    
}
void histo2(std::vector<std::vector<couleur>> image,int nW, int nH)
{
    unsigned int occurenceR [256]={0};
    unsigned int occurenceG [256]={0};
    unsigned int occurenceB [256]={0};

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
    file = fopen( "histo2R.dat", "wb+" );
    for (int i=0; i < 256; i++)
     	fprintf(file,"%d %f\n",i, (float)occurenceR[i]/(float)(nW*nH) *100.);
    fclose( file );
        file = fopen( "histo2G.dat", "wb+" );
    for (int i=0; i < 256; i++)
     	fprintf(file,"%d %f\n",i, (float)occurenceG[i]/(float)(nW*nH) *100. );
    fclose( file );
        file = fopen( "histo2B.dat", "wb+" );
    for (int i=0; i < 256; i++)
     	fprintf(file,"%d %f\n",i, (float)occurenceB[i]/(float)(nW*nH) *100. );
    fclose( file );
    
}
void expansion(std::vector<std::vector<int>> image,std::vector<std::vector<int>> &imageOut,int nW, int nH)
{
    unsigned int occurence [256]={0};
    int v0,v1,vmin=0,vmax=255;

    for (int i=0; i < nH; i++)
    {
        for (int j=0; j < nW ; j++)
        {
            occurence[image[i][j]]++;

        }
    }
    bool findV0=false, findV1=false;
    for(int i =0; i < 256;i++)
    {
        if(!findV0&& occurence[i]!=0)
        {
            v0 = i;
            findV0=true;
        }
        if(findV0 && !findV1 && occurence[i]==0)
        {
            v1 = i;
            findV1=true;
        }

    }
    printf("%d %d\n", v0,v1);
    //printf("%f" ,vmax*v0);
    double d = (vmin * v1 -vmax*v0)/(v1-v0);
    double g = (vmax-vmin)/(v1-v0);
    printf("%f %f\n", d,g);
    
    for (int i=0; i < nH; i++)
    {
        for (int j=0; j < nW ; j++)
        {
            int pix = image[i][j];
            if(pix > v0 && pix <v1)
            {
                pix = g*pix +d;
                imageOut[i][j]=pix;
            }

        }
    }
    //histo( imageOut, nW,  nH);

}
float expand(int color, double vo , double v1)
{
    
    return (255*(color-vo))/(v1-vo);
}
void expansion(std::vector<std::vector<couleur>> image,std::vector<std::vector<couleur>> &imageOut,int nW, int nH)
{
    unsigned int occurenceR [256]={0};
    unsigned int occurenceG [256]={0};
    unsigned int occurenceB [256]={0};

    int Rv0,Rv1,Rvmin=0,Rvmax=255;
    int Gv0,Gv1,Gvmin=0,Gvmax=255;
    int Bv0,Bv1,Bvmin=0,Bvmax=255;

    for (int i=0; i < nH; i++)
    {
        for (int j=0; j < nW ; j++)
        {
            occurenceR[image[i][j].r]++;
            occurenceG[image[i][j].g]++;
            occurenceB[image[i][j].b]++;


        }
    }
    bool findV0=false, findV1=false;
    for(int i =0; i < 256;i++)
    {
        if(!findV0&& occurenceR[i]!=0)
        {
            Rv0 = i;
            findV0=true;
        }
        if(findV0 && !findV1 && occurenceR[i]==0)
        {
            Rv1 = i;
            findV1=true;
        }

    }
    findV0=false, findV1=false;
    for(int i =0; i < 256;i++)
    {
        if(!findV0&& occurenceG[i]!=0)
        {
            Gv0 = i;
            findV0=true;
        }
        if(findV0 && !findV1 && occurenceG[i]==0)
        {
            Gv1 = i;
            findV1=true;
        }

    }
    findV0=false, findV1=false;
    for(int i =0; i < 256;i++)
    {
        if(!findV0&& occurenceB[i]!=0)
        {
            Bv0 = i;
            findV0=true;
        }
        if(findV0 && !findV1 && occurenceB[i]==0)
        {
            Bv1 = i;
            findV1=true;
        }

    }
    printf("%d %d\n%d %d\n%d %d\n", Rv0,Rv1,Gv0,Gv1,Bv0,Bv1);

     double dR = (double)(Rvmin * Rv1 -Rvmax*Rv0)/(double)(Rv1-Rv0);
     double gR = (double)(Rvmax-Rvmin)/(double)(Rv1-Rv0);

     double dG = (double)(Gvmin * Gv1 -Gvmax*Gv0)/(double)(Gv1-Gv0);
     double gG = (double)(Gvmax-Gvmin)/(double)(Gv1-Gv0);

     double dB = (double)(Bvmin * Bv1 -Bvmax*Bv0)/(double)(Bv1-Bv0);
     double gB = (double)(Bvmax-Bvmin)/(double)(Bv1-Bv0);
    
     printf("%f %f\n", dR,gR);
     printf("%f %f\n", dG,gG);
     printf("%f %f\n", dB,gB);


    for (int i=0; i < nH; i++)
    {
        for (int j=0; j < nW ; j++)
        {
            couleur pix = image[i][j];

            if(pix.r >= Rv0 && pix.r <=Rv1)
            {

                imageOut[i][j].r=std::max(0,std::min(255,(int)expand(pix.r,Rv0,Rv1)));
            }
            if(pix.g >= Gv0 && pix.g <=Gv1)
            {

                imageOut[i][j].g=std::max(0,std::min(255,(int)expand(pix.g,Gv0,Gv1)));
            }
            if(pix.b >= Bv0 && pix.b <=Bv1)
            {

                imageOut[i][j].b=std::max(0,std::min(255,(int)expand(pix.b,Bv0,Bv1)));
            }
            //printf("%d %d %d \n", imageOut[i][j].r,imageOut[i][j].g,imageOut[i][j].b);
        }
    }
}
void seuillageExtrema1(std::vector<std::vector<couleur>> image,std::vector<std::vector<couleur>> &imageOut,int nW, int nH)
{
    unsigned int occurenceR [256]={0};
    unsigned int occurenceG [256]={0};
    unsigned int occurenceB [256]={0};

    for (int i=0; i < nH; i++)
    {
        for (int j=0; j < nW ; j++)
        {
            occurenceR[image[i][j].r]++;
            occurenceG[image[i][j].g]++;
            occurenceB[image[i][j].b]++;


        }
    }
    float sumR=0.f,sumG=0.f,sumB=0.f;
    int v0R,v0G,v0B,v1R,v1G,v1B;
    bool RfindV0=false, RfindV1=false;
    bool GfindV0=false, GfindV1=false;
    bool BfindV0=false, BfindV1=false;
    // 5 percent
    float threshold = 5.;
    float maxThreshold = 100.-threshold;

    for (int i=0; i < 256; i++)
    {
        sumR+=(float)occurenceR[i]/(float)(nW*nH)*100.;
        
        if(!RfindV0 && sumR > threshold)
        {
            //printf("%f\n", sumR);
            //printf("%d\n", i);
            v0R = i;
            RfindV0=true;
        }
        if(RfindV0 && !RfindV1 && sumR > maxThreshold)
        {
            v1R = i;
            RfindV1=true; 
        }
        sumG+=(float)occurenceG[i]/(float)(nW*nH)*100.;
        if(!GfindV0 && sumG > threshold)
        {
            v0G = i;
            GfindV0=true;
        }
        if(GfindV0 && !GfindV1 && sumG > maxThreshold)
        {
            v1G = i;
            GfindV1=true; 
        }
        sumB+=(float)occurenceB[i]/(float)(nW*nH)*100.;
        if(!BfindV0 && sumB > threshold)
        {
            v0B = i;
            BfindV0=true;
        }
        if(BfindV0 && !BfindV1 && sumB > maxThreshold)
        {
            v1B = i;
            BfindV1=true; 
        }
    }
    for (int i=0; i < nH; i++)
    {
        for (int j=0; j < nW ; j++)
        {
            couleur pix = image[i][j]; 

            if(pix.r>v0R && pix.r<v1R)
                imageOut[i][j].r = pix.r;
            else if(pix.r <=v0R)
                imageOut[i][j].r=v0R;
            else if(pix.r >=v1R)
                imageOut[i][j].r=v1R;

                            
            if(pix.g>v0G && pix.g<v1G)
                imageOut[i][j].g = pix.g;
            else if(pix.g <=v0G)
                imageOut[i][j].g=v0G;
            else if(pix.g >=v1G)
                imageOut[i][j].g=v1G;

                            
            if(pix.b>v0B && pix.b<v1B)
                imageOut[i][j].b = pix.b;
            else if(pix.b <=v0B)
                imageOut[i][j].b=v0B;
            else if(pix.b >=v1B)
                imageOut[i][j].b=v1B;

            //printf("%d %d %d ", imageOut[i][j].r,imageOut[i][j].g,imageOut[i][j].b);

        }
    }
}
void seuillageExtrema(std::vector<std::vector<couleur>> image,std::vector<std::vector<couleur>> &imageOut,int nW, int nH)
{
    unsigned int occurenceR [256]={0};
    unsigned int occurenceG [256]={0};
    unsigned int occurenceB [256]={0};

    for (int i=0; i < nH; i++)
    {
        for (int j=0; j < nW ; j++)
        {
            imageOut[i][j]=image[i][j];


        }
    }
    for (int i=0; i < nH; i++)
    {
        for (int j=0; j < nW ; j++)
        {
            occurenceR[image[i][j].r]++;
            occurenceG[image[i][j].g]++;
            occurenceB[image[i][j].b]++;


        }
    }
    float sumR=0.f,sumG=0.f,sumB=0.f;
    int v0R,v0G,v0B,v1R,v1G,v1B;
    bool RfindV0=false, RfindV1=false;
    bool GfindV0=false, GfindV1=false;
    bool BfindV0=false, BfindV1=false;
    // 5 percent
    float threshold = 20.;
    float maxThreshold = 100.-threshold;
    v0R=30;
    v0G=30;
    v0B=30;
    
    v1R=220;
    v1G=220;
    v1B=220;
    
    for (int i=0; i < nH; i++)
    {
        for (int j=0; j < nW ; j++)
        {
            couleur pix = image[i][j]; 

            if(pix.r>v0R && pix.r<v1R)
                imageOut[i][j].r = pix.r;
            else if(pix.r <=v0R)
                imageOut[i][j].r=v0R;
            else if(pix.r >=v1R)
                imageOut[i][j].r=v1R;

                            
            if(pix.g>v0G && pix.g<v1G)
                imageOut[i][j].g = pix.g;
            else if(pix.g <=v0G)
                imageOut[i][j].g=v0G;
            else if(pix.g >=v1G)
                imageOut[i][j].g=v1G;

                            
            if(pix.b>v0B && pix.b<v1B)
                imageOut[i][j].b = pix.b;
            else if(pix.b <=v0B)
                imageOut[i][j].b=v0B;
            else if(pix.b >=v1B)
                imageOut[i][j].b=v1B;

            //printf("%d %d %d ", imageOut[i][j].r,imageOut[i][j].g,imageOut[i][j].b);

        }
    }
}
void egalisation(std::vector<std::vector<int>> image,std::vector<std::vector<int>> &imageOut,int nW, int nH)
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
    sum[0]=occurence[0];
    for (int i=1; i < 256; i++)
    {
        sum[i]=sum[i-1]+(float)occurence[i]/(float)(nW*nH);
        fprintf(file,"%d %f\n",i, sum[i]);
        
    }
     	
     fclose( file );

    for (int i=0; i < nH; i++)
    {
        for (int j=0; j < nW ; j++)
        {
            int px =   image[i][j];
            imageOut[i][j] = 255*sum[px];
        }
    }
    
    for (int i=0; i < nH; i++)
    {
        for (int j=0; j < nW ; j++)
        {
            occurence[imageOut[i][j]]++;

        }
    }

    file = fopen( "densite2.dat", "wb+" );
    for (int i=0; i < 256; i++)
     	fprintf(file,"%d %f\n",i, (float)occurence[i]/(float)(nW*nH) );
     fclose( file );

    file = fopen( "histogramme2.dat", "wb+" );
    for (int i=0; i < 256; i++)
     	fprintf(file,"%d %d\n",i, occurence[i]);
     fclose( file );


    file = fopen( "repartition2.dat", "wb+" );
    sum[0]=occurence[0];
    for (int i=1; i < 256; i++)
    {
        sum[i]=sum[i-1]+(float)occurence[i]/(float)(nW*nH);
        fprintf(file,"%d %f\n",i, sum[i]);
        
    }
     	
     fclose( file );

}

void egalisation2(std::vector<std::vector<int>> &image,std::vector<std::vector<int>> &imageOut,int nW, int nH)
{
    unsigned int occurence [256]={0};
    unsigned int occurence2 [256]={0};
    float sum[256] = {0.};
    float sum2[256] = {0.};
    for (int i=0; i < nH; i++)
    {
        for (int j=0; j < nW ; j++)
        {
            occurence[image[i][j]]++;

        }
    }
    for (int i=0; i < nH; i++)
    {
        for (int j=0; j < nW ; j++)
        {
            occurence2[imageOut[i][j]]++;

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
    sum[0]=occurence[0];
    for (int i=1; i < 256; i++)
    {
        sum[i]=sum[i-1]+(float)occurence[i]/(float)(nW*nH);
        fprintf(file,"%d %f\n",i, sum[i]);
        
    }
     	
     fclose( file );
     
     sum[0]=occurence[0]/(float)(nW*nH);
     sum2[0]=occurence2[0]/(float)(nW*nH);
     //printf("%f \n", sum2[0]);
    for (int i=1; i < 256; i++)
    {
        sum[i]=sum[i-1]+(float)occurence[i]/(float)(nW*nH);
        sum2[i]=sum2[i-1]+(float)occurence2[i]/(float)(nW*nH);
         printf("%f ", sum[i]);
         printf("%f \n", sum2[i]);
        //fprintf(file,"%d %f\n",i, sum[i]);
        
    }
    for (int i=0; i < nH; i++)
    {
        for (int j=0; j < nW ; j++)
        {
            int px =   image[i][j];
           
            image[i][j] = 255*sum[px];
        }
    }
        for (int i=0; i < nH; i++)
    {
        for (int j=0; j < nW ; j++)
        {
            int px =   imageOut[i][j];
            //printf("%f \n", sum2[px]);
            imageOut[i][j] = 255*sum2[px];
        }
    }
    
    for (int i=0; i < nH; i++)
    {
        for (int j=0; j < nW ; j++)
        {
            int px =   imageOut[i][j];
            float p = (float)px /255.;
            int m;
            bool f = true;
            for(int k = 0 ; k<256;k++)
            {
                if(f && sum[k]>=p)
                {
                    m=k;
                    f = false;
                }
            }
            imageOut[i][j] = m;
        }
    }
    


    file = fopen( "densite2.dat", "wb+" );
    for (int i=0; i < 256; i++)
     	fprintf(file,"%d %f\n",i, (float)occurence[i]/(float)(nW*nH) );
     fclose( file );

    file = fopen( "histogramme2.dat", "wb+" );
    for (int i=0; i < 256; i++)
     	fprintf(file,"%d %d\n",i, occurence[i]);
     fclose( file );


    file = fopen( "repartition2.dat", "wb+" );
    sum[0]=occurence[0];
    for (int i=1; i < 256; i++)
    {
        sum[i]=sum[i-1]+(float)occurence[i]/(float)(nW*nH);
        fprintf(file,"%d %f\n",i, sum[i]);
        
    }
     	
     fclose( file );
     

}
int main(int argc, char* argv[]) {
    char inputName[250];
     char inputName2[250];
    int nH, nW;
   char * folderIn = (char*)"../../res/";
    char * folderOut = (char*)"out/"; 
    int color,exo;
    if (argc != 5) 
    {
        printf("Usage: -ImageIn.ppm -type (0 for pgm 1 for ppm) -Image2.pgm -exo  \n"); 
        exit (1) ;
    }

    sscanf (argv[1],"%s",inputName) ;
    sscanf (argv[2],"%d",&color) ;
    sscanf (argv[3],"%d",&exo) ;
    sscanf (argv[4],"%s",inputName2) ;

    char * pathIn = makePath(inputName,folderIn);
     char * pathIn2 = makePath(inputName2,folderIn);
    
    std::vector<std::vector<couleur>> imageRGB;
    std::vector<std::vector<couleur>> imageRGBOut;
    std::vector<std::vector<int>> imageG;
    std::vector<std::vector<int>> imageOut;

    if(color)
    {
        
        lire_nb_lignes_colonnes_image_ppm(pathIn, &nH, &nW);
        imageRGB.resize(nH);
        for(auto & line: imageRGB)
            line.resize(nW);

        imageRGBOut.resize(nH);
        for(auto & line: imageRGBOut)
            line.resize(nW);
        
        loadImage(pathIn,imageRGB,nW, nH);
        saveImage(makeFinalPath( folderOut, (char*)"_RGB_originale_",inputName), nH,nW,imageRGB);
        switch(exo)
        {
            case 1:
                histo(imageRGB, nW,  nH);
                expansion(imageRGB,imageRGBOut,nW, nH);
               
                saveImage(makeFinalPath( folderOut, (char*)"_RGB_diff_",inputName), nH,nW,imageRGBOut);
                histo2(imageRGBOut, nW,  nH);
            break;

            case 2:
                printf("exo 2\n");
                
                seuillageExtrema1(imageRGB,imageRGBOut,nW, nH);
                histo(imageRGB, nW,  nH);
                saveImage(makeFinalPath( folderOut, (char*)"_RGB_Seuillage__",inputName), nH,nW,imageRGBOut);
                expansion(imageRGBOut,imageRGBOut,nW, nH);
                histo2(imageRGBOut, nW,  nH);
                saveImage(makeFinalPath( folderOut, (char*)"_RGB_Expansion__",inputName), nH,nW,imageRGBOut);
            break;
            case 3:
                printf("exo 2\n");
                
                seuillageExtrema1(imageRGB,imageRGBOut,nW, nH);
                histo(imageRGB, nW,  nH);
                saveImage(makeFinalPath( folderOut, (char*)"_RGB_Seuillage__",inputName), nH,nW,imageRGBOut);
                expansion(imageRGBOut,imageRGBOut,nW, nH);
                histo2(imageRGBOut, nW,  nH);
                saveImage(makeFinalPath( folderOut, (char*)"_RGB_Expansion__",inputName), nH,nW,imageRGBOut);
            break;
        }

    }
    else
    {

        lire_nb_lignes_colonnes_image_pgm(pathIn, &nH, &nW);
       
        imageG.resize(nH);
        for(auto & line: imageG)
            line.resize(nW);
        imageOut.resize(nH);
        for(auto & line: imageOut)
            line.resize(nW);
        
        loadImage(pathIn,imageG,nW, nH);
        
        loadImage(pathIn2,imageOut,nW, nH);
        saveImage(makeFinalPath( folderOut, (char*)"_originale_",inputName), nH,nW,imageG);
         saveImage(makeFinalPath( folderOut, (char*)"_originale2_",inputName2), nH,nW,imageOut);
        
        switch(exo)
        {
            case 1:
                histo(imageG,nW,  nH);
                expansion(imageG,imageOut,nW, nH);
                saveImage(makeFinalPath( folderOut, (char*)"expansion",inputName), nH,nW,imageOut);
            break;
            case 2:
                printf("exo 3\n");
                
                egalisation2(imageG,imageOut,nW, nH);
                 saveImage(makeFinalPath( folderOut, (char*)"egalisation",inputName), nH,nW,imageG);
                saveImage(makeFinalPath( folderOut, (char*)"egalisation2",inputName2), nH,nW,imageOut);
            break;
        }

    }
    



    return 1;
}