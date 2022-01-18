#include "mlm/vec3.h"

using namespace mlm;

vec3::vec3()
{
    this->d[0] = 0.f;
    this->d[1] = 0.f;
    this->d[2] = 0.f;
}
vec3::vec3(float x , float y , float z )
{
    this->d[0] = x;
    this->d[1] = y;
    this->d[2] = z;

}
float & vec3::operator [] (unsigned int c)
{
    return d[c];
}
float vec3::operator [] (unsigned int c) const
{
    return d[c];
}
vec3::vec3(const vec3& other)
{
    this->d[0]  = other[0];
    this->d[1]  = other[1];
    this->d[2]  = other[2];

}

 vec3& vec3::add (const vec3 &other)
 {
    this->d[0]  += other[0];
    this->d[1]  += other[1];
    this->d[2]  += other[2];

    return *this;
 }
  vec3& vec3::substract (const vec3 &other)
 {
    this->d[0]  -= other[0];
    this->d[1]  -= other[1];
    this->d[2]  -= other[2];

    return *this;
 }
  vec3& vec3::add (const float &other)
 {
    this->d[0]  += other;
    this->d[1]  += other;
    this->d[2]  += other;

    return *this;
 }
  vec3& vec3::substract (const float &other)
 {
    this->d[0]  -= other;
    this->d[1]  -= other;
    this->d[2]  -= other;

    return *this;
 }
vec3& vec3::multiply (const float &other)
 {
    this->d[0]  *= other;
    this->d[1]  *= other;
    this->d[2]  *= other;

    return *this;
 }
vec3& vec3::divide (const float &other)
 {
    this->d[0]  /= other;
    this->d[1]  /= other;
    this->d[2]  /= other;

    return *this;
 }

 float vec3::dot(vec3 other)
 {
      return this->d[0]*other[0] + this->d[1]*other[1] + this->d[2]*other[2]+this->d[3]*other[3];
 }
 
 float vec3::length()
 {
     return sqrt(this->dot(*this));
 }
 vec3& vec3::normalize()
 {
    float length = this->length();
    this->d[0]  /= length;
    this->d[1]  /= length;
    this->d[2]  /= length;

    return *this;
 }
  vec3& vec3::cross(vec3  other)
 {
     this->d[0] = (this->d[1]*other[2]) - (this->d[2]*other[1]);
     this->d[1] = (this->d[2]*other[0]) - (this->d[0]*other[2]);
     this->d[2] = (this->d[0]*other[1]) - (this->d[1]*other[0]);
    return *this;
 }