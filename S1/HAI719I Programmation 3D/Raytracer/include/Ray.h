#ifndef RAY_H
#define RAY_H
#include "mlm/vec4.h"

class Ray 
{
    public:
        Ray();
        Ray(const mlm::vec4 &position , const mlm::vec4 &direction );
        mlm::vec4 at(float t)const{return position + t*direction;}
        mlm::vec4 getPosition()const{return position;}
        void setPosition(mlm::vec4 position){this->position=position;}
        mlm::vec4 getDirection()const{return direction;}
        mlm::vec4 invdir; 
        int sign[3]; 

    private:
        mlm::vec4 position;
        mlm::vec4 direction;


};

#endif