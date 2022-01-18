#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "Mesh.h"
#include <vector>


class Triangle : public Mesh
{
    public:
        Triangle();
        Triangle(mlm::vec4 const & v0 , mlm::vec4 const & v1 , mlm::vec4 const & v2 );
        Triangle(mlm::vec4 const & v0 , mlm::vec4 const & v1 , mlm::vec4 const & v2, Material *material);
        Triangle(mlm::vec4 const & v0 , mlm::vec4 const & v1 , mlm::vec4 const & v2, Material *material,mlm::vec4 normal);
       
        void setMaterial(Material * material){this->material=material;};
        void updateAreaAndNormal();
        void setNormal(mlm::vec4);
        void setVertexNormales(mlm::vec4 vn0, mlm::vec4 vn1, mlm::vec4 vn2 ){vNormal[0]=vn0;vNormal[1]=vn1;vNormal[2]=vn2;};


        virtual bool intersect(const Ray& r, float t_min, float t_max, Intersection& intersection) const;

    private:
        mlm::vec4 v[3];
        bool flatShaded;
        mlm::vec4 vNormal[3];
        mlm::vec4 normal;
        Material * material;
        float area;
        
        
};
#endif