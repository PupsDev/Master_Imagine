#ifndef MATERIAL_H
#define MATERIAL_H
#include "glm/vec3.hpp"


enum MaterialType {
    Material_Diffuse_Blinn_Phong ,
    Material_Glass,
    Material_Mirror
};

class Material 
{ 
    public:
        Material();
        Material(glm::vec3 color);

        glm::vec3 getDiffuse(){return diffuse_material;};
        glm::vec3 getSpecular(){return specular_material;};
        float getDiffuseK(){return kDiffuse;};
        float getSpecularK(){return kSpecular;};
        float getIndiceSpecular(){return indiceSpecular;};

        MaterialType getType() const {
                return type;
        }

        void setType(MaterialType type) {
                this->type = type;
        }   
    private:
        float index_medium;
        float transparency;

        glm::vec3 diffuse_material;
        float kDiffuse;

        glm::vec3 specular_material;
        float kSpecular;
        float indiceSpecular;

        MaterialType type;

};


#endif