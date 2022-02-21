
#ifndef TEST
#define TEST

#include "common/Transform.hpp"
#include "common/sceneGraph.hpp"
#include <memory>
#include <list>
#include <vector>
#include "common/Mesh.hpp"
class Component;

class Test
{


    public:
        Test();
        void run()
        {   
            SceneGraphLeaf* leaf = new SceneGraphLeaf();
            SceneGraphLeaf* leaf2 = new SceneGraphLeaf();

            SceneGraphComposite* node = new SceneGraphComposite();
            Mesh * test = new Mesh();
            test->addPoint(glm::vec3(1.,0.,0.));

            Mesh * test2 = new Mesh();
            test2->addPoint(glm::vec3(0.,1.,0.));
            //test->print();
            Transform * t = new Transform(glm::vec3(2.,1.,1.));

            leaf->gameObject = new GameObject();
            leaf->gameObject->mesh = test;
            leaf->gameObject->transformation = t;

            leaf2->gameObject = new GameObject();
            leaf2->gameObject->mesh = test2;
            leaf2->gameObject->transformation = t;

            node->add(leaf);
            node->add(leaf2);
            node->apply();
            leaf->gameObject->mesh->print();
            leaf2->gameObject->mesh->print();

        }

    
};
#endif

