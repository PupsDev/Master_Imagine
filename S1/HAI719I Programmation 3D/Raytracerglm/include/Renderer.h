#ifndef RENDERER_H
#define RENDERER_H
#include "Camera.h"
#include "Raytracer.h"
#include <thread>
#include <atomic>
#define WIDTH 640


class Renderer
{
    public:
        Renderer();
        Renderer(int height, int width, Camera *camera, Scene * scene,Raytracer * raytracer);
        void draw(GLubyte (*imageTex) [WIDTH][4]);         
        
    private:
    
    Camera *camera;
    Raytracer * raytracer;
    Scene * scene;
    std::atomic<int> jobsDone{ 0 };
    std::atomic<int> nextJob{ 0 };
    glm::vec4 *color;
    // TMP

    int height;
    int width; 
    

        
        
};
#endif