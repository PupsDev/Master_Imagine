#ifndef RAYTRACER_H
#define RAYTRACER_H
#include "Scene.h"
#include <math.h>

class Raytracer
{
    public:
        Raytracer();
        Raytracer(Scene *scene, int depth);
        glm::vec4 raytrace(Ray ray, int depth);
        float trace(Ray ray,float t);
        inline float clamp( float min, float max,float x) {
            if (x < min) return min;
            if (x > max) return max;
            return x;
        }
        glm::vec4 reflect(glm::vec4 v, glm::vec4 n)
        {
            glm::vec4 reflected =glm::vec4 (v - 2*glm::dot(v,n)*n);
            reflected[3]=0.;
            return reflected;
        }
        glm::vec4 refract(const glm::vec4 &I, const glm::vec4 &N, const float &ior) 
        { 
            float cosi = clamp(-1, 1, glm::dot(I,N)); 
            float etai = 1, etat = ior; 
            glm::vec4 n = N; 
            if (cosi < 0) { cosi = -cosi; } else { std::swap(etai, etat); n= glm::vec4(0.,0.,0.,0.)-N; } 
            float eta = etai / etat; 
            float k = 1 - eta * eta * (1 - cosi * cosi); 
            return k < 0 ? glm::vec4(0.,0.,0.,0.) : eta * I + (eta * cosi - sqrtf(k)) * n; 
        } 
        void fresnel(const glm::vec4 &I, const glm::vec4 &N, const float &ior, float &kr) 
        { 
            float cosi = clamp(-1, 1, glm::dot(I,N)); 
            float etai = 1, etat = ior; 
            if (cosi > 0) {  std::swap(etai, etat); } 

            float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi)); 

            if (sint >= 1) { 
                kr = 1; 
            } 
            else { 
                float cost = sqrtf(std::max(0.f, 1 - sint * sint)); 
                cosi = fabsf(cosi); 
                float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost)); 
                float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost)); 
                kr = (Rs * Rs + Rp * Rp) / 2; 
            } 

        } 
    private:
        Scene* scene;
        int depth;
        
        
};
#endif