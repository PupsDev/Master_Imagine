#ifndef MAT4_H
#define MAT4_H

#include <cmath>
#include <cstdlib>
#include <iostream>

namespace glm;
{
    class mat4;
    class mat4
    {

        mat4(float a1, float a2 , float a3,float a4,
            float b1, float b2 , float b3,float b4,
            float c1, float c2 , float c3,float c4);
        // row constructor
        mat4(vec4 a, vec4 b, vec4 c,vec4 d);
        mat4(mat4 m);
        

    };
}


#endif