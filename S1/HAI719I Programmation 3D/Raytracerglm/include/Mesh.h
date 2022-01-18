#ifndef MESH_H
#define MESH_H
#include "Object.h"
#include "glm/mat4x4.hpp"
#include <cfloat>
#include <vector>

class Triangle;
class Material;
struct MeshTriangle {
    inline MeshTriangle () {
        v[0] = v[1] = v[2] = 0;
    }
    inline MeshTriangle (const MeshTriangle & t) {
        v[0] = t.v[0];   v[1] = t.v[1];   v[2] = t.v[2];
    }
    inline MeshTriangle (unsigned int v0, unsigned int v1, unsigned int v2, Material * material) {
        v[0] = v0;   v[1] = v1;   v[2] = v2;
        meshMaterial = material;
    }
    unsigned int & operator [] (unsigned int iv) { return v[iv]; }
    unsigned int operator [] (unsigned int iv) const { return v[iv]; }
    void setMaterial(Material* material){this->meshMaterial = material;}
    inline virtual ~MeshTriangle () {}
    inline MeshTriangle & operator = (const MeshTriangle & t) {
        v[0] = t.v[0];   v[1] = t.v[1];   v[2] = t.v[2];
        return (*this);
    }
    // membres :
    unsigned int v[3];
    Material* meshMaterial;
};

class Mesh : public HittableObject
{
    public:
        Mesh();
        Mesh(std::vector<glm::vec4> vertices, std::vector<size_t> indices,std::vector<glm::vec4> normals);
        virtual bool intersect(const Ray& r, float t_min, float t_max, Intersection& intersection) const;
        virtual glm::vec4 vertexAt(size_t indice)const;
        void setMaterial(Material* material){this->meshMaterial = material;}
        void setMaterialTriangle(size_t indice, Material * material);
        void apply(glm::mat4 transformation);
        void flatShaded();
        void buildTriangles();
        void getBounds(glm::vec4& vmin,glm::vec4& vmax)
        {
            glm::vec4 nmin = glm::vec4(FLT_MAX,FLT_MAX,FLT_MAX,1.);
            glm::vec4 nmax = glm::vec4(-FLT_MAX,-FLT_MAX,-FLT_MAX,1.);
            float xmin = FLT_MAX;
            float ymin = FLT_MAX;
            float zmin = FLT_MAX;

            float xmax = -FLT_MAX;
            float ymax = -FLT_MAX;
            float zmax = -FLT_MAX;


            for(auto &vertice : vertices)
            {

                if(vertice[0]<xmin)xmin=vertice[0];
                if(vertice[1]<ymin)ymin=vertice[1];
                if(vertice[2]<zmin)zmin=vertice[2];
                
                if(xmax<vertice[0])xmax=vertice[0];
                if(ymax<vertice[1])ymax=vertice[1];
                if(zmax<vertice[2])zmax=vertice[2];

            }
            vmin =  glm::vec4(xmin,ymin,zmin,1.);;
            vmax =  glm::vec4(xmax,ymax,zmax,1.);;

            this->position = (vmin+vmax)*0.5;
        }
        
    protected:
        std::vector<glm::vec4> vertices;

    private:
    glm::mat4 transformation;
    std::vector<MeshTriangle*> triangles;
    Material * meshMaterial;
    std::vector<glm::vec4> normals;
    std::vector<glm::vec4> verticesNormals;
    std::vector<size_t> indices;


};
#endif