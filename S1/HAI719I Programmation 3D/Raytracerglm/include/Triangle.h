#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "Mesh.h"
#include <vector>


class Triangle : public Mesh
{
    public:
        Triangle();
        Triangle(glm::vec4 const & v0 , glm::vec4 const & v1 , glm::vec4 const & v2 );
        Triangle(glm::vec4 const & v0 , glm::vec4 const & v1 , glm::vec4 const & v2, Material *material);
        Triangle(glm::vec4 const & v0 , glm::vec4 const & v1 , glm::vec4 const & v2, Material *material,glm::vec4 normal);
       
        void setMaterial(Material * material){this->material=material;};
        void updateAreaAndNormal();
        void setNormal(glm::vec4);
        void setVertexNormales(glm::vec4 vn0, glm::vec4 vn1, glm::vec4 vn2 ){vNormal[0]=vn0;vNormal[1]=vn1;vNormal[2]=vn2;};


        virtual bool intersect(const Ray& r, float t_min, float t_max, Intersection& intersection) const;

    private:
        glm::vec4 v[3];
        bool flatShaded;
        glm::vec4 vNormal[3];
        glm::vec4 normal;
        Material * material;
        float area;
        
        
};
#endif