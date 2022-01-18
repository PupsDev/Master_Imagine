#ifndef CAMERA_H
#define CAMERA_H
#include "Ray.h"
#include "glm/mat4x4.hpp"
class Camera
{
    private:
        Ray* ray;
        float nearClip;
        float farClip;
        float fov;
        float scale;
        glm::mat4 rotation;

    public:
        Camera(Ray* ray);
        glm::vec4 getPosition(){return ray->getPosition();}
        glm::mat4 getRotation(){return rotation;}
        void setPosition(glm::vec4 position){ray->setPosition(position);}
        void setRotation( glm::mat4 rotation){this->rotation =rotation;}
};
#endif