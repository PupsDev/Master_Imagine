#include "Ray.h"

Ray::Ray()
{
    
}
Ray::Ray(const mlm::vec4 &position , const mlm::vec4 &direction  )
{
    this->position = position;
    this->direction = direction;
    this->invdir = mlm::vec4(1. / direction[0],1. / direction[1],1. / direction[2],0.); 
    this->sign[0] = (invdir[0] < 0); 
    this->sign[1] = (invdir[1] < 0); 
    this->sign[2] = (invdir[2] < 0); 
}