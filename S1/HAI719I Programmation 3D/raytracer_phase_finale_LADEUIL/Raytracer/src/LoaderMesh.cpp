#include "LoaderMesh.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "mlm/vec4.h"
#include "Triangle.h"



LoaderObj::LoaderObj()
{

}
LoaderObj::LoaderObj(std::string filename)
{
    std::ifstream input_stream(filename);

    if (!input_stream) std::cerr << "Can't open input file!";
    std::cout << "Loading "<<filename<<"...";
    std::vector<std::string> text;
    std::string delimiter = " ";
    std::vector<float> tokensFloat;
    std::string line;



    while (getline(input_stream, line)) {
        text.push_back(line);
    }
    
    std::string name;
    std::vector<mlm::vec4> vertices;
    std::vector<mlm::vec4> normals;
    std::vector<mlm::vec4> textures;
    size_t indice_v=0,indice_n=0,indice_vt=0;

    std::vector<Triangle> faces;
    std::vector<size_t> indices;
    std::vector<std::string> pointToken;
    std::vector<std::string> vertTexNorm;

    std::vector<size_t> tempFaceindices;
    std::vector<size_t> tempFaceNormalindices;
    std::vector<mlm::vec4> normalsByIndice;
    size_t faceCount = 0;
    

    for( size_t i = 0 ; i < text.size(); i++)
    {
        switch(text[i][0])
        {
            
            case 'o':
                // o is [0] -> name is [1]
                // Name of the Mesh
                name = parseToken(text[i], delimiter)[1];
                break;
            case 'v':
                tokensFloat = parseTokenToFloat(text[i], delimiter);
               

                switch(text[i][1])
                {
                    case 'n': // normals
                        normals.push_back( mlm::vec4(tokensFloat[0],tokensFloat[1],tokensFloat[2],1.));
                    break;
                    case 't': // textures
                        textures.push_back( mlm::vec4(tokensFloat[0],tokensFloat[1],1.,1.));
                    break;
                    default: // 'v' -> vertices
                        vertices.push_back( mlm::vec4(tokensFloat[0],tokensFloat[1],tokensFloat[2],1.));
                    break;
                }

                break;
            case 's':
                break;
            case 'f':
                pointToken = parseToken(text[i], " ");
                /*
                Check for v or v/t or v//vn instead of v/t/vn
                */
                for(size_t j = 1 ; j < 4 ; j++)
                {
                    vertTexNorm = parseToken(pointToken[j], "/");
                    // j : 1 - 4 -> point 1 - 4  as vtn
                    tempFaceindices.push_back((size_t)(std::stoi(vertTexNorm[0])-1));

                    tempFaceNormalindices.push_back((size_t)(std::stoi(vertTexNorm[2])-1));
                }
                // Making triangle
                
                            
                indices.push_back(tempFaceindices[faceCount]);
                indices.push_back(tempFaceindices[faceCount+1]);
                indices.push_back(tempFaceindices[faceCount+2]);
                normalsByIndice.push_back(normals[tempFaceNormalindices[faceCount]]);
                faceCount+=3;


                break;
            default:
                break;

        }
    }
    this->vertices = vertices;
    this->indices = indices;
    this->normals = normalsByIndice;
    std::cout<<"Done !\n";

}
shared_ptr<Mesh> LoaderObj::buildMesh()
{
    return make_shared<Mesh>(vertices,indices,normals);
}
std::vector<std::string> LoaderObj::parseToken(std::string s, std::string delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    size_t pos = 0;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        tokens.push_back(token); 
         
        s.erase(0, pos + delimiter.length());
    }
    tokens.push_back(s); 
    return tokens;
}
std::vector<float> LoaderObj::parseTokenToFloat(std::string s, std::string delimiter)
{
    std::vector<float> tokens;
    std::string token;
    size_t pos = 0;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        if(token[0]!='v')
            tokens.push_back(std::stof(token)); 
         
        s.erase(0, pos + delimiter.length());
    }
    tokens.push_back(std::stof(s)); 
    return tokens;
}