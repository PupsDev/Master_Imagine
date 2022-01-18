#ifndef RENDERER_H
#define RENDERER_H
#include "Camera.h"
#include "Raytracer.h"
#include <thread>
#include <atomic>
#define WIDTH 640*2


class Renderer
{
    public:
        Renderer();
        Renderer(int height, int width, Camera *camera, Scene * scene,Raytracer * raytracer);
        void setOption(int resolution,int depth, int antialiasing)
        {
            this->resolution = resolution;
            this-> depth = depth;
            this-> antialiasing = antialiasing;
        }
        void draw(GLubyte (*imageTex) [WIDTH][4]);         
        
    private:
    
    Camera *camera;
    Raytracer * raytracer;
    Scene * scene;
    mlm::vec4 *color;
    // TMP

    int height;
    int width; 
    int resolution;
    int depth;
    int antialiasing;
    

        
        
};
#endif