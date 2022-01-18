#ifndef LIGHT_H
#define LIGHT_H
#include "Object.h"

class Light : public Object
{
    public:
        glm::vec3 getColor(){return color;}
        size_t getSample(){return sample;}
        float getIntensity(){return diffuseIntensity;}
        float getSpecularIntensity(){return specularIntensity;}
        virtual float getRadius()=0;
        
    protected:
        glm::vec3 color;
        size_t sample;
        float diffuseIntensity;
        float specularIntensity;
};
class PointLight : public Light
{
    public:
        PointLight(glm::vec4 position, glm::vec3 color,float radius, float diffuseIntensity,float specularIntensity );
        virtual float getRadius(){return radius;}

    private:
        float radius;
};
class AreaLight : public Light
{
    public:
        AreaLight(glm::vec4 position,glm::vec4 v1,glm::vec4 v2, glm::vec3 color,size_t sample, float diffuseIntensity,float specularIntensity )
        {
            this->position = position; 
            this->v1 = v1;
            this->v2 = v2;   
            this->sample = sample;
            this->color = color;
            this->diffuseIntensity = diffuseIntensity;
            this->specularIntensity = specularIntensity;
        }
        float getRadius(){return 1.;};
        

    private:
        glm::vec4 v1;
        glm::vec4 v2;
};

#endif