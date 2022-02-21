
#ifndef TEST
#define TEST

#include "common/Transform.hpp"
#include "common/sceneGraph.hpp"
#include <memory>
#include <list>
#include <vector>
#include "common/Mesh.hpp"

class Test
{


    public:
        Test()
        {

        };
        void run()
        {   
            SceneGraphComposite* car = new SceneGraphComposite();

            SceneGraphComposite* essieu = new SceneGraphComposite();


            SceneGraphLeaf* roue1 = new SceneGraphLeaf();
            SceneGraphLeaf* roue2 = new SceneGraphLeaf();
            SceneGraphLeaf* roue3 = new SceneGraphLeaf();
            SceneGraphLeaf* roue4 = new SceneGraphLeaf();


            Mesh * mesh_roue1 = new Mesh();
            Mesh * mesh_roue2 = new Mesh();
            Mesh * mesh_roue3 = new Mesh();
            Mesh * mesh_roue4 = new Mesh();

            Mesh * mesh_essieu = new Mesh();

            Mesh * mesh_car = new Mesh();

            mesh_roue1->addPoint(glm::vec3(1.,0.,0.));
            mesh_roue1->addPoint(glm::vec3(2.,0.,0.));
            mesh_roue1->addPoint(glm::vec3(3.,0.,0.));
            mesh_roue1->addPoint(glm::vec3(4.,0.,0.));

            mesh_essieu->addPoint(glm::vec3(0.,1.,0.));
            mesh_car->addPoint(glm::vec3(0.,0.,1.));

            Transform * t = new Transform(glm::vec3(1.,1.,1.));

            roue1->gameObject = new GameObject();
            roue2->gameObject = new GameObject();
            roue3->gameObject = new GameObject();
            roue4->gameObject = new GameObject();

            essieu->gameObject = new GameObject();

            car->gameObject = new GameObject();

            roue1->gameObject->mesh = mesh_roue1;
            roue1->gameObject->transformation = t;
            roue2->gameObject->mesh = mesh_roue2;
            roue2->gameObject->transformation = t;
            roue3->gameObject->mesh = mesh_roue3;
            roue3->gameObject->transformation = t;
            roue4->gameObject->mesh = mesh_roue4;
            roue4->gameObject->transformation = t;

            essieu->gameObject->mesh = mesh_essieu;
            essieu->gameObject->transformation = t;

            car->gameObject->mesh = mesh_car;
            car->gameObject->transformation = t;
            
            essieu->add(roue1);
            essieu->add(roue2);
            essieu->add(roue3);
            essieu->add(roue4);
            car->add(essieu);
            //world->add(car);
            
            car->apply();

            roue1->gameObject->mesh->print();
            roue2->gameObject->mesh->print();
            roue3->gameObject->mesh->print();
            roue4->gameObject->mesh->print();
            essieu->gameObject->mesh->print();
            car->gameObject->mesh->print();


        }
        void runRotation()
        {
            SceneGraphComposite* car = new SceneGraphComposite();

            SceneGraphComposite* essieu = new SceneGraphComposite();


            SceneGraphLeaf* roue1 = new SceneGraphLeaf();
            SceneGraphLeaf* roue2 = new SceneGraphLeaf();
            SceneGraphLeaf* roue3 = new SceneGraphLeaf();
            SceneGraphLeaf* roue4 = new SceneGraphLeaf();


            Mesh * mesh_roue1 = new Mesh();
            Mesh * mesh_roue2 = new Mesh();
            Mesh * mesh_roue3 = new Mesh();
            Mesh * mesh_roue4 = new Mesh();

            Mesh * mesh_essieu = new Mesh();

            Mesh * mesh_car = new Mesh();

            mesh_roue1->addPoint(glm::vec3(1.,0.,0.));
            mesh_roue1->addPoint(glm::vec3(2.,0.,0.));
            mesh_roue1->addPoint(glm::vec3(3.,0.,0.));
            mesh_roue1->addPoint(glm::vec3(4.,0.,0.));

            mesh_essieu->addPoint(glm::vec3(0.,1.,0.));
            mesh_car->addPoint(glm::vec3(0.,0.,1.));

                    
            Transform * t = new Transform( Transform::convertMat4(glm::rotate(glm::mat4(1.0f), (float)M_PI, glm::vec3(0.,1.,0.0))));

            roue1->gameObject = new GameObject();
            roue2->gameObject = new GameObject();
            roue3->gameObject = new GameObject();
            roue4->gameObject = new GameObject();

            essieu->gameObject = new GameObject();

            car->gameObject = new GameObject();

            roue1->gameObject->mesh = mesh_roue1;
            roue1->gameObject->transformation = t;
            roue2->gameObject->mesh = mesh_roue2;
            roue2->gameObject->transformation = t;
            roue3->gameObject->mesh = mesh_roue3;
            roue3->gameObject->transformation = t;
            roue4->gameObject->mesh = mesh_roue4;
            roue4->gameObject->transformation = t;

            essieu->gameObject->mesh = mesh_essieu;
            essieu->gameObject->transformation = t;

            car->gameObject->mesh = mesh_car;
            car->gameObject->transformation = t;

            
            essieu->add(roue1);
            essieu->add(roue2);
            essieu->add(roue3);
            essieu->add(roue4);
            car->add(essieu);
            
            car->apply();

            roue1->gameObject->mesh->print();
            roue2->gameObject->mesh->print();
            roue3->gameObject->mesh->print();
            roue4->gameObject->mesh->print();
            essieu->gameObject->mesh->print();
            car->gameObject->mesh->print();


        }

    
};
#endif

