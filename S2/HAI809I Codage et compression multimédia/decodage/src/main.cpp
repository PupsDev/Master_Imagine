#include <iostream>
#include <stdio.h>
#include "../src/functions.cpp"
#include <bitset>
#include <fstream>
#include <string>
#include "linalg.h"

using namespace std;


int stringTochar(std::string mybitset)
{
    int n = 0;

    for (int i = 0; i < mybitset.size(); i++) {
        n <<= 1;
        n |= mybitset[i] - '0';
    }

    return n;
}
void codageParPlage()
{
    int plage = 4; // 5 bits par plage 
    int i=0;
    vector<int> sequence;
    for(int i = 0 ; i < 11 ;i++)
    {
        for(int j=0;j<i;j++)
        {
            sequence.push_back(i);
        }
    }
    for(int i = 0 ; i < sequence.size() ;i++)
    {
        cout<<sequence[i]<<" ";
    }

    std::string codage = std::string("");

    while(i< sequence.size() )
    {

        int px = sequence[i];
        int counter = 0;
        int j=0;
        do
        {
            j++;
            counter++;
        }
        while(px == sequence[i+j] && counter<pow(2,4) );
        i+=counter;

        std::string binary = std::bitset<8>(px).to_string();
        std::string counterString = std::bitset<5>(counter).to_string();
        cout<<"\n"<<binary<<" "<<counterString<<endl;
        codage+=binary+counterString;
 
    }
    
    cout<<codage<<endl;
    // 8 -> 13
    FILE * fp = fopen("codage.mlc", "wb+");

    for(int i =0; i < codage.size();i+=8)
    {
        std::string mybitset= std::string("");
        for(int j = 0 ; j <8;j++)
            mybitset+=codage[i+j];
        //cout<<stringTochar(mybitset)<<endl;
        fputc(stringTochar(mybitset), fp);
    }

}
void decode(char* path, ImageG &image)
{
    FILE *fp = fopen(path,"rb+");
    char c;
    int plage = 5;
    unsigned long fileLen;
    unsigned long counter;

    fseek(fp, 0, SEEK_END);
    fileLen=ftell(fp);      // get the exact size of the pic
    fseek(fp, 0, SEEK_SET);
    std::string codage = std::string("");
     int size=0;
    for(counter=0; counter<fileLen; counter++)
    {
        c=getc(fp);
        codage+= std::bitset<8>(c).to_string();
         size++;
    }
    fclose(fp);
    cout<<"taille "<<size<<endl;
    int imageDecode[256*256];
    int nH=256,nW=256;
    int nTaille = nW*nH;
    OCTET *imgColor;
    allocation_tableau(imgColor, OCTET, nTaille);

    int k=0;    
    size = size*8;
    
    for(int i = 0 ; i< size-26;i+=(8+plage))
    {
        std::string valueS= std::string("");
        std::string counterS= std::string("");

        for(int j = 0 ; j <8;j++)
            valueS+=codage[i+j];
        
        for(int j = 8 ; j <(8+plage);j++)
            counterS+=codage[i+j];
       
        int value = stringTochar(valueS);
        int counter = stringTochar(counterS);
        
        //cout<<counter<<" ";

        for(int j = 0; j < counter ;j++)
        {

            imgColor[k +j]=value;
        }
        k+= max(1,counter);
    }

    cout<<"taille "<<k<<endl;
        
    
    ecrire_image_pgm((char*)"test.pgm", imgColor,  nH, nW);
    for (int i=0; i < nH; i++)
    {
        for (int j=0; j < nW ; j++)
        {
            image[i][j]=imgColor[i*nW+j];
            //cout<<image[i][j];

        }
    }
    free(imgColor);

}
double check(int px)
{
    return std::max( std::min(255.,px-128.),0.);
}
void inverseOndelette(ImageG &image, int q1, int q2)
{
    ImageG out;
    int nW,nH;
    nW=nH=image.size();
    resize(out,nW,nH);

    ImageG BF,MFV,MFH,HF;
    resize(BF,nW/2,nH/2);
    resize(MFV,nW/2,nH/2);
    resize(MFH,nW/2,nH/2);
    resize(HF,nW/2,nH/2);

    //std::cout<<nW<<nH<<std::endl;
    for (size_t i = 0; i < nH-1; i+=2)
	{
		for (size_t j = 0; j < nW-1; j+=2)
		{
			double X = image[(i / 2)][(j / 2)];
			double Y = q2*(image[(i / 2)][nW / 2 + (j / 2)]-128);
			double Z = q2*(image[nH / 2 + (i / 2)][(j / 2)]-128);
			double O = q1*(image[nH / 2 + (i / 2)][nW / 2 + (j / 2)]-128);

            //X= std::max( std::min(255.,X),0.);
            //Y= std::max( std::min(255.,Y),0.);
            //O= std::max( std::min(255.,O),0.);
            linalg::aliases::double4x4 a_matrix{ {0,1./8.,1./8.,0},
                                                {1./4.,0,0,-1./4.},
                                                {1./4.,-1./4.,0,0},
                                                {0,0,-1./2.,-1./2.},
                                                };
            linalg::aliases::double4 outVec = linalg::mul(transpose(a_matrix), linalg::aliases::double4(X,Y,Z,O));
            cout<<outVec[0]<<" "<<outVec[1]<<" "<<outVec[2]<<" "<<outVec[3]<<" "<<endl;

			double A = X;//double(2*X+Y+Z+O/2.);
			double B = X+outVec[1];//double(4*X+2*Y-A);
			double D = X+outVec[2];//2*((-X-Z)+A);
			double C = X+outVec[3];//-(4*X - A - B- D);
            //Y = 1./Q2 * Y;
            //Z = 1./Q2 * Z;
            //cout<<A<<" "<<B<<" "<<C<<" "<<D<<endl;
            out[i][j] =     std::max( std::min(255.,A),0.);
			out[i][j+1] =   std::max( std::min(255.,B),0.);
			out[i+1][j] =   std::max( std::min(255.,C),0.);
			out[i+1][j+1] = std::max( std::min(255.,D),0.);
            

		}
	}

    char * folderOut = (char*)""; 
    char * inputName = (char*)"image.pgm"; 
    saveImage(makeFinalPath( folderOut, (char*)"result_",inputName), nH,nW,out);
}
void save(int imageDecode[])
{
        OCTET *imgColor;
        int nH=256,nW=256;
        int nTaille = nW*nH;

        allocation_tableau(imgColor, OCTET, nTaille);
        

        for (int i=0; i < nH; i++)
            {
                for (int j=0; j < nW ; j++)
                {
                  imgColor[i *nW +j]=imageDecode[i *nW +j];


                }
            }
            free(imgColor);
}
int main(int argc, char* argv[]) {
    char inputName[250];
    int nH=256, nW=256;
   char * folderIn = (char*)"";
    char * folderOut = (char*)"out/"; 

    if (argc != 4) 
    {
        printf("Usage: ImageIn.ppm  q1 q2 \n"); 
        exit (1) ;
    }
    int q1,q2;
    sscanf (argv[1],"%s",inputName) ;
    sscanf (argv[2],"%d",&q1) ;
    sscanf (argv[3],"%d",&q2) ;
    char * pathIn = makePath(inputName,folderIn);
    ImageG image;
    resize(image,256,256);
    decode(inputName,image);
    inverseOndelette(image,q1,q2);
    saveImage(makeFinalPath( folderOut, (char*)"originale_",(char*)"output.pgm"), nH,nW,image);
    //codageParPlage();





    return 1;
}