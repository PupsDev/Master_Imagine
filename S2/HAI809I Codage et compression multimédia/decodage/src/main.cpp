#include <iostream>
#include <stdio.h>
#include "../src/functions.cpp"
#include <bitset>
#include <fstream>
#include <string>

using namespace std;
void decode(char* path)
{
    FILE *fp = fopen(path,"rb+");
    char c;
    int plage = 5;
    std::string codage = std::string("");
    do
    {
        c = fgetc(fp);
        codage+= (char)(c);
    }while(c!= EOF);
    //cout<<codage<<endl;
    for(int i = 0 ; i< codage.size();i+=(8+plage))
    {
        std::string value= std::string("");
        std::string counter= std::string("");

        for(int j = 0 ; j <8;j++)
            value+=codage[i+j];
        
        for(int j = 8 ; j <(8+plage);j++)
            counter+=codage[i+j];
        
        cout<<value;
        //std::bitset<8> valueB(value);
        //std::bitset<5> counterB(counter);

        //cout<<valueB.to_string();

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
    decode(pathIn);




    return 1;
}