#ifndef TRANSFORM
#define TRANSFORM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include <glm/gtx/rotate_vector.hpp>
#include "glm/ext.hpp"
#include "glm/gtx/string_cast.hpp"
#include <iostream>

class Transform
{
    public:
        float s; // scale
        glm::mat3 m; // orientation
        glm::mat3 m2; // rotation
        glm::vec3 translation; // translate

        Transform()
        {
            this->s =1.;
            this->m = glm::mat3(1.);
            this->translation = glm::vec3(0.);
        }
        Transform(glm::vec3 translation)
        {
            this->s =1.;
            this->m = glm::mat3(1.);
            this->translation = translation;
        }
        Transform(glm::mat3 m)
        {
            this->s =1.;
            this->m = m;
            this->translation = glm::vec3(0.);                    
        }
        Transform(glm::mat3 m,glm::vec3 translation)
        {
            this->s =1.;
            this->m = m;
            this->translation = translation;    
            
            
        }
         Transform(float s,glm::mat3 m,glm::vec3 translation)
         {
            this->s =s;
            this->m = m;
            this->translation = translation;
            
             
         }
         void print()
         {
             std::cout<<"scale: "<<"\n";
             std::cout<<s<<"\n";
             std::cout<<"matrice: "<<"\n";
            for(int i = 0 ; i < 3;i++)
            {
                for(int j=0;j<3;j++)
                {
                    std::cout<<glm::value_ptr(m)[i*3+j]<<" ";
                }
                std::cout<<"\n";

            }
            std::cout<<"translate: "<<"\n";
     
            std::cout<<glm::to_string(translation)<<" ";
            

         }
        static glm::mat3 convertMat4(glm::mat4 mat)
                {
            const float *matrix = glm::value_ptr(mat);
            float matrix2[9];
            for(int i = 0 ; i < 3;i++)
            {
                for(int j=0;j<3;j++)
                {
                    matrix2[i*3+j]=matrix[i*4+j];
                }

            }
            return glm::make_mat3(matrix2);
        }

        glm::vec3 applyToPoint(glm::vec3 p)
        {
            return m * (s*p) +(1.f/s)*translation;
        }
        
        glm::vec3 applyToVector(glm::vec3 v)
        {
            return m * (s*v);
        }
        Transform combine_with(Transform & t, float k)
        {
            Transform result;
            result.s = this->s*k + t.s*(1-k);
            return result;
        }
        void inverse()
        {
            this->s = 1.f;
            this->m = glm::inverse(m);
            this->translation += -1.f*this->translation;

        }
        /*Transform interpolate_with(Transform & t, float k)
        {
        }*/
};

/*static glm::mat3 Transform::convertMat4(glm::mat4 mat)
        {
            const float *matrix = glm::value_ptr(mat);
            float matrix2[9];
            for(int i = 0 ; i < 3;i++)
            {
                for(int j=0;j<3;j++)
                {
                    matrix2[i*3+j]=matrix[i*4+j];
                }

            }
            return glm::make_mat3(matrix2);
        }*/
#endif