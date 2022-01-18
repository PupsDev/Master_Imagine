#include "Light.h"


PointLight::PointLight(mlm::vec4 position, mlm::vec3 color,float radius, float diffuseIntensity,float specularIntensity )
{
    this->position = position;    
    this->sample = 1;
    this->radius = radius;
    this->color = color;
    this->diffuseIntensity = diffuseIntensity;
    this->specularIntensity = specularIntensity;
}