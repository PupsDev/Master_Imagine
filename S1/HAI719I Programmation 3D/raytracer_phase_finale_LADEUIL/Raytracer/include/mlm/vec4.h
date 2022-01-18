#ifndef VEC4_H
#define VEC4_H

#include <cmath>
#include <iostream>
#include <cassert>
#include "mlm/vec3.h"
#include <tuple>

namespace mlm
{

    class vec4;

    class vec4 {
        private:
            float d[4];
        public:
            vec4();
            vec4(float x , float y , float z , float w);
            vec4(mlm::vec3 v , float w);
            vec4(const vec4& other);

            float& operator [] (unsigned int c);
            float operator [] (unsigned int c)const;
            vec4 operator = (vec4 const  other)
            {
                d[0]=other[0];
                d[1]=other[1];
                d[2]=other[2];
                d[3]=other[3];
      
                return (*this);
            }
            // vector operations
            vec4& add (const vec4 &other);
            vec4& substract(const vec4 &other);

            // constant operations
            vec4& add (const float &other);
            vec4& substract(const float &other);
            vec4& multiply(const float &other);
            vec4& divide(const float &other);

            //comparaison operation


            float dot(vec4 other);

            float lengthSquared();
            float length();
            vec4& normalize();

            vec4& cross(vec4  other );

    };
    static inline std::ostream & operator << (std::ostream & s , vec4  p) {
        s <<"vec4:( " <<p[0] << " " << p[1] << " " << p[2]<< " " << p[3]<<")";
        return s;
    }

    // self addition
    static inline vec4 operator += (vec4& left , vec4 const & right)
    {
        return left.add(right);
    }

    static inline vec4 operator += (vec4& left , float const & right)
    {
        return left.add(right);
    }

    // self substraction
    static inline vec4 operator -= (vec4& left , vec4 const & right)
    {
        return left.substract(right);
    }

    static inline vec4 operator -= (vec4& left , float const & right)
    {
        return left.substract(right);
    }


    // self multiplication
    static inline vec4 operator *=(vec4& left , float const & right)
    {
        return left.multiply(right);
    }

    // self division
    static inline vec4 operator /= (vec4& left , float const & right)
    {
        return left.divide(right);
    }

    // addition
    static inline vec4 operator + (vec4 left , vec4 const & right)
    {
  
        return left.add(right);
    }

    // addition float
    static inline vec4 operator +( float const & left,vec4 right)
    {
        
        return right.add(left);
    }

    static inline vec4 operator + (vec4 left , float const & right)
    {
  
        return left.add(right);
    }


    // substraction
    static inline vec4 operator - (vec4 left , vec4 const & right)
    {
  
        return left.substract(right);
    }
    // substraction float
    static inline vec4 operator -( float const & left,vec4 right)
    {
        
        return right.substract(left);
    }

    static inline vec4 operator - (vec4 left , float const & right)
    {
  
        return left.substract(right);
    }


    // multiplication
    static inline vec4 operator *(vec4 left , float const & right)
    {
  
        return left.multiply(right);
    }
    static inline vec4 operator *( float const & left,vec4 right)
    {
        
        return right.multiply(left);
    }
   
    // division
    static inline vec4 operator / (vec4 left , float const & right)
    {
  
        return left.divide(right);
    }


    inline float dot( vec4 left ,vec4  right)
    {
        
        return left.dot(right);
    }

    // compare
    static inline bool operator==(const vec4& left, const vec4& right)
    {
    return left[0] == right[0] && left[1]==right[1] && left[2]==right[2] && left[3]==right[3];
    }
    static inline bool operator<(const vec4& left, const vec4& right)
    {
      return std::make_tuple(left[0], left[1], left[2]) < std::make_tuple(right[0], right[1], right[2]);
    }
    static inline bool operator!=(const vec4& left,const vec4& right)
    {
    return !(left == right);
    }

    static inline vec4 cross(vec4 left, vec4  right)
    {
        vec4 res(
        (left[1]*right[2]) - (left[2]*right[1]),
        (left[2]*right[0]) - (left[0]*right[2]),
        (left[0]*right[1]) - (left[1]*right[0]),
        0.);
        return res;
    }


}

#endif
