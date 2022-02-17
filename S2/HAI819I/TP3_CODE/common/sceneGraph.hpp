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
        virtual void methodForChildren() = 0;

    protected:
        GameObject* gameObject;

        SceneGraphInterface* parent;
        std::list< SceneGraphInterface* > children; 

       
};

class SceneGraphLeaf : public SceneGraphInterface
{
    public:
        SceneGraphLeaf();
        void methodForChildren() override;
};
class SceneGraphComposite : public SceneGraphInterface
{
    public:
        SceneGraphComposite();
        void methodForChildren() override ;
        void add(SceneGraphLeaf* child);
};
#endif