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
            this->orientation=new Transform();
            this->parentTransformation=new Transform();
        }
        void apply()
        { 
            //orientation->print();
            transformation->print();
            //parentTransformation->print();  
            for(auto& point : mesh->points)
            {
                point = orientation->applyToPoint(point);
                //point = parentTransformation->applyToPointRevolution(point);
                point = transformation->applyToPointRevolution(point);
            }

        }
        void inverse()
        {   
            transformation->inverse();    
            transformation->print();  
            //parentTransformation->inverse();
            for(auto& point : mesh->points)
            {
                point = transformation->applyToPointRevolution(point);
                //point = parentTransformation->applyToPointRevolution(point);
            }

        }


    
        Mesh * mesh;
        Transform* orientation;
        Transform* transformation;
        Transform* parentTransformation;
        std::list<  Component * > components;
};
#endif
