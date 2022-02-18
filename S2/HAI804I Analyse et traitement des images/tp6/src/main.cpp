#include <iostream>
#include <stdio.h>
#include "../src/functions.cpp"


double moyenne(ImageG image,int startX,int endX, int startY, int endY)
{
    double res=0.;
    double c=0.;
    for (size_t i = startY; i < endY; i++)
	{
		for (size_t j = startX; j < endX; j++)
		{
            res+=image[i][j];
            c++;
        }
    }   
    return res/c;
}

double ecart_type(ImageG image,int startX,int endX, int startY, int endY, double moyenne)
{
    double res=0.;
    double c=0.;
    for (size_t i = startY; i < endY; i++)
	{
		for (size_t j = startX; j < endX; j++)
		{
            res+= (moyenne - image[i][j])*(moyenne - image[i][j]);
            c++;
        }
    }   
    return res/c;
}
void exo1(ImageG &image, int size)
{
    ImageG out;
    int nW,nH;
    nW=nH=size;
    resize(out,nW,nH);

    double cadran1 = moyenne(image,0,nH/2, 0, nW/2);
    double cadran2 = moyenne(image,0,nH/2, nW/2, nW);

    double cadran3 = moyenne(image,nH/2,nH, 0, nW/2);
    double cadran4 = moyenne(image,nH/2,nH, nW/2, nW);
    double ecart_type1 = sqrt(ecart_type(image,0,nH/2, 0, nW/2,cadran1));
    double ecart_type2 = sqrt(ecart_type(image,0,nH/2, nW/2, nW,cadran2));
    double ecart_type3 = sqrt(ecart_type(image,nH/2,nH, 0, nW/2,cadran3));
    double ecart_type4 = sqrt(ecart_type(image,nH/2,nH, nW/2, nW,cadran4));
    
    std::cout<<"ecart_type: "<<ecart_type1<<"ecart_type: "<<ecart_type2<<"ecart_type: "<<ecart_type3<<"ecart_type: "<<ecart_type4<<"\n";
    for (size_t i = 0; i < nH-1; i++)
	{
		for (size_t j = 0; j < nW-1; j++)
		{
			double A = image[i][j];

            out[(i / 2)][(j / 2)]                   = (int)cadran1;                    
            out[(i / 2)][nW / 2 + (j / 2)]          = (int)cadran2;           
			out[nH / 2 + (i / 2)][(j / 2)]          = (int)cadran3;           
			out[nH / 2 + (i / 2)][nW / 2 + (j / 2)] = (int)cadran4;  
        

		}
	}
        for (size_t i = 0; i < nH; i++)
	{
		for (size_t j = 0; j < nW; j++)
		{
            image[i][j]=out[i][j];
        }
    }
}
void fill(ImageG &image, int size, int value)
{
    for (size_t i = 0; i < size; i++)
	{
		for (size_t j = 0; j < size; j++)
		{
            image[i][j]=value;
        }
    }

}
void exo2(ImageG &image, int size)
{
    int nW,nH;
    double seuil =2.;
    nW=nH=size;
    ImageG A,B,C,D;
    resize(A,nW/2,nH/2);
    resize(B,nW/2,nH/2);
    resize(C,nW/2,nH/2);
    resize(D,nW/2,nH/2);

    double cadran1 = moyenne(image,0,nH/2, 0, nW/2);
    double cadran2 = moyenne(image,0,nH/2, nW/2, nW);

    double cadran3 = moyenne(image,nH/2,nH, 0, nW/2);
    double cadran4 = moyenne(image,nH/2,nH, nW/2, nW);

    double ecart_type1 = sqrt(ecart_type(image,0,nH/2, 0, nW/2,cadran1));
    double ecart_type2 = sqrt(ecart_type(image,0,nH/2, nW/2, nW,cadran2));
      
    double ecart_type3 = sqrt(ecart_type(image,nH/2,nH, 0, nW/2,cadran3));
    double ecart_type4 = sqrt(ecart_type(image,nH/2,nH, nW/2, nW,cadran4));

    std::cout<<"\nSize :"<<size<<"\n";
    //std::cout<<"\necart_type: "<<ecart_type1<<"ecart_type: "<<ecart_type2<<"ecart_type: "<<ecart_type3<<"ecart_type: "<<ecart_type4<<"\n";

    for (size_t i = 0; i < nH-1; i++)
    {
        for (size_t j = 0; j < nW-1; j++)
        {

            A[(i / 2)][(j / 2)]=image[(i / 2)][(j / 2)]                   ;                   
            B[(i / 2)][(j / 2)]=image[(i / 2)][nW / 2 + (j / 2)]          ;          
            C[(i / 2)][(j / 2)]=image[nH / 2 + (i / 2)][(j / 2)]          ;          
            D[(i / 2)][(j / 2)]=image[nH / 2 + (i / 2)][nW / 2 + (j / 2)] ; 
        

        }
    }
    if(size>=4)
    {

        if(ecart_type1>=seuil)
            exo2(A,  size/2);
        else
            fill(A,size/2,cadran1);

        if(ecart_type2>=seuil)    
            exo2(B,  size/2);
        else
            fill(B,size/2,cadran2);

        if(ecart_type3>=seuil)
            exo2(C,  size/2);
        else
            fill(C,size/2,cadran3);

        if(ecart_type4>=seuil)
            exo2(D,  size/2);
        else
            fill(D,size/2,cadran4);
        
        for (size_t i = 0; i < nH-1; i++)
        {
            for (size_t j = 0; j < nW-1; j++)
            {
                image[(i / 2)][(j / 2)]                   =A[(i / 2)][(j / 2)];                                                                           
                image[(i / 2)][nW / 2 + (j / 2)]          =B[(i / 2)][(j / 2)];                                       
                image[nH / 2 + (i / 2)][(j / 2)]          =C[(i / 2)][(j / 2)];       			                              
                image[nH / 2 + (i / 2)][nW / 2 + (j / 2)] =D[(i / 2)][(j / 2)];            

            }
        }

    }
    else
    {
        std::cout<<"\n moyenne: "<<cadran1<<"cadran: "<<cadran2<<"cadran: "<<cadran3<<"cadran: "<<cadran4<<"\n";
        for (size_t i = 0; i < nH-1; i++)
        {
            for (size_t j = 0; j < nW-1; j++)
            {
                image[(i / 2)][(j / 2)]                   =cadran1;                                                                           
                image[(i / 2)][nW / 2 + (j / 2)]          =cadran2;                                       
                image[nH / 2 + (i / 2)][(j / 2)]          =cadran3;       			                              
                image[nH / 2 + (i / 2)][nW / 2 + (j / 2)] =cadran4;            

            }
        }
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
    ImageG greyscale;

    if(color)
    {
        
        lire_nb_lignes_colonnes_image_ppm(pathIn, &nH, &nW);
        resize(imageRGB,nW,nH);
        resize(greyscale,nW,nH);
        loadImage(pathIn,imageRGB,nW, nH);
        saveImage( folderOut, (char*)"originale_",inputName,imageRGB);

        convertToGreyscaleLuminance(imageRGB,greyscale);
        saveImage( folderOut, (char*)"originale_",(char*)"greyscale.pgm",greyscale);

        //exo1(greyscale,512);
        //saveImage( folderOut, (char*)"divide1_",(char*)"greyscale.pgm",greyscale);

        exo2(greyscale, 256);
        saveImage( folderOut, (char*)"divideN_",(char*)"greyscale.pgm",greyscale);



    }

    



    return 1;
}