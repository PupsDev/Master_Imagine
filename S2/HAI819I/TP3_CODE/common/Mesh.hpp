#ifndef MESH
#define MESH

#include "common/Transform.hpp"
#include <memory>
#include <list>
#include <vector>
#include <iostream>

class Mesh
{


    public:

    void print()
    {
        for(auto& point:points)
        {
            std::cout<<"("<<point[0]<<" ; "<<point[1]<<" ; "<<point[2]<<" )\n";
        }
    }
    void addPoint(glm::vec3 p)
    {
        points.push_back(p);
    }
    std::vector<glm::vec3> points;
    std::vector<unsigned short> indices;
    std::vector<glm::vec2> uvs;


};
#endif
