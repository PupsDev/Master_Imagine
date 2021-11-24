#include <vector>
#include "Object.hpp"
#include "Sphere.hpp"
#include "Mesh.hpp"
#include "Light.hpp"


class Scene
{
    private:
    std::vector<Sphere> spheres;
    std::vector<Triangle> triangles;
    std::vector<Mesh> meshes;
    std::vector<Light> lights;

    std::vector<Object*> objects;
    public:
        Scene(){}
        void addSphere(Sphere sphere)
        {
            spheres.push_back(sphere);
        }
        void addTriangle(Triangle triangle)
        {
            triangles.push_back(triangle);
        }
        void addLight(Light light)
        {
            lights.push_back(light);
        }
        void addMesh(Mesh mesh)
        {
            meshes.push_back(mesh);
        }
        Mesh getMesh(size_t indice){return meshes[indice];}
        Sphere getSphere(size_t indice){return spheres[indice];}
        Triangle getTriangle(size_t indice){return triangles[indice];}
        Light getLight(size_t indice){return lights[indice];}

        void addObject(Object* object)
        {
            objects.push_back(object);
        }
        Object* getObject(size_t indice){return objects[indice];}

        size_t getMeshesCount(){return meshes.size();}
        size_t getObjectsCount(){return objects.size();}

};