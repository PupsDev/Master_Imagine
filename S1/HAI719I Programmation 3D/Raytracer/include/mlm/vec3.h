#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>


namespace mlm
{

    class vec3;

    class vec3 {
        private:
            float d[3];
        public:
            vec3();
            vec3(float x , float y , float z);
    
            vec3(const vec3& other);

            float& operator [] (unsigned int c);
            float operator [] (unsigned int c)const;
            vec3 operator = (vec3 const  other)
            {
                d[0]=other[0];
                d[1]=other[1];
                d[2]=other[2];
      
                return (*this);
            }
            // vector operations
            vec3& add (const vec3 &other);
            vec3& substract(const vec3 &other);

            // constant operations
            vec3& add (const float &other);
            vec3& substract(const float &other);
            vec3& multiply(const float &other);
            vec3& divide(const float &other);

            //comparaison operation


            float dot(vec3 other);

            float length();
            vec3& normalize();

            vec3& cross(vec3  other );

    };
    static inline std::ostream & operator << (std::ostream & s , vec3  p) {
        s <<"vec3:( " <<p[0] << " " << p[1] << " " << p[2]<<")";
        return s;
    }
    // self addition
    static inline vec3 operator += (vec3& left , vec3 const & right)
    {
        return left.add(right);
    }

    static inline vec3 operator += (vec3& left , float const & right)
    {
        return left.add(right);
    }

    // self substraction
    static inline vec3 operator -= (vec3& left , vec3 const & right)
    {
        return left.substract(right);
    }

    static inline vec3 operator -= (vec3& left , float const & right)
    {
        return left.substract(right);
    }


    // self multiplication
    static inline vec3 operator *=(vec3& left , float const & right)
    {
        return left.multiply(right);
    }

    // self division
    static inline vec3 operator /= (vec3& left , float const & right)
    {
        return left.divide(right);
    }

    // addition
    static inline vec3 operator + (vec3 left , vec3 const & right)
    {
  
        return left.add(right);
    }

    // addition float
    static inline vec3 operator +( float const & left,vec3 right)
    {
        
        return right.add(left);
    }

    static inline vec3 operator + (vec3 left , float const & right)
    {
  
        return left.add(right);
    }


    // substraction
    static inline vec3 operator - (vec3 left , vec3 const & right)
    {
  
        return left.substract(right);
    }
    // substraction float
    static inline vec3 operator -( float const & left,vec3 right)
    {
        
        return right.substract(left);
    }

    static inline vec3 operator - (vec3 left , float const & right)
    {
  
        return left.substract(right);
    }


    // multiplication
    static inline vec3 operator *(vec3 left , float const & right)
    {
  
        return left.multiply(right);
    }
    static inline vec3 operator *( float const & left,vec3 right)
    {
        
        return right.multiply(left);
    }
   
    // division
    static inline vec3 operator / (vec3 left , float const & right)
    {
  
        return left.divide(right);
    }


    inline float dot( vec3& left ,vec3 const & right)
    {
        
        return left.dot(right);
    }

    // compare
    static inline bool operator==(const vec3& left, const vec3& right)
    {
    return left[0] == right[0] && left[1]==right[1] && left[2]==right[2];
    }
    static inline bool operator!=(const vec3& left,const vec3& right)
    {
    return !(left == right);
    }

}

#endif
