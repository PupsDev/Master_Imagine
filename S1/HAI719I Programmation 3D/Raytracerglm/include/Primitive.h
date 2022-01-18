#ifndef PRIMITIVE_H
#define PRIMITIVE_H
#include "Object.h"


class Primitive : public HittableObject
{
    public:
        virtual bool intersect(const Ray& r, float t_min, float t_max, Intersection& intersection)const =0;
        virtual void getBounds(glm::vec4& vmin,glm::vec4& vmax)=0;
};

class Sphere : public Primitive
{
    public:
        Sphere();
        Sphere(float radius);
        Sphere(glm::vec4 position,float radius);
        Sphere(glm::vec4 position, float radius, Material* material);
        virtual bool intersect(const Ray& r, float t_min, float t_max, Intersection& intersection) const;
        void getBounds(glm::vec4& vmin,glm::vec4& vmax)
        {
            float size = std::max(radius,1.f);
            vmin = this->position-glm::vec4(size,size,size,0.);
            vmax = this->position+glm::vec4(size,size,size,0.);

        }
        float getRadius()const{return radius;}

    private:
        float radius;
        
};
class Box : public Primitive
{
    public:
        Box();
        Box(glm::vec4 vmin, glm::vec4 vmax)
        {
            this->bounds[0] = vmin;
            this->bounds[1] = vmax;

        }
        virtual bool intersect(const Ray& r, float t_min, float t_max, Intersection& intersection)const;
        virtual void getBounds(glm::vec4& vmin,glm::vec4& vmax)
        {
            vmin = bounds[0];
            vmax = bounds[1];
        }


    private:
       glm::vec4 bounds[2];
    
};
#endif