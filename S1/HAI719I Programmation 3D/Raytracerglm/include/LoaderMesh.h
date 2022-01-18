#ifndef LOADER_MESH_H
#define LOADER_MESH_H

#include <string>
#include <vector>
#include "Mesh.h"
class LoaderMesh 
{ 
    private:

};

class LoaderObj : public LoaderMesh
{
    public:
        LoaderObj();
        LoaderObj(std::string filename);
        shared_ptr<Mesh> buildMesh();
                std::vector<glm::vec4> getVertices(){return vertices;};
        std::vector<size_t> getIndices(){return indices;};
        std::vector<glm::vec4> getNormals(){return normals;};
    private:
        std::vector<glm::vec4> vertices;
        std::vector<size_t> indices;
        std::vector<glm::vec4> normals;

        std::vector<std::string> parseToken(std::string s, std::string delimiter);
        std::vector<float> parseTokenToFloat(std::string s, std::string delimiter);

};
class LoaderOff : public LoaderMesh
{
    
};
#endif