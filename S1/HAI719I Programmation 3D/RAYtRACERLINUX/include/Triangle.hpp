#include <vector>
#include "Vec3.h"

using namespace std;

class Triangle
{
    private:
        vector<Vec3> vertices;
        vector<Vec3> normales;
        Vec3 normal;
        Material* material;
    public:
        Triangle(Vec3 v0, Vec3 v1, Vec3 v2)
        {
            vertices.push_back(v0);
            vertices.push_back(v1);
            vertices.push_back(v2);
            
            computeNormaleFace();
            computeNormales();
            this->material = new Material();
            this->material->SetKDiffuse(0.1);
            this->material->SetKSpecular(0.1);
            this->material->SetIndiceSpecular(1.);


        }
        Material* getMaterial()const
        {
            return this->material;
        }
        void setMaterial(Material *material)
        {
            this->material=material;
        }
        void computeNormaleFace()
        {
            Vec3 v1v0 = vertices[ 1 ] - vertices[0];
            Vec3 v2v0 = vertices[ 2 ] - vertices[0];
            Vec3 cross = v1v0.cross(v2v0);
            normal=cross.normalize();
            
        }
        void computeNormales()
        {
            normales.clear();
            normales.resize(vertices.size());
            std::vector< float > diviseur;
            diviseur.resize(vertices.size());
            int k,k1;
                for( size_t j = 0 ; j< 3 ; j++)
        	    {
            	        k = (j+1)%3;
	        			k1 = (j+2)%3;
	        			
	        			// Les 2 vecteurs issus de j
	        			Vec3 vec1 = vertices[k] -vertices[j];
	        			Vec3 vec2 = vertices[k1] -vertices[j];
		
						float angleTriangle = acos(   vec1.dot(vec2) / (vec1.abs() * vec2.abs()) );
                        Vec3 vec3 = normal;
                        vec3*=angleTriangle;
	        			normales[j]+= vec3;	
	        			diviseur[j]+=angleTriangle;
                }
            for(size_t i = 0 ; i < normales.size(); i++)
            {
                normales[i]/=abs(diviseur[i]);
                normales[i].normalize();           
            }
        }
        Vec3 getNormal()
        {
            return normal;
        }
        bool intersect(const Vec3 &orig, const Vec3 &dir, float &t) const 
        {
            const float EPSILON = 0.0000001;
            Vec3 e1, e2 , h, s, q, outIntersectionPoint;
            float a,f,u,v;
            e1=vertices[1]-vertices[0];
            e2=vertices[2]-vertices[0];
            h = dir.cross(e2);
            a=e1.dot(h);
            if (a > -EPSILON && a < EPSILON)
                return false;    // Le rayon est parallèle au triangle.
            if(e1.dot(normal)>0)
                return false;
            f = 1.0/a;
            s = orig - vertices[0];
            u = f * (s.dot(h));
            if (u < 0.0 || u > 1.0)
                return false;
            q = s.cross(e1);
            v = f * dir.dot(q);
            if (v < 0.0 || u + v > 1.0)
                return false;

            t = f * e2.dot(q);
            if (t > EPSILON) // Intersection avec le rayon
            {
                outIntersectionPoint = orig + dir * t;
                return true;
            }
            else // On a bien une intersection de droite, mais pas de rayon.
                return false;

        } 

};