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
            this->transformation;
            this->parentTransformation;
        }
        void apply()
        { 
            //orientation->print();
            //transformation->print();
            //parentTransformation->print();  

            //Transform* trans = transformation->multiply(parentTransformation);
            //Transform* trans = parentTransformation->multiply(transformation);
            //trans = trans->multiply(trans);
            //Transform* trans = parentTransformation->multiply(transformation);
            //trans->print();
            for(auto& point : mesh->points)
            {
                for(auto& trans : parentTransformation)
                {
                    point = trans->applyToPoint(point);
                }
                for(auto& trans : transformation)                {
                    point = trans->applyToPoint(point);
                }
                //point = orientation->applyToPoint(point);
                //point = parentTransformation->applyToPointRevolution(point);
                //point = transformation->applyToPointRevolution(point);
                
            }
            
        }
        void apply(Transform * t)
        { 

            for(auto& point : mesh->points)
            {
                //point = orientation->applyToPoint(point);
                //point = parentTransformation->applyToPointRevolution(point);
                //point = transformation->applyToPointRevolution(point);
                point = t->applyToPoint(point);
            }
            
        }

        void inverse()
        {   
            //Transform* trans = parentTransformation->multiply(transformation);
             //transformation->print();  
            //Transform* itrans = trans->inverse();  
             //Transform* trans = transformation->multiply(itrans);  
             //trans->inverse();  
            
            //transformation->print();  
            //itrans->print();
            //parentTransformation->inverse();
            for(auto& point : mesh->points)
            {
                //point = parentTransformation->inversePoint(point);
                //point = itrans->inversePoint(point);
                int size = transformation.size();
                for(int i = 0 ; i < size;i++)                {
                    Transform * itrans = transformation[size-i-1]->inverse(); 
                    point = itrans->applyToPoint(point);
                }
                size = parentTransformation.size();
                for(int i = 0 ; i < size;i++)                {
                    Transform * itrans = parentTransformation[size-i-1]->inverse(); 
                    point = itrans->applyToPoint(point);
                }
                
            }

        }


    
        Mesh * mesh;

        std::vector<Transform*> transformation;
        std::vector<Transform*> parentTransformation;
        std::list<  Component * > components;
};
#endif
