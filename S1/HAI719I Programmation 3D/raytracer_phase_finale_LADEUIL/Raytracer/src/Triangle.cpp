#include "Triangle.h"

Triangle::Triangle()
{

}

Triangle::Triangle(mlm::vec4 const & v0 , mlm::vec4 const & v1 , mlm::vec4 const & v2 )
{
    v[0] = v0;
    v[1] = v1;
    v[2] = v2;
    updateAreaAndNormal();
}
Triangle::Triangle(mlm::vec4 const & v0 , mlm::vec4 const & v1 , mlm::vec4 const & v2, Material *material)
{
    v[0] = v0;
    v[1] = v1;
    v[2] = v2;
    this->material = material;
    updateAreaAndNormal();
}
Triangle::Triangle(mlm::vec4 const & v0 , mlm::vec4 const & v1 , mlm::vec4 const & v2,Material *material, mlm::vec4 normal)
{
    v[0] = v0;
    v[1] = v1;
    v[2] = v2;
    this->material = material;
    this->normal = normal;
    this->flatShaded = false;

}

void Triangle::setNormal(mlm::vec4 normal)
{
    this->normal = normal;
}

void Triangle::updateAreaAndNormal()
{
    mlm::vec4 v1 = mlm::vec4( v[1] - v[0]);
    mlm::vec4 v2 = mlm::vec4( v[2] - v[0]);
    mlm::vec4 nNotNormalized = mlm::cross(v1,v2);
    float norm = nNotNormalized.length();
    area = norm / 2.f;
}

bool Triangle::intersect(const Ray& r, float t_min, float t_max, Intersection& intersection) const
{
    mlm::vec4 e1, e2 , h, s, q;
    float a,f,u,v,t;
    e1=this->v[1]-this->v[0];
    e2=this->v[2]-this->v[0];
    mlm::vec4 orig = r.getPosition();
    mlm::vec4 dir = r.getDirection();
   
   
    h = mlm::cross(dir,e2);
    a=e1.dot(h);

    if (a > -t_min && a < t_min)
        return false;    // Le rayon est parallèle au triangle.
    
    // Single Sided
    /*
    if(dir.dot(normal)<0)
    {
        return false;
    }
    */

    f = 1.0/a;
    
    s = orig - this->v[0];
   
    u = f * (s.dot(h));

    if (u < 0.0 || u > 1.0)
        return false;

    q = mlm::cross(s,e1);
    v = f * dir.dot(q);
    
    if (v < 0.0 || u + v > 1.0)
        return false;
    
    t = f * e2.dot(q);

    if (t > t_min && t < t_max) // Intersection avec le rayon
    {
        float area = mlm::dot( normal, mlm::cross( (this->v[1] - this->v[0]), (this->v[2] - this->v[0]) )  ) ;
        float areaPBC = mlm::dot( normal, mlm::cross( (this->v[1] - this->v[0]), (this->v[2] - this->v[0]) )  ) ;
        float areaPCA = mlm::dot( normal, mlm::cross( (this->v[2] - this->v[0]), (this->v[0] - this->v[0]) )  ) ;

        float u0 = areaPBC / area ; // alpha
        float u1 = areaPCA / area ; // beta
        float u2 = 1.0f - u0 - u1 ; // gamma

        intersection.t = t;
        
        intersection.hitPoint = r.at(intersection.t);

        mlm::vec4 norm = vNormal[0]*u0 + vNormal[1]*u1+ vNormal[2]*u2;

        if(flatShaded)
        {
            intersection.normal = normal;
        }
        else
        {
            intersection.normal = norm;
        }
        
        intersection.frontFace= true;
        intersection.material = material;
        
        return true;
    }
    else // On a bien une intersection de droite, mais pas de rayon.
        return false;
    

    
}