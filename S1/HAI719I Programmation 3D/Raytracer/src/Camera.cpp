#include "Camera.h"

Camera::Camera(Ray* ray)
{
        this->nearClip=0.1;
        this->farClip=100.;
        this->fov = 40.;
        this->scale = tan((this->fov * 0.5 )* M_PI / 180);          
        this->rotation = mlm::rotationX(0.);
        this->ray = ray;
        
}
