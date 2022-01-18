#ifndef RAY_H
#define RAY_H
#include "glm/vec4.hpp"

class Ray 
{
    public:
        Ray();
        Ray(const glm::vec4 &position , const glm::vec4 &direction );
        glm::vec4 at(float t)const{return position + t*direction;}
        glm::vec4 getPosition()const{return position;}
        void setPosition(glm::vec4 position){this->position=position;}
        glm::vec4 getDirection()const{return direction;}
        glm::vec4 invdir; 
        int sign[3]; 

    private:
        glm::vec4 position;
        glm::vec4 direction;


};

#endif