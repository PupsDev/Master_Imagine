#include "mlm/mat4.h"
#include <xmmintrin.h>

using namespace mlm;

mat4::mat4()
{
    for(size_t i = 0; i < 16 ; i ++)
        this->m[i]=0.f;

}
mat4::mat4(float a1, float a2 , float a3,float a4,
    float b1, float b2 , float b3,float b4,
    float c1, float c2 , float c3,float c4,
     float d1, float d2 , float d3,float d4)
{
    this->m[0]=a1;
    this->m[1]=a2;
    this->m[2]=a3;
    this->m[3]=a4;

    this->m[4]=b1;
    this->m[5]=b2;
    this->m[6]=b3;
    this->m[7]=b4;

    this->m[8]=c1;
    this->m[9]=c2;
    this->m[10]=c3;
    this->m[11]=c4;

    this->m[12]=d1;
    this->m[13]=d2;
    this->m[14]=d3;
    this->m[15]=d4;

}

mat4::mat4(vec4 a, vec4 b, vec4 c,vec4 d)
{

}
mat4::mat4(float diagonal)
{
    this->m[0]=diagonal;
    this->m[1]=0.f;
    this->m[2]=0.f;
    this->m[3]=0.f;

    this->m[4]=0.f;
    this->m[5]=diagonal;
    this->m[6]=0.f;
    this->m[7]=0.f;

    this->m[8]=0.f;
    this->m[9]=0.f;
    this->m[10]=diagonal;
    this->m[11]=0.f;

    this->m[12]=0.f;
    this->m[13]=0.f;
    this->m[14]=0.f;
    this->m[15]=diagonal;

}
float & mat4::operator [] (unsigned int c)
{
    return m[c];
}
float mat4::operator [] (unsigned int c) const
{
    return m[c];
}

mat4 mat4:: multiply (mat4 other)
{
    __m128 row1 = _mm_load_ps(&other[0]);
    __m128 row2 = _mm_load_ps(&other[4]);
    __m128 row3 = _mm_load_ps(&other[8]);
    __m128 row4 = _mm_load_ps(&other[12]);
    for(int i=0; i<4; i++) {
        __m128 brod1 = _mm_set1_ps(m[4*i + 0]);
        __m128 brod2 = _mm_set1_ps(m[4*i + 1]);
        __m128 brod3 = _mm_set1_ps(m[4*i + 2]);
        __m128 brod4 = _mm_set1_ps(m[4*i + 3]);
        __m128 row = _mm_add_ps(
                    _mm_add_ps(
                        _mm_mul_ps(brod1, row1),
                        _mm_mul_ps(brod2, row2)),
                    _mm_add_ps(
                        _mm_mul_ps(brod3, row3),
                        _mm_mul_ps(brod4, row4)));
        _mm_store_ps(&m[4*i], row);
    }
    return (*this);
}
