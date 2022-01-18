#include "Object.h"

Object::Object()
{
    position = glm::vec4(0.,0.,0.,1.);
}
HittableObject::HittableObject()
{
    
}

bool HittableList::intersect(const Ray& r, float t_min, float t_max, Intersection& intersection) const
{
    Intersection temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : objects) {
        // speedup 10times
        if (object->box->intersect(r, t_min, t_max, temp_rec)) {
              
            if (object->intersect(r, t_min, t_max, temp_rec)) {
                
                hit_anything = true;
                if(temp_rec.t<closest_so_far)
                {
                    closest_so_far = temp_rec.t;
                    intersection = temp_rec;    
                }
            }
            
       }
        
    }

    return hit_anything;
}
