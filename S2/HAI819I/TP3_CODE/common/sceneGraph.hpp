#ifndef SCENE_GRAPH_H
#define SCENE_GRAPH_H
#include "common/GameObject.hpp"


class SceneGraphInterface
{
    /*
        Pattern Composite 
    */

    public:
        //virtual ~SceneGraphInterface();
        SceneGraphInterface();
        virtual void apply() = 0;
        virtual void inverse() = 0;


    
        GameObject* gameObject;

        SceneGraphInterface* parent;
        std::list< SceneGraphInterface* > children; 

       
};

class SceneGraphLeaf : public SceneGraphInterface
{
    public:
        SceneGraphLeaf();
        void apply() override;
        void inverse() override;


};
class SceneGraphComposite : public SceneGraphInterface
{
    public:
        SceneGraphComposite();
        void apply() override ;
        void inverse() override ;

        void add(SceneGraphInterface* child);
        void update();
};
#endif