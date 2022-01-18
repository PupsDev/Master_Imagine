#include "Raytracer.h"
#include <random>

glm::vec4 random_in_unit_sphere() {
    while (true) {
        static std::uniform_real_distribution<float> distr(-1., 1.);
        static std::mt19937 generator;
        glm::vec4 p(distr(generator),distr(generator),0.,0.);
        if (p.length()*p.length() >= 1) continue;
        return p;
    }
}
Raytracer::Raytracer()
{
    
}
Raytracer::Raytracer(Scene *scene, int depth)
{
    this->scene = scene;
    this->depth = depth ;
}
float Raytracer::trace(Ray ray, float t)
{
    Intersection rec;
    float t_max = 1000.;
    float closest_so_far=t_max;
    float visibility = 1.;
    bool hit=false;
    if (scene->hittables.intersect(ray, 0.001, t_max, rec)) {

        if(rec.t<t && rec.material->getType()!=MaterialType::Material_Glass)
        {
            visibility=0.;
        }
    }
    return visibility;
}
glm::vec4 Raytracer::raytrace(Ray ray,int depth)
{
    if (depth <= 0)
        return glm::vec4(0.0,0.0,0.0,1.);
    Intersection rec;
    float t_max = 1000.;
    
    if (scene->hittables.intersect(ray, 0.001, t_max, rec)) {
        glm::vec4 finalColor(0.,0.,0.,1.);

        for(auto light : scene->lights)
        {  
            glm::vec4 normal = rec.normal;
            normal.normalize();           
            glm::vec4 baseColor(0.,0.,0.,1.);
            glm::vec4 lightVec =light->getPosition()- rec.hitPoint ;
            float distanceLight = lightVec.length();
            lightVec.normalize();

            float distance = light->getRadius();
            float NL =  fmax(lightVec.dot(normal),0.);
            float agg = 2*(NL);
            
            glm::vec4 reflectedRay =  normal*agg - lightVec;
            reflectedRay.normalize();

            float RR = fmax(reflectedRay.dot(ray.getDirection()*-1.),0.);
            float ior = 1.2;

            if(rec.material->getType()==MaterialType::Material_Glass)
            {
                
                glm::vec4 reflectionDirection = reflect(ray.getDirection(), rec.normal).normalize(); 
                glm::vec4 refractionDirection = refract(ray.getDirection(), rec.normal, ior).normalize(); 

                glm::vec4 reflectionRayOrig = (reflectionDirection.dot(rec.normal) < 0) ? 
                    rec.hitPoint - rec.normal * 0.01 : 
                    rec.hitPoint + rec.normal * 0.01; 
                glm::vec4 refractionRayOrig = (refractionDirection.dot(rec.normal) < 0) ? 
                    rec.hitPoint - rec.normal * 0.01 : 
                    rec.hitPoint + rec.normal * 0.01; 
                Ray reflectRay = Ray(reflectionRayOrig,reflectionDirection);
                Ray refractRay = Ray(refractionRayOrig,refractionDirection);

                glm::vec4 reflectionColor = this->raytrace(reflectRay, depth-1);
                glm::vec4 refractionColor = this->raytrace(refractRay, depth-1);
                float kr; 
                fresnel(ray.getDirection(), rec.normal, ior, kr); 
                
                baseColor= reflectionColor * kr + refractionColor * (1-kr);

            }
        else if(rec.material->getType()==MaterialType::Material_Mirror)
            {
                rec.normal.normalize();
                glm::vec4 reflectionRayOrig = ray.getDirection() - 2* glm::dot(ray.getDirection(),rec.normal)* rec.normal;
                reflectionRayOrig[3]=0.;
                Ray reflectRay = Ray(rec.hitPoint,reflectionRayOrig);
                Ray reflectRay2 = Ray(rec.hitPoint,reflectionRayOrig);

                return this->raytrace(reflectRay2, depth-1);
            } 
            else
            {       
                float ambiant = 0.1;//*light->getIntensity();
                baseColor+=glm::vec4(rec.material->getDiffuse(),1.)*ambiant;
                baseColor+= glm::vec4(light->getColor(),1.)*NL * light->getIntensity()/ (distanceLight);
                //Spec       
                glm::vec4 spec = glm::vec4(rec.material->getSpecular(),1.)*(float)(light->getSpecularIntensity()*rec.material->getSpecularK()*pow(RR,rec.material->getIndiceSpecular()));

                baseColor +=spec;
                float visibility = 1.;

                glm::vec4 norm = rec.normal;
                norm.normalize();
                /*
                float rng = (float)(rand() % 100)/100.;
                glm::vec4 ilight;
                glm::vec4 v1=glm::vec4(6.0,0.0,0.0,0.);
                glm::vec4 v2=glm::vec4(0.0,6.0,.0,0.);
                float z = light->getPosition()[2];
                float xl = light->getPosition()[0];
                float yl =light->getPosition()[1];
                
                float shadowCounter = 0.;
                size_t sample = light->getSample();
                sample = 1;
                srand (time(NULL));
                for(size_t lighti=0; lighti<sample ;lighti++)
                {
                    for(size_t lightj=0; lightj<sample ;lightj++)
                    {
                       
                        float x = (float)(rand() % 100)/100.* v1[0]+ xl;
                        float y = (float)(rand() % 100)/100.*v2[1] +yl;
                        glm::vec4 ilight= glm::vec4(x,y,z,1.);
                         glm::vec4 shadowVec =ilight-rec.hitPoint;
                         shadowVec.normalize();
                        Ray shadowRay = Ray(rec.hitPoint, shadowVec);
                        shadowCounter += (1.-trace(shadowRay,distanceLight));
                
                
            
                        
                    }

                }
                float percentShadow = shadowCounter / (float)(sample*sample);
                visibility = 1.-percentShadow;
                if(sample==1)
                {
                    glm::vec4 shadowVec =light->getPosition()-rec.hitPoint;
                    shadowVec.normalize();
                    Ray shadowRay = Ray(rec.hitPoint+shadowVec*0.01, shadowVec);
                    visibility = trace(shadowRay,distanceLight);
                }
                baseColor = baseColor *visibility;
                baseColor+=(1.-visibility)/2.*(glm::vec4(rec.material->getDiffuse(),1.)*ambiant*0.1 +ambiant*0.1*glm::vec4(light->getColor(),1.));
                */

            }           
            finalColor += baseColor;
        }
        float visibility = 1.;
        for(auto light : scene->lights)
        {
              
                glm::vec4 lightVec =light->getPosition()- rec.hitPoint ;
                float distanceLight = lightVec.length();
                float rng = (float)(rand() % 100)/100.;
                glm::vec4 ilight;
                glm::vec4 v1=glm::vec4(1.0,0.0,0.0,0.);
                glm::vec4 v2=glm::vec4(0.0,1.0,.0,0.);
                float z = light->getPosition()[2];
                float xl = light->getPosition()[0];
                float yl =light->getPosition()[1];
                
                float shadowCounter = 0.;
                size_t sample = light->getSample();
                //sample = 2;
                if(sample==1)
                {
                    glm::vec4 shadowVec =light->getPosition()-rec.hitPoint;
                    shadowVec.normalize();

                    Ray shadowRay = Ray( rec.hitPoint, shadowVec);
                    visibility =std::min(visibility,trace(shadowRay,distanceLight));
                }
                else
                {
                    srand (time(NULL));
                    for(size_t lighti=0; lighti<sample ;lighti++)
                    {
                        float y = (float)(rand() % 100)/100.*v2[1] +yl;
                        for(size_t lightj=0; lightj<sample ;lightj++)
                        {
                        
                            float x = (float)(rand() % 100)/100.* v1[0]+ xl;
                            
                            glm::vec4 ilight= glm::vec4(x,y,z,1.);
                            glm::vec4 shadowVec =ilight-rec.hitPoint;
                            shadowVec.normalize();
                            Ray shadowRay = Ray(rec.hitPoint, shadowVec);
                            shadowCounter += (1.-trace(shadowRay,distanceLight));
                    
                    
                
                            
                        }

                    }
                    float percentShadow = shadowCounter / (float)(sample*sample);
                    visibility = 1.-percentShadow;

                    
                }

        }
        finalColor = finalColor *(visibility);
        finalColor+=0.1*(glm::vec4(rec.material->getDiffuse(),1.)*0.1 +0.1*glm::vec4(1.,1.,1.,1.));  
        return finalColor;
    }
    
    //sky
    glm::vec4 unit_direction = ray.getDirection();
    auto t = 0.5*(unit_direction[1] + 1.0);
    return (1.0-t)*glm::vec4(1.0, 1.0, 1.0,1.) + t*glm::vec4(0.5, 0.7, 1.0,1.);

}