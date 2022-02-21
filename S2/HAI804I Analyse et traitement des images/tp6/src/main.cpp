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
    
    //std::cout<<"ecart_type: "<<ecart_type1<<"ecart_type: "<<ecart_type2<<"ecart_type: "<<ecart_type3<<"ecart_type: "<<ecart_type4<<"\n";
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

void fill2(ImageG &image, int size, int value)
{
    for (size_t i = 0; i < size; i++)
	{
		for (size_t j = 0; j < size; j++)
		{
            if(i==0 || j==0 || i == size-1|| j== size-1)
                image[i][j]=255;    
            else
                image[i][j]=0;
        }
    }

}
double minMax(ImageG image,int startX,int endX, int startY, int endY)
{
    double mini = 255.;
    double maxi =0.;
    for (size_t i = startY; i < endY; i++)
	{
		for (size_t j = startX; j < endX; j++)
		{
            if(image[i][j]>maxi)
            {
                maxi = image[i][j];
            }
            if(image[i][j]<mini)
            {
                mini = image[i][j];
            }
        }
    }   
    return maxi-mini;
}
void Split(ImageG &image, int size, double seuil )
{
    int nW,nH;
   
    nW=nH=size;
    ImageG A,B,C,D;
    resize(A,nW/2,nH/2);
    resize(B,nW/2,nH/2);
    resize(C,nW/2,nH/2);
    resize(D,nW/2,nH/2);

    double cadran1 = moyenne(image,0,nW/2, 0, nH/2);
    double cadran2 = moyenne(image,nW/2,nW, 0, nH/2);
    double cadran3 = moyenne(image,0,nW/2, nH/2, nH);
    double cadran4 = moyenne(image,nW/2,nH, nH/2, nH);


/*
    double ecart_type1 = minMax(image,0,nH/2, 0, nW/2);
    double ecart_type2 = minMax(image,0,nH/2, nW/2, nW);
    double ecart_type3 = minMax(image,nH/2,nH, 0, nW/2);
    double ecart_type4 = minMax(image,nH/2,nH, nW/2, nW);
    
*/
    double ecart_type1 = sqrt(ecart_type(image,0,nW/2, 0, nH/2,cadran1));
    double ecart_type2 = sqrt(ecart_type(image,nW/2,nW, 0, nH/2,cadran2));
    double ecart_type3 = sqrt(ecart_type(image,0,nW/2, nH/2, nH,cadran3));
    double ecart_type4 = sqrt(ecart_type(image,nW/2,nH, nH/2, nH,cadran4));

    //std::cout<<"\nSize :"<<size<<"\n";
    //std::cout<<"\necart_type: "<<ecart_type1<<"ecart_type: "<<ecart_type2<<"ecart_type: "<<ecart_type3<<"ecart_type: "<<ecart_type4<<"\n";

    for (size_t i = 0; i < nH; i++)
    {
        for (size_t j = 0; j < nW; j++)
        {

            A[(i / 2)][(j / 2)]=image[(i / 2)][(j / 2)]                   ;                   
            B[(i / 2)][(j / 2)]=image[(i / 2)][nW / 2 + (j / 2)]          ;          
            C[(i / 2)][(j / 2)]=image[nH / 2 + (i / 2)][(j / 2)]          ;          
            D[(i / 2)][(j / 2)]=image[nH / 2 + (i / 2)][nW / 2 + (j / 2)] ; 
        

        }
    }
    if(size>=8)
    {
        
        if(ecart_type1>=seuil)
            Split(A,  size/2,seuil);
        else
            fill(A,size/2,cadran1);

        if(ecart_type2>=seuil)    
            Split(B,  size/2,seuil);
        else
            fill(B,size/2,cadran2);

        if(ecart_type3>=seuil)
            Split(C,  size/2,seuil);
        else
            fill(C,size/2,cadran3);

        if(ecart_type4>=seuil)
            Split(D,  size/2,seuil);
        else
            fill(D,size/2,cadran4);
        
        for (size_t i = 0; i < nH; i++)
        {
            for (size_t j = 0; j < nW; j++)
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


        for (size_t i = 0; i < nH; i++)
        {
            for (size_t j = 0; j < nW; j++)
            {
                image[(i / 2)][(j / 2)]                   =cadran1;                                                                           
                image[(i / 2)][nW / 2 + (j / 2)]          =cadran2;                                       
                image[nH / 2 + (i / 2)][(j / 2)]          =cadran3;       			                              
                image[nH / 2 + (i / 2)][nW / 2 + (j / 2)] =cadran4;            

            }
        }
        
    }

        
}
void testSplit(ImageG &image, int size,double seuil )
{
    int nW,nH;
    
    nW=nH=size;
    ImageG A,B,C,D;
    resize(A,nW/2,nH/2);
    resize(B,nW/2,nH/2);
    resize(C,nW/2,nH/2);
    resize(D,nW/2,nH/2);


    double cadran1 = moyenne(image,0,nW/2, 0, nH/2);
    double cadran2 = moyenne(image,nW/2,nW, 0, nH/2);
    double cadran3 = moyenne(image,0,nW/2, nH/2, nH);
    double cadran4 = moyenne(image,nW/2,nH, nH/2, nH);


/*
    double ecart_type1 = minMax(image,0,nH/2, 0, nW/2);
    double ecart_type2 = minMax(image,0,nH/2, nW/2, nW);
    double ecart_type3 = minMax(image,nH/2,nH, 0, nW/2);
    double ecart_type4 = minMax(image,nH/2,nH, nW/2, nW);
    
*/
    double ecart_type1 = sqrt(ecart_type(image,0,nW/2, 0, nH/2,cadran1));
    double ecart_type2 = sqrt(ecart_type(image,nW/2,nW, 0, nH/2,cadran2));
    double ecart_type3 = sqrt(ecart_type(image,0,nW/2, nH/2, nH,cadran3));
    double ecart_type4 = sqrt(ecart_type(image,nW/2,nH, nH/2, nH,cadran4));

    for (size_t i = 0; i < nH; i++)
    {
        for (size_t j = 0; j < nW; j++)
        {

            A[(i / 2)][(j / 2)]=image[(i / 2)][(j / 2)]                   ;                   
            B[(i / 2)][(j / 2)]=image[(i / 2)][nW / 2 + (j / 2)]          ;          
            C[(i / 2)][(j / 2)]=image[nH / 2 + (i / 2)][(j / 2)]          ;          
            D[(i / 2)][(j / 2)]=image[nH / 2 + (i / 2)][nW / 2 + (j / 2)] ; 
        

        }
    }
    if(size>=8)
    {
        
        if(ecart_type1>=seuil)
            testSplit(A,  size/2,seuil);
        else
            fill2(A,size/2,cadran1);

        if(ecart_type2>=seuil)    
            testSplit(B,  size/2,seuil);
        else
            fill2(B,size/2,cadran2);

        if(ecart_type3>=seuil)
            testSplit(C,  size/2,seuil);
        else
            fill2(C,size/2,cadran3);

        if(ecart_type4>=seuil)
            testSplit(D,  size/2,seuil);
        else
            fill2(D,size/2,cadran4);

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
        fill2(image,size,cadran4);

    }

        
}
void myFill(ImageG &image, int startI, int startJ, int endI, int endJ)
{
    for (size_t i = startI; i < endI; i++)
	{
		for (size_t j = startJ; j < endJ; j++)
		{
            if(i==startI || j==startJ || i == endI-1|| j== endJ-1)
                image[i][j]=255;    
            else
                image[i][j]=0;
        }
    }

}
void myFill2(ImageG &image, int startI, int startJ, int endI, int endJ,int value)
{
    for (size_t i = startI; i < endI; i++)
	{
		for (size_t j = startJ; j < endJ; j++)
		{
            image[i][j]=value;
        }
    }

}
void testSplit2(ImageG &image,ImageG &out, int startI, int startJ, int endI, int endJ, int size)
{
    double treshold =5.;
    double moy = moyenne(image,startJ,endJ,startI,endI);
    double var = sqrt(ecart_type(image, startI, endI, startI,  endJ, moy));

     //std::cout<<"var: "<<var<<"\n";
    int h = endI - startI;
    int w = endJ - startJ;
//    std::cout<<"h: "<<h<<"w: "<<w<<"\n";
    //std::cout<<"SIZE:"<<size<<"\n";
    //std::cout<<"v: "<<startI<<"v: "<<startJ<<"v: "<<endI<<"v: "<<endJ<<"\n";
    

    double cadran1 = moyenne(image,0,size/2, 0, size/2);
    double cadran2 = moyenne(image,0,size/2, size/2, size);

    double cadran3 = moyenne(image,size/2,size, 0, size/2);
    double cadran4 = moyenne(image,size/2,size, size/2, size);

    double ecart_type1 = sqrt(ecart_type(image,0,size/2, 0, size/2,cadran1));
    double ecart_type2 = sqrt(ecart_type(image,0,size/2, size/2, size,cadran2));
      
    double ecart_type3 = sqrt(ecart_type(image,size/2,size, 0, size/2,cadran3));
    double ecart_type4 = sqrt(ecart_type(image,size/2,size, size/2, size,cadran4));
    size = size/2;
    
    if(var>treshold && size>8)
    {
        
        //std::cout<<"A\n";
        //if(ecart_type1>treshold)
            testSplit2(image,out,  startI, startJ,  startI+size,   startJ+size,  size);
        //std::cout<<"B\n";
        //if(ecart_type2>treshold)
            testSplit2(image,out,  startI, startJ+size,  startI+size,   startJ+2*size,  size);
        //std::cout<<"C\n";
        //if(ecart_type3>treshold)
            testSplit2(image,out,  startI+size, startJ,  startI+2*size,   startJ+size,  size);
        //std::cout<<"A\n";
        //if(ecart_type4>treshold)
            testSplit2(image,out,  startI+size, startJ+size,  startI+2*size,   startJ+2*size,  size);
    }
    else
    {
       
        myFill2(out, startI, startJ, endI, endJ,moy);
    
    }

}

void exo3(ImageG greyscale, int nW, double seuil)
{
    char * folderOut = (char*)"out/"; 
        testSplit(greyscale, nW,seuil);
        std::string output = std::string("image")+std::string(std::to_string((int)seuil))+std::string(".pgm");
        saveImage( folderOut, (char*)"testSplit",&output[0],greyscale);
}
void exo4(ImageG out, int nW, double seuil)
{
    char * folderOut = (char*)"out/"; 
   
        Split(out, nW,seuil);
        std::string output = std::string("image")+std::string(std::to_string((int)seuil))+std::string(".pgm");
        saveImage( folderOut, (char*)"Split",&output[0],out);
}

std::vector<std::vector<int>> rag;
int distance(int x,int y, int x1, int y1)
{
    return abs(x-x1) +abs(y-y1);
}
class region
{
    public:
        region(
            int indice,
            ImageG bitboard,
            int size,
            int sx,
            int sy,
            double moyenne
                    )
        {
            this->indice=indice;
            this->bitboard=bitboard;
            this->size=size; 
            this->sx=sx;
            this->sy=sy;
            this->moyenne=moyenne;

        }
        int indice;
        ImageG bitboard;
        int size; 
        int sx;
        int sy;
        double moyenne;

        std::vector<region*> children;


};
static int regionIndice=0;
void SplitGraph(ImageG &image,region* root, int size, double seuil, int sx, int sy)
{
    int nW,nH;
   
    nW=nH=size;
    ImageG A,B,C,D;
    resize(A,nW/2,nH/2);
    resize(B,nW/2,nH/2);
    resize(C,nW/2,nH/2);
    resize(D,nW/2,nH/2);

    double cadran1 = moyenne(image,0,nW/2, 0, nH/2);
    double cadran2 = moyenne(image,nW/2,nW, 0, nH/2);
    double cadran3 = moyenne(image,0,nW/2, nH/2, nH);
    double cadran4 = moyenne(image,nW/2,nH, nH/2, nH);

    double ecart_type1 = sqrt(ecart_type(image,0,nW/2, 0, nH/2,cadran1));
    double ecart_type2 = sqrt(ecart_type(image,nW/2,nW, 0, nH/2,cadran2));
    double ecart_type3 = sqrt(ecart_type(image,0,nW/2, nH/2, nH,cadran3));
    double ecart_type4 = sqrt(ecart_type(image,nW/2,nH, nH/2, nH,cadran4));

    //std::cout<<"start:"<<sx<<" "<<sy<<"\n";

    ImageG A2,B2,C2,D2;
    resize(A2,512,512);
    resize(B2,512,512);
    resize(C2,512,512);
    resize(D2,512,512);

    for (size_t i = 0; i < nH; i++)
    {
        for (size_t j = 0; j < nW; j++)
        {

            A2[(i )][(j )]=0;                   
            B2[(i )][(j )]=0;          
            C2[(i )][(j )]=0;          
            D2[(i )][(j )]=0; 


        }
    }
        for (size_t i = 0; i < nH; i++)
    {
        for (size_t j = 0; j < nW; j++)
        {

            A[(i / 2)][(j / 2)]=image[(i / 2)][(j / 2)]                   ;                   
            B[(i / 2)][(j / 2)]=image[(i / 2)][nW / 2 + (j / 2)]          ;          
            C[(i / 2)][(j / 2)]=image[nH / 2 + (i / 2)][(j / 2)]          ;          
            D[(i / 2)][(j / 2)]=image[nH / 2 + (i / 2)][nW / 2 + (j / 2)] ; 
            
            A2[sy+(i / 2)][sx+(j / 2)]                    = 255;                 
            B2[sy+(i / 2)][sx+nW / 2 + (j / 2)]           = 255;    
            C2[sy+nH / 2 + (i / 2)][sx+(j / 2)]           = 255;        
            D2[sy+nH / 2 + (i / 2)][sx+nW / 2 + (j / 2)]  = 255; 
        

        }
    }

    region *rA = new region(++regionIndice,A2,size/2,sx,sy,cadran1);
    region *rB = new region(++regionIndice,B2,size/2,sx,sy,cadran2);
    region *rC = new region(++regionIndice,C2,size/2,sx,sy,cadran3);
    region *rD = new region(++regionIndice,D2,size/2,sx,sy,cadran4);

    root->children.push_back(rA);
    root->children.push_back(rB);
    root->children.push_back(rC);
    root->children.push_back(rD);

    if(size>=256)
    {
        
        if(ecart_type1>=seuil)
            SplitGraph(A,rA, size/2,seuil,sx,sy);
        else
            fill(A,size/2,cadran1);

        if(ecart_type2>=seuil)    
            SplitGraph(B,rB, size/2,seuil,sx+size/2,sy);
        else
            fill(B,size/2,cadran2);

        if(ecart_type3>=seuil)
            SplitGraph(C,rC, size/2,seuil,sx,sy+size/2);
        else
            fill(C,size/2,cadran3);

        if(ecart_type4>=seuil)
            SplitGraph(D,rD, size/2,seuil,sx+size/2,sy+size/2);
        else
            fill(D,size/2,cadran4);
        
        for (size_t i = 0; i < nH; i++)
        {
            for (size_t j = 0; j < nW; j++)
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


        for (size_t i = 0; i < nH; i++)
        {
            for (size_t j = 0; j < nW; j++)
            {
                image[(i / 2)][(j / 2)]                   =cadran1;                                                                           
                image[(i / 2)][nW / 2 + (j / 2)]          =cadran2;                                       
                image[nH / 2 + (i / 2)][(j / 2)]          =cadran3;       			                              
                image[nH / 2 + (i / 2)][nW / 2 + (j / 2)] =cadran4;            

            }
        }
        
    }

        
}
void make_list(region*root,std::vector<region*> &liste)
{
        for(auto &region : root->children)
        {
            liste.push_back(region);
            
        }
       
        for(auto &region : root->children)
        {
            make_list(region,liste);
            
        }
    
}
void printMiniatures(region * root)
{
    char * folderOut = (char*)"out/"; 
    for(auto &region : root->children)
    {
        std::string output = std::string("imageChildren")+std::string(std::to_string(region->indice))+std::string(".pgm");
        saveImage( folderOut, (char*)"Split",&output[0],region->bitboard);
        printMiniatures(region);
    }
}
bool computeCloseness(region* r1,region* r2,int size)
{
    bool test =false;
    for(int i =1;i<size-1;i++)
        for(int j=1;j<size-1;j++)
        {
            
            if(r1->bitboard[i][j]==255)
            {
                if(r2->bitboard[i+1][j] ==255)
                {
                    test=true;
                    break;
                }
                else if(r2->bitboard[i][j+1] ==255)
                {
                    test=true;
                    break;
                }
                else if(r2->bitboard[i-1][j] ==255)
                {
                    test=true;
                    break;
                }
                else if(r2->bitboard[i][j-1] ==255)
                {
                    test=true;
                    break;
                }
                
            }
        }
        return test;

}
void mergeRegion(region* &r1,region* &r2,int size, ImageG &image)
{
            double res=0.;
        double c=0.;
    for(int i =0;i<size;i++)
        for(int j=0;j<size;j++)
        {
                if(r2->bitboard[i][j]==255)
                {  
                    r1->bitboard[i][j]+=r2->bitboard[i][j];
                }
                if(r1->bitboard[i][j]==255)
                {
                    res+=image[i][j];
                    c++;
                }
           
        }  
        r1->moyenne=res/c;
        for (size_t i = 0; i < size; i++)
        {
            for (size_t j = 0; j < size; j++)
            {
                if(r1->bitboard[i][j]==255)
                {
                    image[i][j]=r1->moyenne;

                }
            }
        } 
}
bool tryto(Image& image,int size)
{
    bool test =false;
    for(int i =1;i<size-1;i++)
        for(int j=1;j<size-1;j++)
        {
            
            if(r1->bitboard[i][j]==255)
            {
                if(r2->bitboard[i+1][j] ==255)
                {
                    test=true;
                    break;
                }
                else if(r2->bitboard[i][j+1] ==255)
                {
                    test=true;
                    break;
                }
                else if(r2->bitboard[i-1][j] ==255)
                {
                    test=true;
                    break;
                }
                else if(r2->bitboard[i][j-1] ==255)
                {
                    test=true;
                    break;
                }
                
            }
        }
        return test;

}
int main(int argc, char* argv[]) {
    char inputName[250];
    int nH, nW;
   char * folderIn = (char*)"../../res/";
    char * folderOut = (char*)"out/"; 
    double seuil;

    if (argc != 3) 
    {
        printf("Usage: ImageIn.ppm seuil  \n"); 
        exit (1) ;
    }

    sscanf (argv[1],"%s",inputName) ;
    sscanf (argv[2],"%lf",&seuil) ;

    char * pathIn = makePath(inputName,folderIn);
    int color = ppmOrPgm(inputName);
    ImageRGB imageRGB;
    ImageG greyscale;
    ImageG out;

    if(color)
    {
        
        lire_nb_lignes_colonnes_image_ppm(pathIn, &nH, &nW);
        resize(imageRGB,nW,nH);
        resize(greyscale,nW,nH);
        resize(out,nW,nH);
        loadImage(pathIn,imageRGB,nW, nH);
        saveImage( folderOut, (char*)"originale_",inputName,imageRGB);

        convertToGreyscaleLuminance(imageRGB,greyscale);
        convertToGreyscaleLuminance(imageRGB,out);
        saveImage( folderOut, (char*)"originale_",(char*)"greyscale.pgm",greyscale);

        //exo1(greyscale,512);
        //saveImage( folderOut, (char*)"divide1_",(char*)"greyscale.pgm",greyscale);
        
        exo3( greyscale,  nW,  seuil);
        
        //exo4( out,  nW,  seuil);
        
        
        region *root = new region(regionIndice,greyscale,nW,0,0,0);
        
        SplitGraph(out,root, nW,seuil,0,0);
        //saveImage( folderOut, (char*)"divide1_",(char*)"greyscale.pgm",greyscale);
        //std::string output = std::string("imageSpliut")+std::string(std::to_string((int)seuil))+std::string(".pgm");
        //saveImage( folderOut, (char*)"Split",&output[0],out);
        /*
        printMiniatures(root);
        */
        std::string output = std::string("imageSpliutBefore")+std::string(std::to_string((int)seuil))+std::string(".pgm");
        saveImage( folderOut, (char*)"Split",&output[0],out);
       std::vector<region*> listeRegion;
       /* make_list(root,listeRegion);
        //int sizeListe =
        for(int i =0; i<listeRegion.size();i++)
        {

            for(int j =0;j<listeRegion.size();j++)
            {
                if(i!=j)
                {
                    if( abs(listeRegion[i]->moyenne - listeRegion[j]->moyenne) < 50)
                    {
                        if(computeCloseness(listeRegion[i],listeRegion[j],nW))
                        {
                            //std::cout<<"lerge\n";
                            mergeRegion(listeRegion[i],listeRegion[j],nW, out);
                            listeRegion.erase(listeRegion.begin() +j);
                        }
                    }

                }


            }
        }*/
        std::string output2 = std::string("imageSpliut")+std::string(std::to_string((int)seuil))+std::string(".pgm");
        saveImage( folderOut, (char*)"Split",&output2[0],out);
        

    }
    else
    {
        lire_nb_lignes_colonnes_image_pgm(pathIn, &nH, &nW);
        resize(greyscale,nW,nH);
        resize(out,nW,nH);
        loadImage(pathIn,greyscale,nW, nH);
        //testSplit(greyscale, nW);
        //testSplit2(greyscale,out, 0,0, nW, nW, nW);
        //testSplit(greyscale, nW);
        saveImage( folderOut, (char*)"divideN_",inputName,greyscale);
        saveImage( folderOut, (char*)"OUT_",inputName,out);



    }

    



    return 1;
}