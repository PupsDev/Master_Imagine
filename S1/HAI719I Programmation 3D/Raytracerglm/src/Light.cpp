#include "Light.h"


PointLight::PointLight(glm::vec4 position, glm::vec3 color,float radius, float diffuseIntensity,float specularIntensity )
{
    this->position = position;    
    this->sample = 1;
    this->radius = radius;
    this->color = color;
    this->diffuseIntensity = diffuseIntensity;
    this->specularIntensity = specularIntensity;
}