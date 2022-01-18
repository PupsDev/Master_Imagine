#ifndef OBJECT_H
#define OBJECT_H
#include "mlm/vec4.h"
#include "Ray.h"
#include "Material.h"
#include <memory>
#include <vector>


using std::shared_ptr;
using std::make_shared;


struct Intersection{
    bool intersectionExists;
    mlm::vec4 hitPoint;
    mlm::vec4 normal;
    Material* material;
    bool frontFace=false;
    float t;
};

inline bool solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1) 
{ 
    float discr = b * b - 4 * a * c; 
    if (discr < 0) return false; 
    else if (discr == 0) x0 = x1 = - 0.5 * b / a; 
    else { 
        float q = (b > 0) ? 
            -0.5 * (b + sqrt(discr)) : 
            -0.5 * (b - sqrt(discr)); 
        x0 = q / a; 
        x1 = c / q; 
    } 
    if (x0 > x1) std::swap(x0, x1); 
 
    return true; 
} 
inline float clip(float n, float lower, float upper) {
  if(n < lower)return lower;
  if(n>upper)return upper;
  return n;
    
}
class Box;
class Object
{
    public:
        Object();

        mlm::vec4 getPosition()const{return position;}
        

    protected:

        mlm::vec4 position;

};
class HittableObject : public Object
{
    public:
        HittableObject();
        virtual bool intersect(const Ray& r, float t_min, float t_max, Intersection& intersection)const =0;
        virtual void getBounds(mlm::vec4& vmin,mlm::vec4& vmax)=0;

        void computeBox()
        {
             mlm::vec4 vmin,vmax;
            this->getBounds(vmin,vmax);
            box = make_shared<Box>(vmin,vmax);
        }

        shared_ptr<Box> box;
    protected:
        
        Material* material; 

};

class HittableList : public HittableObject
{
    public:
        HittableList(){};
        HittableList(shared_ptr<HittableObject> object) { add(object); }

        void clear() { objects.clear(); }
        void add(shared_ptr<HittableObject> object) { 
            object->computeBox();
            objects.push_back(object); 

            
            }
        


        virtual bool intersect(const Ray& r, float t_min, float t_max, Intersection& intersection) const;
        virtual void getBounds(mlm::vec4& vmin,mlm::vec4& vmax){};
        std::vector<shared_ptr<HittableObject>> objects;
    private:
        
        
       

};
#endif