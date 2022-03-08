#include "common/sceneGraph.hpp"
#include <iostream>
SceneGraphInterface::SceneGraphInterface()
{

}
SceneGraphLeaf::SceneGraphLeaf()
{

}
void SceneGraphLeaf::apply() {
        gameObject->apply();
}
void SceneGraphLeaf::inverse() {
        gameObject->inverse();
}
SceneGraphComposite::SceneGraphComposite()
{

}
void SceneGraphComposite:: apply() {
        
        gameObject->apply();
        for( auto &child : children ) {
            child->apply();
        }
}

void SceneGraphComposite:: inverse() {
        
        gameObject->inverse();
        for( auto &child : children ) {
            //child->gameObject->parentTransformation=gameObject->transformation;
            child->inverse();
        }
}
void SceneGraphComposite::add(SceneGraphInterface* child)
{

     //child->gameObject->transformation=gameObject->transformation;
     child->gameObject->parentTransformation=gameObject->transformation;
     children.push_back(child);

}
void SceneGraphComposite::update()
{
     //child->gameObject->transformation=gameObject->transformation;
        for( auto &child : children ) {
            child->gameObject->parentTransformation=gameObject->transformation;
        }
     

}