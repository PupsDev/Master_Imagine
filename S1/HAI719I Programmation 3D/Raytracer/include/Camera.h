#ifndef CAMERA_H
#define CAMERA_H
#include "Ray.h"
#include "mlm/mat4.h"
class Camera
{
    private:
        Ray* ray;
        float nearClip;
        float farClip;
        float fov;
        float scale;
        mlm::mat4 rotation;

    public:
        Camera(Ray* ray);
        mlm::vec4 getPosition(){return ray->getPosition();}
        mlm::mat4 getRotation(){return rotation;}
        void setPosition(mlm::vec4 position){ray->setPosition(position);}
        void setRotation( mlm::mat4 rotation){this->rotation =rotation;}
};
#endif