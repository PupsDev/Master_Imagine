#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <string>
#include "Mesh.h"
#include "Sphere.h"
#include "Square.h"


#include <GL/glut.h>


enum LightType {
    LightType_Spherical,
    LightType_Quad
};


struct Light {
    Vec3 material;
    bool isInCamSpace;
    LightType type;

    Vec3 pos;
    float radius;

    Mesh quad;

    float powerCorrection;

    Light() : powerCorrection(1.0) {}

};

struct RaySceneIntersection{
    bool intersectionExists;
    unsigned int typeOfIntersectedObject;
    unsigned int objectIndex;
    float t;
    RayTriangleIntersection rayMeshIntersection;
    RaySphereIntersection raySphereIntersection;
    RaySquareIntersection raySquareIntersection;
    RaySceneIntersection() : intersectionExists(false) , t(FLT_MAX) {}
};



class Scene {
    std::vector< Mesh > meshes;
    std::vector< Sphere > spheres;
    std::vector< Square > squares;
    std::vector< Light > lights;

public:


    Scene() {
    }

    void draw() {
        // iterer sur l'ensemble des objets, et faire leur rendu :
        for( unsigned int It = 0 ; It < meshes.size() ; ++It ) {
            Mesh const & mesh = meshes[It];
            mesh.draw();
        }
        for( unsigned int It = 0 ; It < spheres.size() ; ++It ) {
            Sphere const & sphere = spheres[It];
            sphere.draw();
        }
        for( unsigned int It = 0 ; It < squares.size() ; ++It ) {
            Square const & square = squares[It];
            square.draw();
        }
    }




    RaySceneIntersection computeIntersection(Ray const & ray) {
        RaySceneIntersection result;
        //TODO calculer les intersections avec les objets de la scene et garder la plus proche
        /*for( unsigned int It = 0 ; It < meshes.size() ; ++It ) {
            Mesh const & mesh = meshes[It];
            mesh.draw();
        }
        */
/*
        for( unsigned int It = 0 ; It < spheres.size() ; ++It ) {
            Sphere const & sphere = spheres[It];

            RaySphereIntersection sphereIntersection = sphere.intersect(ray);


            if(sphereIntersection.t < result.t)
            {
                result.intersectionExists = sphereIntersection.intersectionExists;
                result.typeOfIntersectedObject=1;
                result.t = sphereIntersection.t;
                result.objectIndex=It;
                result.t = sphereIntersection.t;
                result.raySphereIntersection = sphereIntersection;
            }
            
        }
        */
        
        for( unsigned int It = 0 ; It < squares.size() ; ++It ) {
            Square const & square = squares[It];

            RaySquareIntersection squareIntersection = square.intersect(ray);


            if(squareIntersection.t < result.t)
            {
                //std::cout<<squareIntersection.t<<std::endl;
                result.intersectionExists = squareIntersection.intersectionExists;
                result.typeOfIntersectedObject=2;
                result.t = squareIntersection.t;
                result.objectIndex=It;
                result.raySquareIntersection = squareIntersection;
            }
        }

        return result;
    }





    Vec3 rayTraceRecursive( Ray ray , int NRemainingBounces ) {

        //TODO RaySceneIntersection raySceneIntersection = computeIntersection(ray);
        Vec3 color;
        return color;
    }


    Vec3 rayTrace( Ray const & rayStart ) {
        //TODO appeler la fonction recursive

        RaySceneIntersection raySceneIntersection = computeIntersection(rayStart);
        Vec3 color=Vec3(0.2f, 0.2f, 0.3f);

        if(raySceneIntersection.intersectionExists)
        {
            /*
            RaySphereIntersection sphereIntersection  = raySceneIntersection.raySphereIntersection;

            if(sphereIntersection.intersectionExists)
            {
                unsigned int index = raySceneIntersection.objectIndex;
                Vec3 colorMaterial = spheres[index].material.diffuse_material;
                color = sphereIntersection.t * colorMaterial;
               
            }
            */
            RaySquareIntersection squareIntersection  = raySceneIntersection.raySquareIntersection;

            if(squareIntersection.intersectionExists)
            {

                unsigned int index = raySceneIntersection.objectIndex;
                Vec3 colorMaterial = squares[index].material.diffuse_material;
                //std::cout<<colorMaterial<<std::endl;
                color =  squareIntersection.t * colorMaterial;
               
            }
         
            
        }
        
        return color;
    }

    void setup_single_sphere() {
        meshes.clear();
        spheres.clear();
        squares.clear();
        lights.clear();

        {
            lights.resize( lights.size() + 1 );
            Light & light = lights[lights.size() - 1];
            light.pos = Vec3(-5,5,5);
            light.radius = 2.5f;
            light.powerCorrection = 2.f;
            light.type = LightType_Spherical;
            light.material = Vec3(1,1,1);
            light.isInCamSpace = false;
        }
        {
            spheres.resize( spheres.size() + 1 );
            Sphere & s = spheres[spheres.size() - 1];
            s.m_center = Vec3(0. , 0. , 0.);
            s.m_radius = 1.f;
            s.build_arrays();
            s.material.type = Material_Mirror;
            s.material.diffuse_material = Vec3( 1.,1.,1 );
            s.material.specular_material = Vec3( 0.2,0.2,0.2 );
            s.material.shininess = 20;

            spheres.resize( spheres.size() + 1 );
            Sphere & s2 = spheres[spheres.size() - 1];
            s2.m_center = Vec3(-1. , 0. , -2.);
            s2.m_radius = 1.f;
            s2.build_arrays();
            s2.material.type = Material_Mirror;
            s2.material.diffuse_material = Vec3( 0.,1.,0 );
            s2.material.specular_material = Vec3( 0.2,0.2,0.2 );
            s2.material.shininess = 20;
        }
    }

    void setup_single_square() {
        meshes.clear();
        spheres.clear();
        squares.clear();
        lights.clear();

        {
            lights.resize( lights.size() + 1 );
            Light & light = lights[lights.size() - 1];
            light.pos = Vec3(-5,5,5);
            light.radius = 2.5f;
            light.powerCorrection = 2.f;
            light.type = LightType_Spherical;
            light.material = Vec3(1,1,1);
            light.isInCamSpace = false;
        }

        {
            squares.resize( squares.size() + 1 );
            Square & s = squares[squares.size() - 1];
            s.setQuad(Vec3(-1., -1., 0.), Vec3(1., 0, 0.), Vec3(0., 1, 0.), 2., 2.);
            s.build_arrays();
            s.material.diffuse_material = Vec3( 0.8,0.8,0.8 );
            s.material.specular_material = Vec3( 0.8,0.8,0.8 );
            s.material.shininess = 20;
        }
    }
};



#endif
