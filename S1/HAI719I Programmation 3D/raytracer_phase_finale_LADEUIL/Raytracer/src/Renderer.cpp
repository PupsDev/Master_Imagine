#include "Renderer.h"
#include "mlm/mat4.h"
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
    this->color = new mlm::vec4[height*width];
    this->resolution = 1;
    this-> depth = 3;
    this-> antialiasing = 1;

}
void  Renderer::draw( GLubyte (*imageTex) [WIDTH][4])
{   
    mlm::vec4 color;

    mlm::vec4 origin = camera->getPosition();
    mlm::mat4 matRot = camera->getRotation();

    int sample = resolution;
    const int samples_per_pixel = antialiasing;
    const int maxDepth = depth;
    
    float vfov = 60.;
    auto theta = vfov* M_PI / 180.;
    auto h = tan(theta/2);
    auto viewport_height  = 2.0 * h;
    auto viewport_width  = 16./9. * viewport_height ;
    
    mlm::vec4 lookat = mlm::vec4(0.,0.,-1.,0.);
    lookat = mlm::multiply(matRot,lookat);
    lookat += origin;
    mlm::vec4 vup = mlm::vec4(0.,1.,0.,0.);
    auto w = origin - lookat;
    w.normalize();
    auto u = mlm::cross(vup, w);
    u.normalize();
    auto v = mlm::cross(w, u);

    auto horizontal = viewport_width  * u;
    auto vertical = viewport_height  * v;
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - w;
    

    for(int i =0 ; i< height ; i+=sample)
    {
        int lineIndice = i*width;
        for(int j = 0 ; j < width ; j+=sample)
        {
            
            int indice = lineIndice + j;
            color = mlm::vec4(0, 0, 0,0.);
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
