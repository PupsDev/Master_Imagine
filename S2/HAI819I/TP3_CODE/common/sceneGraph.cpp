#include "common/sceneGraph.hpp"
#include <iostream>
SceneGraphInterface::SceneGraphInterface()
{

}
SceneGraphLeaf::SceneGraphLeaf()
{

}
void SceneGraphLeaf::methodForChildren() {
        std::cout << "test" << std::endl;
}

SceneGraphComposite::SceneGraphComposite()
{

}
void SceneGraphComposite:: methodForChildren() {
        for( auto &child : children ) {
            child->methodForChildren();
        }
}
void SceneGraphComposite::add(SceneGraphLeaf* child)
{
     children.push_back(child);

}