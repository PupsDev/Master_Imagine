#include "mlm/vec4.h"


using namespace mlm;

vec4::vec4()
{
    this->d[0] = 0.f;
    this->d[1] = 0.f;
    this->d[2] = 0.f;
    this->d[3] = 0.f;
}
vec4::vec4(mlm::vec3 v , float w)
{
    this->d[0] = v[0];
    this->d[1] = v[1];
    this->d[2] = v[2];
    this->d[3] = w;
}
vec4::vec4(float x , float y , float z , float w)
{
    this->d[0] = x;
    this->d[1] = y;
    this->d[2] = z;
    this->d[3] = w;
}
float & vec4::operator [] (unsigned int c)
{
    return d[c];
}
float vec4::operator [] (unsigned int c) const
{
    return d[c];
}
vec4::vec4(const vec4& other)
{
    this->d[0]  = other[0];
    this->d[1]  = other[1];
    this->d[2]  = other[2];
    this->d[3]  = other[3];
}

 vec4& vec4::add (const vec4 &other)
 {
    this->d[0]  += other[0];
    this->d[1]  += other[1];
    this->d[2]  += other[2];
    this->d[3]  += other[3];
    return *this;
 }
  vec4& vec4::substract (const vec4 &other)
 {
    this->d[0]  -= other[0];
    this->d[1]  -= other[1];
    this->d[2]  -= other[2];
    this->d[3]  -= other[3];
    return *this;
 }
  vec4& vec4::add (const float &other)
 {
    this->d[0]  += other;
    this->d[1]  += other;
    this->d[2]  += other;
    this->d[3]  += other;
    return *this;
 }
  vec4& vec4::substract (const float &other)
 {
    this->d[0]  -= other;
    this->d[1]  -= other;
    this->d[2]  -= other;
    this->d[3]  -= other;
    return *this;
 }
vec4& vec4::multiply (const float &other)
 {
    this->d[0]  *= other;
    this->d[1]  *= other;
    this->d[2]  *= other;

    return *this;
 }
vec4& vec4::divide (const float &other)
 {
    this->d[0]  /= other;
    this->d[1]  /= other;
    this->d[2]  /= other;

    return *this;
 }

 float vec4::dot(vec4 other)
 {
      return this->d[0]*other[0] + this->d[1]*other[1] + this->d[2]*other[2]+this->d[3]*other[3];
 }
 /*
 float vec4::dotSIMD(vec4 other)
 {
      __m128 row1 = _mm_load_ps(&other[0]);
      //_mm_fmadd_ps(  ) 
 }*/

float vec4::lengthSquared()
 {
     return this->dot(*this);
 }
 float vec4::length()
 {
     return sqrt(this->dot(*this));
 }
 vec4& vec4::normalize()
 {
    float length = this->length();
    this->d[0]  /= length;
    this->d[1]  /= length;
    this->d[2]  /= length;
    return *this;
 }
  vec4& vec4::cross(vec4  other)
 {
     this->d[0] = (this->d[1]*other[2]) - (this->d[2]*other[1]);
     this->d[1] = (this->d[2]*other[0]) - (this->d[0]*other[2]);
     this->d[2] = (this->d[0]*other[1]) - (this->d[1]*other[0]);
     this->d[3] = 0;
    return *this;
 }
