#ifndef LIGHT_H
#define LIGHT_H
#include "Object.h"

class Light : public Object
{
    public:
        mlm::vec3 getColor(){return color;}
        size_t getSample(){return sample;}
        float getIntensity(){return diffuseIntensity;}
        float getSpecularIntensity(){return specularIntensity;}
        virtual float getRadius()=0;
        virtual mlm::vec4 getVec(int indice)=0;
        
    protected:
        mlm::vec3 color;
        size_t sample;
        float diffuseIntensity;
        float specularIntensity;
};
class PointLight : public Light
{
    public:
        PointLight(mlm::vec4 position, mlm::vec3 color,float radius, float diffuseIntensity,float specularIntensity );
        virtual float getRadius(){return radius;}
        virtual mlm::vec4 getVec(int indice){return mlm::vec4(0.,0.,0.,0.); };
    private:
        float radius;
};
class AreaLight : public Light
{
    public:
        AreaLight(mlm::vec4 position,mlm::vec4 v1,mlm::vec4 v2, mlm::vec3 color,size_t sample, float diffuseIntensity,float specularIntensity )
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
        mlm::vec4 getVec(int indice){ mlm::vec4 vec; indice == 1 ? vec = v1 :vec=v2 ;return vec;};
        

    private:
        mlm::vec4 v1;
        mlm::vec4 v2;
};

#endif