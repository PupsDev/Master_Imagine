inline 
float clamp(const float &lo, const float &hi, const float &v) 
{ return std::max(lo, std::min(hi, v)); } 
class Raytracer
{
    private:
        Scene* scene;
        int depth;
    public:
        Raytracer(Scene* scene, int depth)
        {   
            this->scene = scene;
            this->depth = depth;

        }
        Vec3 computeHitPoint(Ray ray, float last_t)
        {
            return ray.GetOrigin() + ray.GetDirection()*last_t;
        }
        float deg2rad(const float &deg) 
        { return deg * M_PI / 180; } 
        Vec3 computeColor(Light light, Material* mat, float NL, float RR)
        {
            Vec3 colorM(0.0,0.0,0.0);

            //Ambiant
            float ambiant = 0.2*light.getIntensity();
            colorM+=mat->GetDiffuse_material()*ambiant;
            
            //Diffuse
            colorM+=mat->GetDiffuse_material()*mat->GetKDiffuse()*NL;

            //Spec
            colorM+= mat->GetSpecular_material()*(float)(light.GetIntensitySpec()*mat->GetKSpecular()*pow(RR,mat->GetIndiceSpecular()));

            return colorM;

        }
        float computeRR(Vec3 N,Vec3 L,Vec3 hitPoint)
        {
            Vec3 R = L-N;
            R.normalize();
            return fmax(R.dot(hitPoint.normalize()),0.);
        }
        float computeShadows(int nblight,Vec3 originLight, Vec3 hitpoint, Ray ray, Vec3 N, float t,float last_t,int i)
        {
            
            int shadowCounter=0;
            float vis=1.;
            for(int lighti=0; lighti<nblight ;lighti++)
            {
                for(int lightj=0; lightj<nblight ;lightj++)
                {
                    shadowCounter = computeShadowNumber(shadowCounter, lighti, lightj, nblight, originLight, hitpoint, ray, N, t,last_t, i);
                    //shadowCounter = computeShadowNumberRandom(shadowCounter, lighti, lightj, nblight, originLight, hitpoint, ray, N, t,last_t, i);
                }
            }
            float percentShadow = (float)shadowCounter / (float)(nblight*nblight);
            if(shadowCounter)
                vis=(1.- (percentShadow*0.5));
            return vis;
        }
        int computeShadowNumber(int t1, int lighti, int lightj, int nblight, Vec3 originLight, Vec3 hitpoint, Ray ray, Vec3 N, float t,float last_t, int i)
        {

            Vec3 v1=Vec3(6.0,0.0,0.0);
            Vec3 v2=Vec3(0.0,0.0,6.0);
            
            float x = (float)(lighti/(float)nblight) * v1[0] ;
            float y = (float)(lightj/(float)nblight)*v2[2];
            float z = 0.;
            Vec3 ilight= Vec3(x,y,z)+ originLight;
            Vec3 L2= ilight-hitpoint;
            L2.normalize();
            return computeShadowIntersection(L2, hitpoint, ray, N, t,last_t, t1, i);
            
        }
        int computeShadowNumberRandom(int t1, int lighti, int lightj, int nblight, Vec3 originLight, Vec3 hitpoint, Ray ray, Vec3 N, float t,float last_t, int i)
        {
            srand (time(NULL));
            float rng = (float)(rand() % 100)/100.;
            
            Vec3 v1=Vec3(6.0,0.0,0.0);
            Vec3 v2=Vec3(0.0,0.0,6.0);
            
            float x = (float)(rand() % 100)/100.* v1[0] ;
            float y = (float)(rand() % 100)/100.*v2[2];
            float z = 0.;
            Vec3 ilight= Vec3(x,y,z)+ originLight;
            Vec3 L2= ilight-hitpoint;
            L2.normalize();
            return computeShadowIntersection(L2, hitpoint, ray, N, t,last_t, t1, i);
            
        }
        int computeShadowIntersection(Vec3 L2, Vec3 hitpoint, Ray ray, Vec3 N, float t,float last_t, int t1, int i)
        {
            for(size_t j =0 ; j< scene->getObjectsCount();j++)
            {
                Object *sphere2 =  scene->getObject(j);
                if(i!=j)
                {
                
                    if(sphere2->intersect(computeHitPoint(ray, last_t)+N*(float)0.1, L2, t))
                    {
                        t1++;
                    }
                }

            }
            return t1;

        }
        Ray computeRebound(Vec3 hitpoint,Vec3 N,Vec3 originRay)
        {
            Vec3 dir2 = hitpoint-originRay;
            dir2.normalize();
            Ray r(hitpoint+N*(float)0.05,dir2);
            return r;
        }
        Vec3 computeColorTotal(Light light,Ray ray,Material *mat,Vec3 hitPoint,Vec3 N,Vec3 originLight, float t,float last_t,int i)
        {
            Vec3 color(0.,0.,0.);
            
            Vec3 L = (light.getPosition() - hitPoint);
            L.normalize();

            float NL = N.dot(L);
            float agg = 2*(NL);
            float RR = computeRR(N*agg,L,hitPoint);
            NL = fmax(NL,0.);
            
            float visiblity=1.;
            int nblight = 8;
            visiblity = computeShadows(nblight,originLight, hitPoint, ray, N, t,last_t,i);
            color = computeColor(light,mat,NL,RR);

            color*=visiblity;
            return color;
        }
        Vec3 reflect(Vec3 v, Vec3 n)
        {
            return v - 2*v.dot(n)*n;
        }
        Vec3 refract(const Vec3 &I, const Vec3 &N, const float &ior) 
        { 
            float cosi = clamp(-1, 1, I.dot(N)); 
            float etai = 1, etat = ior; 
            Vec3 n = N; 
            if (cosi < 0) { cosi = -cosi; } else { std::swap(etai, etat); n= Vec3(0.,0.,0.)-N; } 
            float eta = etai / etat; 
            float k = 1 - eta * eta * (1 - cosi * cosi); 
            return k < 0 ? Vec3(0.,0.,0.) : eta * I + (eta * cosi - sqrtf(k)) * n; 
        } 
        void fresnel(const Vec3 &I, const Vec3 &N, const float &ior, float &kr) 
        { 
            float cosi = clamp(-1, 1, I.dot(N)); 
            float etai = 1, etat = ior; 
            if (cosi > 0) {  std::swap(etai, etat); } 
            // Compute sini using Snell's law
            float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi)); 
            // Total internal reflection
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
            // As a consequence of the conservation of energy, transmittance is given by:
            // kt = 1 - kr;
        } 
        Vec3 trace(Ray ray, int depth)
        {

            Vec3 colorBackground(0.2,0.2,0.3);
            Vec3 color(0.2,0.2,0.3);

            Light light =  scene->getLight(0);
            Vec3 originLight = light.getPosition();

            float t=FLT_MAX;
            float last_t=FLT_MAX;
            
            if (depth <= 0)
                return Vec3(0.,0.,0.); 
            
           for(size_t i =0 ; i<  scene->getObjectsCount();i++)
            {
                Vec3 colorM(0.0,0.0,0.0);
                Object *sphere =  scene->getObject(i);
                    
                    if(sphere->intersect(ray.GetOrigin(), ray.GetDirection(), t))
                    {
                        if(t<last_t)
                        {    
                            last_t=t;

                            Vec3 hitPoint = computeHitPoint(ray, last_t);
                            Vec3 N = hitPoint-sphere->getPosition();
                            N.normalize();

                            int reflecti =1;

                            Material *mat = sphere->getMaterial();
                            float ior = mat->GetIndex_medium();

                            if(mat->GetType()==MaterialType::Material_Glass)
                            {
                                Vec3 reflectionDirection = reflect(ray.GetDirection(), N).normalize(); 
                                Vec3 refractionDirection = refract(ray.GetDirection(), N, ior).normalize(); 
                                Vec3 reflectionRayOrig = (reflectionDirection.dot(N) < 0) ? 
                                    hitPoint - N * 0.01 : 
                                    hitPoint + N * 0.01; 
                                Vec3 refractionRayOrig = (refractionDirection.dot(N) < 0) ? 
                                    hitPoint - N * 0.01 : 
                                    hitPoint + N * 0.01; 
                                Ray reflectRay = Ray(reflectionRayOrig,reflectionDirection);
                                Ray refractRay = Ray(refractionRayOrig,refractionDirection);

                                Vec3 reflectionColor = this->trace(reflectRay, depth-1);
                                Vec3 refractionColor = this->trace(refractRay, depth-1);
                                float kr; 
                                fresnel(ray.GetDirection(), N, ior, kr); 
                                color = reflectionColor * kr + refractionColor * (1-kr); 
                
                            }
                            else if(mat->GetType()==MaterialType::Material_Mirror)
                            {
                                float kr; 
                                fresnel(ray.GetDirection(), N, ior, kr); 
                                Vec3 reflectionDirection = reflect(ray.GetDirection(), N); 
                                Vec3 reflectionRayOrig = (reflectionDirection.dot( N) > 0) ? 
                                    hitPoint + N * 0.01 : 
                                    hitPoint - N * 0.01; 
                                Ray reflectRay = Ray(reflectionRayOrig,reflectionDirection);

                                color=this->trace(reflectRay, depth-1)*(1-kr);
            
                            }
                            else
                            {
                                color = computeColorTotal(light,ray,mat,hitPoint,N,originLight, t,last_t, i);
                    
                                Ray rebound = computeRebound(hitPoint,N,ray.GetOrigin());
                                return color+this->trace(rebound, depth-1)*(float)0.1 ;

                            }
                            
                        }
                            
                    }                
            }

            for(int iMesh =0 ; iMesh < scene->getMeshesCount() ; iMesh++)
            {
  
                Mesh mesh =  scene->getMesh(iMesh);
                
            Vec3 colorM(0.0,0.0,0.0);
            Vec3 colorS(1.0,1.0,1.0);
            for(size_t i = 0 ; i < mesh.getFacesCount();i++)
            {
                Triangle tri = mesh.getFace(i);
                if(tri.intersect(ray.GetOrigin(), ray.GetDirection(), t))
                {

                    if(t<last_t)
                    {
                        
                            last_t=t;
                            Material *mat  = tri.getMaterial();
        
                            Vec3 ray2 = ray.GetOrigin() + ray.GetDirection()*last_t;
                            Vec3 pointSphere = ray2;
                            ray2.normalize();

                            Vec3 L = (light.getPosition() - pointSphere);
                            L.normalize();
    
                            Vec3 N = tri.getNormal();


                            float NL = N.dot(L);
                            float agg = 2*(NL);
                            N*=agg;

                            Vec3 R = L-N;
                            R.normalize();

                            float RR = fmax(R.dot(ray2),0.);
                            NL = fmax(NL,0.);
                            
                            color = computeColor(light, mat, NL, RR);
                            int  nblight;
                            int t1=0;
                            float vis = 1.;
                            
                            if(light.getType()==LightType_Point)
                            {
                                nblight=1;
                                Vec3 L2= originLight-pointSphere;
                                L2.normalize();
                                for(size_t j =0 ; j< scene->getObjectsCount();j++)
                                {
                                    Object *sphere =  scene->getObject(j);
                                    if(sphere->intersect(pointSphere+N*(float)0.01, L2, t))
                                    {
                                        t1++;
                                    }
                                }

                            }
                            else
                            {

                                if(light.getType()==LightType_Random_Area)
                                {
                                nblight = 8;
                                for(int lighti=0;lighti<nblight;lighti++)
                                {
                                    for(int lightj=0;lightj<nblight;lightj++)
                                    {

                                        float x = (float)(rand() % 100)/100.* light.getV1()[0] ;
                                        float y = (float)(rand() % 100)/100.*light.getV2()[2];
                                        float z = 0.;
                                        Vec3 ilight= Vec3(x,y,z)+ originLight;
                                        Vec3 L2= ilight-pointSphere;
                                        L2.normalize();
                                        
                                        for(size_t j =0 ; j< scene->getObjectsCount();j++)
                                        {
                                            
                                            Object *sphere =  scene->getObject(j);
                                            if(sphere->intersect(pointSphere+N*(float)0.01, L2, t))
                                            {
                                                    t1++;
                                            }
                                        }
                                    }
                                }

                                }
                                else
                                {

                                nblight = 8;
                                for(int lighti=0;lighti<nblight;lighti++)
                                {
                                    for(int lightj=0;lightj<nblight;lightj++)
                                    {
                                        float x = (float)(lighti/(float)nblight) *light.getV1()[0] ;
                                        float y = (float)(lightj/(float)nblight)*light.getV2()[2];
                                        float z = 0.;
                                        Vec3 ilight= Vec3(x,y,z)+ originLight;
                                        Vec3 L2= ilight-pointSphere;
                                        L2.normalize();
                                        
                                        for(size_t j =0 ; j< scene->getObjectsCount();j++)
                                        {
                                            
                                            Object *sphere =  scene->getObject(j);
                                            if(sphere->intersect(pointSphere+N*(float)0.01, L2, t))
                                            {
                                                    t1++;
                                            }
                                        }
                                    }
                                }
                                }
                                
                            }
      
                            float p = (float)t1 / (float)(nblight*nblight);
                            if(p)
                                vis=(1.- (p*0.5));
                            
                            color*=vis;
                            Vec3 dir2 = pointSphere-ray.GetOrigin();
                            dir2.normalize();
                        
                        
                        //color = computeColorTotal(light,ray,mesh.getMaterial(),pointSphere,N,originLight, t,last_t, i);
                        //Ray rebound = computeRebound(pointSphere,N,ray.GetOrigin());
                        //return color+trace(rebound, depth-1)*(float)0.1 ;

                        Ray r(pointSphere+N*(float)0.1,dir2);
                        return color+trace(r, depth-1)*(float)0.01 ;
                    
                    }
                }
            }

            }
           
            return color;
        }
        void setScene(Scene * scene)
        {
            this->scene = scene;
        }
        void setDepth(int depth)
        {
            this->depth =depth ;
        }
        Vec3 Raytrace(Ray ray)
        {
            return trace(ray, this->depth);
        }
};