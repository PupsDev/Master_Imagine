#ifndef BVH_H
#define BVH_H

#include "Object.h"

class BVH : public HittableObject
{
    public:
        BVH()
        {
            const glm::vec4 normals[3] = { 
            glm::vec4(1., 0., 0.,0.), 
            glm::vec4(0., 1., 0.,0.), 
            glm::vec4(0., 0., 1.,0.), 
            }; 
        }
        void computeTree(std::vector<shared_ptr<HittableObject>>& objects)
        {
            /*std::vector<shared_ptr<HittableObject>> boxes;
            for(size_t i = 0 ; i < objects.size();i++)
            {   
                glm::vec4 vmin,vmax;
                objects[i]->getBounds(vmin,vmax);
                  
                //boxes.push_back(make_shared<Box>(vmin,vmax ));
            }*/
            /*
            for(size_t i = 0 ; i < boxes.size();i++)
            { 
                objects.push_back(boxes[i]);
               
            }*/
        }
        void getBounds(glm::vec4& vmin,glm::vec4& vmax){};
        bool intersect(const Ray& r, float t_min, float t_max, Intersection& intersection) const
        {
            return false;
        }
    protected:

};


#endif