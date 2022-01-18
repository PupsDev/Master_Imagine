#ifndef MAT4_H
#define MAT4_H

#include <cmath>
#include <cstdlib>
#include <iostream>
#include "vec3.h"
#include "vec4.h"

namespace mlm
{

    class mat4;
    class mat4
    {
        private:
            float m[4*4];
        public:
            mat4();
            mat4(float a1, float a2 , float a3,float a4,
                float b1, float b2 , float b3,float b4,
                float c1, float c2 , float c3,float c4,
                float d1, float d2 , float d3,float d4);
            // row constructor
            mat4(vec4 a, vec4 b, vec4 c,vec4 d);
            mat4(float diagonal);

            float& operator [] (unsigned int c);
            float operator [] (unsigned int c)const;

            mat4 multiply (mat4 other);       

    };
    static inline std::ostream & operator << (std::ostream & s , mat4  p) {
        s <<"mat4:\n("   <<p[0] << " " << p[1] << " " << p[2]<<" " << p[3]<<"\n"
                        <<p[4] << " " << p[5] << " " << p[6]<<" " << p[7]<<"\n"
                        <<p[8] << " " << p[9] << " " << p[10]<<" " << p[11]<<"\n"
                        <<p[12] << " " << p[13] << " " << p[14]<<" " << p[15]<<")";
        return s;
    }
    static inline mat4 operator * (mat4 left , mat4 const & right)
    {
        return left.multiply(right);
    }
    static inline mat4 operator *= (mat4& left , mat4 const & right)
    {
        return left.multiply(right);
    }

    static inline mat4 identity()
    {
        return mat4(1.f);
    }
     static inline vec4 multiply (mat4& m, vec4& other)
    {   
        return vec4(other[0] * m[0] + other[1] * m[4] + other[2] * m[8] + other[3] * m[12],
                    other[0] * m[1] + other[1] * m[5] + other[2] * m[9] + other[3] * m[13],
                    other[0] * m[2] + other[1] * m[6] + other[2] * m[10] + other[3] * m[14],
                    other[0] * m[3] + other[1] * m[7] + other[2] * m[11] + other[3] * m[15]);
    }
    static inline mat4 operator+ (mat4 m, mat4 other)
    {   
        return mat4(other[0] + m[0],other[1] + m[1],other[2] + m[2],other[3] + m[3],
                    other[4] + m[4],other[5] + m[5],other[6] + m[6],other[7] + m[7],
                    other[8] + m[8],other[9] + m[9],other[10] + m[10],other[11] + m[11],
                    other[12] + m[12],other[13] + m[13],other[14] + m[14],other[15] + m[15]);
    }
    static inline mat4 transpose(mat4 &other)
    {
        mat4 m;
        m[0]=other[0];
        m[1]=other[4];
        m[2]=other[8];
        m[3]=other[12];

        m[4]=other[1];
        m[5]=other[5];
        m[6]=other[9];
        m[7]=other[13];

        m[8]=other[2];
        m[9]=other[6];
        m[10]=other[10];
        m[11]=other[14];

        m[12]=other[3];
        m[13]=other[7];
        m[14]=other[11];
        m[15]=other[15];
        return m;
    }
    static inline mat4 orthographic (float left, float right, float bottom, float top, float near, float far)
    {
        float IRMinusL = 1./(right - left);
        float IFminusN = 1./(far - near);
        float ITMinusB = 1./(top - bottom);
        mat4 m =  mat4();
        m[0]= 2*IRMinusL;
        m[3]= -(right+left)*IRMinusL;
        m[5]= 2*ITMinusB;
        m[7]= -(top + bottom)*ITMinusB;
        m[10]= -2. *IFminusN;
        m[11]= -(far+near)*IFminusN;
        m[15]=1.;
        return m;
    }
    static inline mat4 perspective (float fov, float aspectRatio, float near, float far)
    {
        float S = tanf(fov/2.);
        float IFMinusN = 1./far-near;
        mat4 m =mat4();
        m[0] = 1./(aspectRatio*S);
        m[5] = 1./S;
        m[10] = far/(near- far);
        m[11] = -1.;
        m[14] = -2*near;
        // Like glm infinite perspective ?

        return m;

    }
    static inline mat4 lookAt(vec4 eye, vec4 target, vec4 upDir)
    {

        vec4 forward = eye - target;
        forward.normalize();                 

        vec4 left = upDir.cross(forward); 
        left.normalize();

        vec4 up =  forward.cross(left);    

        mat4 matrix = mlm::identity();

        matrix[0] = left[0];
        matrix[4] = left[1];
        matrix[8] = left[2];
        matrix[1] = up[0];
        matrix[5] = up[1];
        matrix[9] = up[2];
        matrix[2] = forward[0];
        matrix[6] = forward[1];
        matrix[10]= forward[2];

        matrix[12]= -left[0] * eye[0] - left[1] * eye[1] - left[2] * eye[2];
        matrix[13]= -up[0] * eye[0] - up[1] * eye[1] - up[2] * eye[2];
        matrix[14]= -forward[0] * eye[0] - forward[1] * eye[1] - forward[2] * eye[2];

        return matrix;
    }
    
    static inline mat4 translation (const vec3& translation)
    {
        return mat4( 1.,0.,0.,0.,
                    0.,1.,0.,0.,
                    0.,0.,1.,0.,
                    translation[0],translation[1],translation[2],1.);
    }
    static inline mat4 rotationX (float angle)
    {
        return mat4( 1.,0.,0.,0,
                    0.,cos(angle),-sin(angle),0,
                    0.,sin(angle),cos(angle),0,
                    0.,0.,0.,1);
    }
    static inline mat4 rotationY (float angle)
    {
        
        return mat4(cos(angle),0.,sin(angle),0.,
                    0.,1.,0.,0.,
                    -sin(angle),0.,cos(angle),0.,
                    0.,0.,0.,1.);
    }
    static inline mat4 rotationZ (float angle)
    {
        return mat4( cos(angle),-sin(angle),0.,0.,
                    sin(angle),cos(angle),0,0.,
                    0.,0.,1.,0.,
                    0.,0.,0.,1.);
    }

    static inline mat4 rotation (float angle, const vec3& axis)
    {
        return mat4();
    }
    static inline mat4 scale (const float& scale)
    {   
        return mat4( scale,0.,0.,0.,
                    0.,scale,0.,0.,
                    0.,0.,scale,0.,
                    0.,0.,0.,1.);        
    }
    static inline mat4 scale (const float& scale,const float& scale2,const float& scale3)
    {   
        return mat4( scale,0.,0.,0.,
                    0.,scale2,0.,0.,
                    0.,0.,scale3,0.,
                    0.,0.,0.,1.);        
    }

}


#endif