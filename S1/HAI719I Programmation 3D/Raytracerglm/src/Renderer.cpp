#include "Renderer.h"
#include "glm/mat4x4.hpp"
#include <random>


inline float random_double() {
    static std::uniform_real_distribution<float> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}
inline float clamp(float x, float min, float max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}
Renderer::Renderer()
{   

}
Renderer::Renderer(int height, int width, Camera *camera, Scene * scene, Raytracer * raytracer)
{
    this->height = height;
    this->width = width;
    this->raytracer = raytracer;
    this->camera = camera;
    this->color = new glm::vec4[height*width];

}
void  Renderer::draw( GLubyte (*imageTex) [WIDTH][4])
{   
    glm::vec4 color;

    glm::vec4 origin = camera->getPosition();
    glm::mat4 matRot = camera->getRotation();

    int sample = 2;
    const int samples_per_pixel = 1;
    const int maxDepth = 3;
    
    float vfov = 60.;
    auto theta = vfov* M_PI / 180.;
    auto h = tan(theta/2);
    auto viewport_height  = 2.0 * h;
    auto viewport_width  = 16./9. * viewport_height ;
    
    glm::vec4 lookat = glm::vec4(0.,0.,-1.,0.);
    lookat = glm::multiply(matRot,lookat);
    lookat += origin;
    glm::vec4 vup = glm::vec4(0.,1.,0.,0.);
    auto w = origin - lookat;
    w.normalize();
    auto u = glm::cross(vup, w);
    u.normalize();
    auto v = glm::cross(w, u);

    auto horizontal = viewport_width  * u;
    auto vertical = viewport_height  * v;
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - w;
    

    for(int i =0 ; i< height ; i+=sample)
    {
        int lineIndice = i*width;
        for(int j = 0 ; j < width ; j+=sample)
        {
            
            int indice = lineIndice + j;
            color = glm::vec4(0, 0, 0,0.);
            for (int s = 0; s < samples_per_pixel; ++s) {

                auto u = (j + random_double()) / (width-1);
                auto v = (i + random_double()) / (height-1);

                Ray ray = Ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);
                color+=raytracer->raytrace(ray, maxDepth);
            }
            auto scale = 1.0 / samples_per_pixel;
            color*=scale;
            
            for(int k =0;k<sample;k++)
            {       
                for(int k2 =0;k2<sample;k2++)
                {

                    imageTex[i+k][j+k2][0] = (GLubyte) (clamp(sqrt(color[0]), 0.0, 0.999)*255);
                    imageTex[i+k][j+k2][1] = (GLubyte) (clamp(sqrt(color[1]), 0.0, 0.999)*255);
                    imageTex[i+k][j+k2][2] = (GLubyte) (clamp(sqrt(color[2]), 0.0, 0.999)*255);
                    imageTex[i+k][j+k2][3] = (GLubyte) 255;   
                }
            }

        }
        
    }  


}
