#include <iostream>
#include <stdio.h>
#include "image_ppm.h"
#include <stdlib.h>
#include <vector>
#include <cassert>
#include <math.h>
#include <bitset>
#include <fstream>
#include <string>

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
char *makeFinalPath2( char *folderOut, char *specificName,char* inputName)
{
    return (char*)makePath((char*)"sard.pgm",makePath(specificName,folderOut));
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
typedef std::vector<std::vector<couleur>> ImageRGB;
typedef std::vector<std::vector<int>> ImageG;
void RGBtolum(ImageRGB image, ImageG &luminance,ImageG &Cr,ImageG &Cb, int nW,int nH)
{

      //LUMINANCE
        for (int i=0; i < nH; i++)
        {
            for (int j=0; j < nW; j++)
            { 
                luminance[i][j]= 0.299* image[i][j].r +0.587*image[i][j].g+0.114*image[i][j].b;

            }

        }
        //CHROMINANCE
        for (int i=0; i < nH; i++)
        {
            for (int j=0; j < nW; j++)
            { 
                Cr[i][j]=  (image[i][j].r-luminance[i][j])/1.402;
                Cb[i][j]=  (image[i][j].b-luminance[i][j])/1.772;
                

            }

        }
}
void resize(ImageG& image, int nW,int nH)
{
    image.resize(nH);
    for(auto & line: image)
        line.resize(nW);
}
void ondelette1D(ImageG &data, int nW,int nH, double Q1, double Q2)
{
    ImageG out;
    resize(out,nW,nH);

    ImageG BF,MFV,MFH,HF;
    resize(BF,nW/2,nH/2);
    resize(MFV,nW/2,nH/2);
    resize(MFH,nW/2,nH/2);
    resize(HF,nW/2,nH/2);

    std::cout<<nW<<nH<<std::endl;
    for (size_t i = 0; i < nH-1; i++)
	{
		for (size_t j = 0; j < nW-1; j++)
		{
			double A = data[i][j];
			double B = data[i][j+1];
			double C = data[i+1][j];
			double D = data[i+1][j+1];

			double X = double(A + B + C + D) / 4;
			double Y = double(A + B - C - D) / 2;
			double Z = double(A - B + C - D) / 2;
			double O = double(A - B - C + D);

            O = 1./Q1 * O;
            Y = 1./Q2 * Y;
            Z = 1./Q2 * Z;

            out[(i / 2)][(j / 2)] =                     std::max( std::min(255.,X),0.);
			out[(i / 2)][nW / 2 + (j / 2)] =            std::max( std::min(255.,Y+128.),0.);
			out[nH / 2 + (i / 2)][(j / 2)] =            std::max( std::min(255.,Z+128.),0.);
			out[nH / 2 + (i / 2)][nW / 2 + (j / 2)] =   std::max( std::min(255.,O+128.),0.);
            
            BF[i/2][j/2]= std::max( std::min(255.,X),0.);
            MFV[i/2][j/2]=std::max( std::min(255.,Y+128.),0.);
            MFH[i/2][j/2]=std::max( std::min(255.,Z+128.),0.);
            HF [i/2][j/2]=std::max( std::min(255.,O+128.),0.);

		}
	}
        for (size_t i = 0; i < nH; i++)
	{
		for (size_t j = 0; j < nW; j++)
		{
            data[i][j]=out[i][j];
        }
    }
    char * folderOut = (char*)"out/"; 
    char * inputName = (char*)"lena256.pgm"; 
    saveImage(makeFinalPath( folderOut, (char*)"BF",inputName), nH/2,nW/2,BF);
    saveImage(makeFinalPath( folderOut, (char*)"MFV_",inputName), nH/2,nW/2,MFV);
    saveImage(makeFinalPath( folderOut, (char*)"MFH_",inputName), nH/2,nW/2,MFH);
    saveImage(makeFinalPath( folderOut, (char*)"HF_",inputName), nH/2,nW/2,HF);
}

/*
void ondelette(ImageG &luminance,ImageG &Cr,ImageG &Cb, int nW,int nH)
{
    ondelette1D(luminance,nW,nH);
    ondelette1D(Cr,nW,nH);
    ondelette1D(Cb,nW,nH);
    int N = 5;
    for(int i = 0 ; i < N-1; i++)
    {
        nW/=2;
        nH/=2;
        ondelette1D(luminance,nW,nH);
        ondelette1D(Cr,nW,nH);
        ondelette1D(Cb,nW,nH);
    }




}*/
void ondelette2(ImageG &image, int nW,int nH, double Q1, double Q2)
{
    ondelette1D(image,nW,nH,Q1, Q2);
    /*
    int N = 1;
    for(int i = 0 ; i < N-1; i++)
    {

        ondelette1D(image,nW,nH,  Q1, Q2);
        nW/=2;
        nH/=2;
    }
    */
}
/*
void write(ostream& str, const bitset& bs)
{
    while( bits_remaining_in_bs )
    {
        int bitstoextract = 8 - bitswritten;

        bitbuffer |= extract_bits_from_bitset( bs, bitstoextract );
        
        if(bitswritten == 8)
        {
            str.write(&bitbuffer,1);
            bitswritten = 0;
            bitbuffer = 0;
        }
    }
}
*/
int stringTochar(std::string mybitset)
{
    int n = 0;

    for (int i = 0; i < mybitset.size(); i++) {
        n <<= 1;
        n |= mybitset[i] - '0';
    }

    return n;
}
int write_bit(unsigned char * byte, int *pos, FILE* fp, unsigned char e)
{
    int written;
    if(*pos==8) //si les 8bits sont remplis
    {
        written = fputc(*byte, fp); //on écrit l'octet dans le fichier
        if(written==EOF)
            return -1;
        *byte=0; //on initialise ensuite la pos et le buffer
        *pos=0;
      }

    if( e =='1') // si c'est un 1
    {
        *byte |= 0x1 <<(7-(*pos)); //on réalise un "ou" binaire qui va assigner le décalage de 7 - position bits
                                    // pour le buffer
    }
    if(e=='0'); //si c'est 0 on ne change rien, on part de 0000 0000 pour travailler donc les 0 ne changent pas
    (*pos)++;
    if(*pos >8) // cas où on dépasserait, cela n'arrive jamais en théorie
    {
        printf("\n DEPASSEMENT \n");
        *pos=0;
    }
}
void codageParPlage(ImageG &image,char * out ,int nW,int nH)
{
    int plage = 5; // 5 bits par plage 
    int i=0;
    std::string codage = std::string("");
    while(i< (nH*nH)-1 )
    {
        int px = image[i/nW][i%nW];
        int counter = 1;
        do
        {
            i++;
            counter++;
        }
        while(px == image[i/nW][i%nW] && counter<pow(2,5) );

        std::string binary = std::bitset<8>(px).to_string();
        std::string counterString = std::bitset<5>(counter).to_string();
       
        codage+=binary+counterString;
        i++;
    }
    // 8 -> 13
    FILE * fp = fopen("codage.mlc", "wb+");

    for(int i =0; i < codage.size()/8;i+=8)
    {
        std::string mybitset= std::string("");
        for(int j = 0 ; j <8;j++)
            mybitset+=codage[i+j];
        fputc(stringTochar(mybitset)+'0', fp);
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
    /*
    ImageRGB imageRGB;

    ImageG luminance;
    ImageG Cr;
    ImageG Cb;

        
    lire_nb_lignes_colonnes_image_ppm(pathIn, &nH, &nW);
    imageRGB.resize(nH);
    for(auto & line: imageRGB)
        line.resize(nW);

    resize(luminance,nW,nH);
    resize(Cr,nW,nH);
    resize(Cb,nW,nH);


    loadImage(pathIn,imageRGB,nW, nH);
    RGBtolum(imageRGB,luminance,Cr,Cb,nW,nH);
    saveImage(makeFinalPath( folderOut, (char*)"originale_",inputName), nH,nW,imageRGB);

    saveImage(makeFinalPath2( folderOut, (char*)"luminance_",inputName), nH,nW,luminance);
    saveImage(makeFinalPath2( folderOut, (char*)"Cr_",inputName), nH,nW,Cr);
    saveImage(makeFinalPath2( folderOut, (char*)"Cb",inputName), nH,nW,Cb);
    ondelette(luminance,Cr,Cb,nW,nH);
    saveImage(makeFinalPath2( folderOut, (char*)"luminance_2",inputName), nH,nW,luminance);
    saveImage(makeFinalPath2( folderOut, (char*)"Cr_2",inputName), nH,nW,Cr);
    saveImage(makeFinalPath2( folderOut, (char*)"Cb_2",inputName), nH,nW,Cb);
    */
    ImageG image;
    double Q1 = 20.,Q2=20.;
    lire_nb_lignes_colonnes_image_pgm(pathIn, &nH, &nW);
    resize(image,nW,nH);
    loadImage(pathIn,image,nW, nH);
    ondelette2(image,nW,nH,Q1,Q2);
    codageParPlage(image,(char*)"out",nW,nH);
    saveImage(makeFinalPath( folderOut, (char*)"originale_",inputName), nH,nW,image);




    return 1;
}