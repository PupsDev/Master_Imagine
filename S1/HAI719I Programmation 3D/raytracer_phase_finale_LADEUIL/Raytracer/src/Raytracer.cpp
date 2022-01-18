#include "Raytracer.h"
#include <random>

mlm::vec4 random_in_unit_sphere() {
    while (true) {
        static std::uniform_real_distribution<float> distr(-1., 1.);
        static std::mt19937 generator;
        mlm::vec4 p(distr(generator),distr(generator),0.,0.);
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
mlm::vec4 Raytracer::raytrace(Ray ray,int depth)
{
    if (depth <= 0)
        return mlm::vec4(0.0,0.0,0.0,1.);
    Intersection rec;
    float t_max = 1000.;
    
    if (scene->hittables.intersect(ray, 0.001, t_max, rec)) {
        mlm::vec4 finalColor(0.,0.,0.,1.);

        for(auto light : scene->lights)
        {  
            mlm::vec4 normal = rec.normal;
            normal.normalize();           
            mlm::vec4 baseColor(0.,0.,0.,1.);
            mlm::vec4 lightVec =light->getPosition()- rec.hitPoint ;
            float distanceLight = lightVec.length();
            lightVec.normalize();

            float distance = light->getRadius();
            float NL =  fmax(lightVec.dot(normal),0.);
            float agg = 2*(NL);
            
            mlm::vec4 reflectedRay =  normal*agg - lightVec;
            reflectedRay.normalize();

            float RR = fmax(reflectedRay.dot(ray.getDirection()*-1.),0.);
            float ior = 1.2;

            if(rec.material->getType()==MaterialType::Material_Glass)
            {
                
                mlm::vec4 reflectionDirection = reflect(ray.getDirection(), rec.normal).normalize(); 
                mlm::vec4 refractionDirection = refract(ray.getDirection(), rec.normal, ior).normalize(); 

                mlm::vec4 reflectionRayOrig = (reflectionDirection.dot(rec.normal) < 0) ? 
                    rec.hitPoint - rec.normal * 0.01 : 
                    rec.hitPoint + rec.normal * 0.01; 
                mlm::vec4 refractionRayOrig = (refractionDirection.dot(rec.normal) < 0) ? 
                    rec.hitPoint - rec.normal * 0.01 : 
                    rec.hitPoint + rec.normal * 0.01; 
                Ray reflectRay = Ray(reflectionRayOrig,reflectionDirection);
                Ray refractRay = Ray(refractionRayOrig,refractionDirection);

                mlm::vec4 reflectionColor = this->raytrace(reflectRay, depth-1);
                mlm::vec4 refractionColor = this->raytrace(refractRay, depth-1);
                float kr; 
                fresnel(ray.getDirection(), rec.normal, ior, kr); 
                
                baseColor= reflectionColor * kr + refractionColor * (1-kr);

            }
        else if(rec.material->getType()==MaterialType::Material_Mirror)
            {

                rec.normal.normalize();
                mlm::vec4 reflectionRayOrig = ray.getDirection() - 2* mlm::dot(ray.getDirection(),rec.normal)* rec.normal;
                reflectionRayOrig[3]=0.;
                Ray reflectRay = Ray(rec.hitPoint,reflectionRayOrig);
                Ray reflectRay2 = Ray(rec.hitPoint,reflectionRayOrig);

                return this->raytrace(reflectRay2, depth-1);
            } 
            else
            {       
                float ambiant = 0.1*light->getIntensity();
                baseColor+=mlm::vec4(rec.material->getDiffuse(),1.)*ambiant;
                baseColor+= mlm::vec4(light->getColor(),1.)*NL * light->getIntensity()/ (distanceLight);
                //Spec       
                mlm::vec4 spec = mlm::vec4(rec.material->getSpecular(),1.)*(float)(light->getSpecularIntensity()*rec.material->getSpecularK()*pow(RR,rec.material->getIndiceSpecular()));

                baseColor +=spec;
                float visibility = 1.;

                mlm::vec4 norm = rec.normal;
                norm.normalize();

            }           
            finalColor += baseColor;
        }
        float visibility = 1.;
        for(auto light : scene->lights)
        {
              
                mlm::vec4 lightVec =light->getPosition()- rec.hitPoint ;
                float distanceLight = lightVec.length();


                size_t sample = light->getSample();

                if(sample==1)
                {
                    mlm::vec4 shadowVec =light->getPosition()-rec.hitPoint;
                    shadowVec.normalize();

                    Ray shadowRay = Ray( rec.hitPoint, shadowVec);
                    visibility =std::min(visibility,trace(shadowRay,distanceLight));
                }
                else
                {
                    srand (time(NULL));
                    mlm::vec4 ilight;
                    mlm::vec4 v1=light->getVec(1);
                    mlm::vec4 v2=light->getVec(2);
                    float z = light->getPosition()[2];
                    float xl = light->getPosition()[0];
                    float yl =light->getPosition()[1];
                
                    float shadowCounter = 0.;
                    for(size_t lighti=0; lighti<sample ;lighti++)
                    {
                        float y = (float)(rand() % 100)/100.*v2[1] +yl;
                        for(size_t lightj=0; lightj<sample ;lightj++)
                        {
                        
                            float x = (float)(rand() % 100)/100.* v1[0]+ xl;
                            
                            mlm::vec4 ilight= mlm::vec4(x,y,z,1.);
                            mlm::vec4 shadowVec =ilight-rec.hitPoint;
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
        finalColor+=0.1*(mlm::vec4(rec.material->getDiffuse(),1.)*0.1 +0.1*mlm::vec4(1.,1.,1.,1.));  
        return finalColor;
    }
    
    //sky
    mlm::vec4 unit_direction = ray.getDirection();
    auto t = 0.5*(unit_direction[1] + 1.0);
    return (1.0-t)*mlm::vec4(1.0, 1.0, 1.0,1.) + t*mlm::vec4(0.5, 0.7, 1.0,1.);

}