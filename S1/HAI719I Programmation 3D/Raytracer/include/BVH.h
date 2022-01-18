#ifndef BVH_H
#define BVH_H

#include "Object.h"

class BVH : public HittableObject
{
    public:
        BVH()
        {
            const mlm::vec4 normals[3] = { 
            mlm::vec4(1., 0., 0.,0.), 
            mlm::vec4(0., 1., 0.,0.), 
            mlm::vec4(0., 0., 1.,0.), 
            }; 
        }
        void computeTree(std::vector<shared_ptr<HittableObject>>& objects)
        {

        }
        void getBounds(mlm::vec4& vmin,mlm::vec4& vmax){};
        bool intersect(const Ray& r, float t_min, float t_max, Intersection& intersection) const
        {
            return false;
        }
    protected:

};


#endif