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
            //transformation->print();
            //parentTransformation->print();  

            //Transform* trans = transformation->multiply(parentTransformation);
            //Transform* trans = parentTransformation->multiply(transformation);
            //trans = trans->multiply(trans);
            Transform* trans = parentTransformation->multiply(transformation);
            trans->print();
            for(auto& point : mesh->points)
            {
                //point = orientation->applyToPoint(point);
                //point = parentTransformation->applyToPointRevolution(point);
                //point = transformation->applyToPointRevolution(point);
                point = trans->applyToPoint(point);
            }
            
        }

        void inverse()
        {   
            Transform* trans = parentTransformation->multiply(transformation);
            //transformation->print();  
            Transform* itrans = trans->inverse();  
            //Transform* trans = transformation->multiply(itrans);  
            //trans->inverse();  
            
            //transformation->print();  
            //itrans->print();
            //parentTransformation->inverse();
            for(auto& point : mesh->points)
            {
                //point = parentTransformation->inversePoint(point);
                point = itrans->inversePoint(point);
            }

        }


    
        Mesh * mesh;
        Transform* orientation;
        Transform* transformation;
        Transform* parentTransformation;
        std::list<  Component * > components;
};
#endif
