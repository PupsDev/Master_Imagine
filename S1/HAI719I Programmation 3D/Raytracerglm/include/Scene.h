#ifndef SCENE_H
#define SCENE_H
#include <vector>
#include "Primitive.h"
#include "BVH.h"
#include "Light.h"



class Scene
{
    
    public:
        HittableList hittables;
        std::vector< Light* > lights;
        
        Scene(){this->bvh = new BVH();}
        void add(shared_ptr<HittableObject> object){hittables.add(object);}
        
        void buildBVH()
        {
            //bvh->computeTree(hittables.objects);
        }
        
        void addLight(Light * light){lights.push_back(light);}
    private:
        BVH * bvh;

        
        
};
#endif