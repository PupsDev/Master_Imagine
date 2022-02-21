#ifndef GAME_OBJECT
#define GAME_OBJECT

#include "common/Transform.hpp"
#include <memory>
#include <list>
#include <vector>
#include "common/Mesh.hpp"
class Component;

class GameObject
{


    public:
        GameObject()
        {
            this->mesh=new Mesh();
            this->transformation=new Transform();
            this->parentTransformation=new Transform();
        }
        void apply()
        {   
            for(auto& point : mesh->points)
            {
                point = parentTransformation->applyToPoint(point);
                point = transformation->applyToPoint(point);
            }

        }
        void inverse()
        {   
            transformation->inverse();      
            parentTransformation->inverse();
            for(auto& point : mesh->points)
            {
                point = transformation->applyToPoint(point);
                point = parentTransformation->applyToPoint(point);
            }

        }


    
        Mesh * mesh;
        Transform* transformation;
        Transform* parentTransformation;
        std::list<  Component * > components;
};
#endif
