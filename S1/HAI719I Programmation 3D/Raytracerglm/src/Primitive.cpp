#include "Primitive.h"
Sphere::Sphere()
{

}
Sphere::Sphere(float radius)
{
    this->radius = radius;
        
}
Sphere::Sphere(glm::vec4 position, float radius)
{
    this->position = position;
    this->radius = radius;
}
Sphere::Sphere(glm::vec4 position, float radius, Material* material)
{
    this->position = position;
    this->radius = radius;
    this->material = material;
}

bool Sphere::intersect(const Ray& r, float t_min, float t_max, Intersection& intersection) const
{
    float t0, t1;
    
    glm::vec4 L = r.getPosition() - this->position; 
    float a = r.getDirection().dot(r.getDirection()); 
    float b = 2 * (r.getDirection().dot(L)); 
    float c = L.dot(L) - this->radius; 

    if (!solveQuadratic(a, b, c, t0, t1)) return false; 
    
    if (t0 > t1) std::swap(t0, t1); 

    if (t0 < t_min) { 
        t0 = t1; 
        if (t0 < t_min || t0 > t_max) return false; 
    } 

    intersection.t = t0;
    intersection.hitPoint = r.at(intersection.t);
    intersection.normal = (intersection.hitPoint-this->position)*1./this->radius;
    intersection.material = material;
    return true; 
}
bool Box::intersect(const Ray& r, float t_min, float t_max, Intersection& intersection)const
{
    
    float tmin, tmax, tymin, tymax, tzmin, tzmax; 

    tmin = (bounds[r.sign[0]][0] - r.getPosition()[0]) * r.invdir[0]; 
    tmax = (bounds[1-r.sign[0]][0] - r.getPosition()[0]) * r.invdir[0]; 
    tymin = (bounds[r.sign[1]][1] - r.getPosition()[1]) * r.invdir[1]; 
    tymax = (bounds[1-r.sign[1]][1] - r.getPosition()[1]) * r.invdir[1]; 

    if ((tmin > tymax) || (tymin > tmax)) 
        return false; 
    if (tymin > tmin) 
        tmin = tymin; 
    if (tymax < tmax) 
        tmax = tymax; 

    tzmin = (bounds[r.sign[2]][2] - r.getPosition()[2]) * r.invdir[2]; 
    tzmax = (bounds[1-r.sign[2]][2] - r.getPosition()[2]) * r.invdir[2]; 

    if ((tmin > tzmax) || (tzmin > tmax)) 
        return false; 
    if (tzmin > tmin) 
        tmin = tzmin; 
    if (tzmax < tmax) 
        tmax = tzmax;

    intersection.t = std::min(tmin,tmax);
    intersection.hitPoint = r.at(intersection.t);
    intersection.normal = glm::vec4(0.,0.,1.,0.);
    intersection.material = new Material(glm::vec3(1.,1.,1.));
    return true; 

}