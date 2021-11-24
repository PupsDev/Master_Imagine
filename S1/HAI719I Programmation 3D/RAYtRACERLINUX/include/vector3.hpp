#ifndef Vec3_
#define Vec3_

#include <cmath>

///
/// Template class for three element vectors.
///
template<class T>
class Vec3
{
public:
  Vec3();
  Vec3(const T x, const T y, const T z);
  Vec3(const Vec3<T>& v);

  // utility operations
  Vec3<T>& zero();
  Vec3<T>& set(const T x, const T y, const T z);
  Vec3<T>& normalize();

  // math operations
  const T norm() const;
  const T sum() const;
  const T dot(const Vec3<T>&) const;
  const Vec3<T> cross(const Vec3<T>&) const;
  const Vec3<T> abs() const;

  // operators
  Vec3<T>& operator= (const Vec3<T>& v);        // assignment

  const T operator[] (const int i) const;             // indexing
  T& operator[] (const int i);                        // indexing

  const Vec3<T> operator-();                       // unary negate

  Vec3<T>& operator+=(const T s);                  // scalar addition
  Vec3<T>& operator-=(const T s);                  // scalar subtraction
  Vec3<T>& operator*=(const T s);                  // scalar multiplication
  Vec3<T>& operator/=(const T s);                  // scalar division

  Vec3<T>& operator+=(const Vec3<T>& v);        // vector addition
  Vec3<T>& operator-=(const Vec3<T>& v);        // vector subtraction
  Vec3<T>& operator*=(const Vec3<T>& v);        // element-wise multiplication
  Vec3<T>& operator/=(const Vec3<T>& v);        // element-wise division

  const Vec3<T> operator < (const T s);            // compare each element with s, return vector of 1 or 0 based on test
  const Vec3<T> operator > (const T s);

  const Vec3<T> operator < (const Vec3<T>& v);  // element-wise less than comparion, return vector of 1 or 0 based on test
  const Vec3<T> operator > (const Vec3<T>& v);  // element-wise greater than comparion, return vector of 1 or 0 based on test

  bool operator == (const Vec3<T>& v);             // test vector for equality
  bool operator != (const Vec3<T>& v);             // test vector for inequality

  T* ptr(){return _v;}                                // return reference to array (use with caution)

private:
  T _v[3];
};


//
// Binary operations
//

//inline const Vec3<T> operator+ (const Vec3<T>& v, const T& s);  // scalar operations
//inline const Vec3<T> operator- (const Vec3<T>& v, const T& s);
//inline Vec3<T> operator* (const Vec3<T>& v, const T& s);
//inline const Vec3<T> operator/ (const Vec3<T>& v, const T& s);

//inline const Vec3<T> operator+ (const Vec3<T>& v1, const Vec3<T>& v2);  // element-wise addition
//inline const Vec3<T> operator- (const Vec3<T>& v1, const Vec3<T>& v2);  // element-wise subtraction
//inline const Vec3<T> operator* (const Vec3<T>& v1, const Vec3<T>& v2);  // dot product
//inline const Vec3<T> operator^ (const Vec3<T>& v1, const Vec3<T>& v2);  // cross product


//
// Function definitions
//


template <class T>
Vec3<T>::Vec3()
{
  _v[0] = 0.0;
  _v[1] = 0.0;
  _v[2] = 0.0;
}


template <class T>
Vec3<T>::Vec3(const Vec3<T>& v)
{
  _v[0] = v[0];
  _v[1] = v[1];
  _v[2] = v[2];
}


template <class T>
Vec3<T>::Vec3(const T x, const T y, const T z)
{
  _v[0] = x;
  _v[1] = y;
  _v[2] = z;
}


template <class T>
Vec3<T>& Vec3<T>::zero()
{
  _v[0] = 0.0;
  _v[1] = 0.0;
  _v[2] = 0.0;
  return *this;
}


template <class T>
Vec3<T>& Vec3<T>::set(const T x, const T y, const T z)
{
  _v[0] = x;
  _v[1] = y;
  _v[2] = z;
  return *this;
}


template <class T>
inline const T Vec3<T>::operator [] (const int i) const
{
  return _v[i];
}


template <class T>
T& Vec3<T>::operator [] (const int i)
{
  return _v[i];
}


template <class T>
inline const Vec3<T> Vec3<T>::abs() const
{
  return Vec3<T>(std::abs(_v[0]), std::abs(_v[1]), std::abs(_v[2]));
}


template <class T>
inline const T Vec3<T>::sum() const
{
  return _v[0] + _v[1] + _v[2];
}


template <class T>
inline const T Vec3<T>::dot(const Vec3<T>& v) const
{
  return _v[0]*v[0] + _v[1]*v[1] + _v[2]*v[2];
}


template <class T>
inline const Vec3<T> Vec3<T>::cross(const Vec3<T>& v) const
{
  return Vec3<T>(
    (_v[1] * v[2]) - (_v[2] * v[1]),
    (_v[2] * v[0]) - (_v[0] * v[2]),
    (_v[0] * v[1]) - (_v[1] * v[0])
  );
}


template <class T>
inline const T Vec3<T>::norm() const
{
  return (T) sqrt(dot(*this)); // cast to type
}


template <class T>
Vec3<T>& Vec3<T>::normalize()
{
  T n = norm();
  if(n){
    _v[0]/=n;
    _v[1]/=n;
    _v[2]/=n;
  }
  return *this;
}

template <class T>
Vec3<T>& Vec3<T>::operator= (const Vec3<T>& v)
{
  _v[0] = v[0];
  _v[1] = v[1];
  _v[2] = v[2];
  return *this;
}


template <class T>
Vec3<T>& Vec3<T>::operator += (const Vec3<T>& v)
{
  _v[0] += v[0];
  _v[1] += v[1];
  _v[2] += v[2];
  return *this;
}


template <class T>
Vec3<T>& Vec3<T>::operator += (T v)
{
  _v[0] += v;
  _v[1] += v;
  _v[2] += v;
  return *this;
}


template <class T>
Vec3<T>& Vec3<T>::operator -= (const Vec3<T>& v)
{
  _v[0] -= v[0];
  _v[1] -= v[1];
  _v[2] -= v[2];
  return *this;
}


template <class T>
Vec3<T>& Vec3<T>::operator -= (T v)
{
  _v[0] -= v;
  _v[1] -= v;
  _v[2] -= v;
  return *this;
}


template <class T>
Vec3<T>& Vec3<T>::operator *= (T v)
{
  _v[0] *= v;
  _v[1] *= v;
  _v[2] *= v;
  return *this;
}


template <class T>
Vec3<T>& Vec3<T>::operator *= (const Vec3<T>& v)
{
  _v[0] *= v[0];
  _v[1] *= v[1];
  _v[2] *= v[2];
  return *this;
}


template <class T>
Vec3<T>& Vec3<T>::operator /= (T v)
{
  _v[0] /= v;
  _v[1] /= v;
  _v[2] /= v;
  return *this;
}


template <class T>
Vec3<T>& Vec3<T>::operator /= (const Vec3<T>& v)
{
  _v[0] /= v[0];
  _v[1] /= v[1];
  _v[2] /= v[2];
  return *this;
}


template<class T>
inline const Vec3<T> Vec3<T>::operator < (const T s)
{
  return Vec3<T>(_v[0]<s, _v[1]<s, _v[2]<s);
}


template<class T>
inline const Vec3<T> Vec3<T>::operator > (const T s)
{
  return Vec3<T>(_v[0]>s, _v[1]>s, _v[2]>s);
}


template<class T>
inline const Vec3<T> Vec3<T>::operator < (const Vec3<T>& v)
{
  return Vec3<T>(_v[0]<v[0], _v[1]<v[1], _v[2]<v[2]);
}


template<class T>
inline const Vec3<T> Vec3<T>::operator > (const Vec3<T>& v)
{
  return Vec3<T>(_v[0]>v[0], _v[1]>v[1], _v[2]>v[2]);
}


template<class T>
inline const Vec3<T> Vec3<T>::operator - ()
{
  return Vec3<T>(-_v[0], -_v[1], -_v[2]);
}


template <class T>
inline bool Vec3<T>::operator == (const Vec3<T>& v)
{
  return    _v[0] == v[0]
         && _v[1] == v[1]
         && _v[2] == v[2];
}


template <class T>
inline bool Vec3<T>::operator != (const Vec3<T>& v)
{
  return    _v[0] != v[0]
         || _v[1] != v[1]
         || _v[2] != v[2];
}



template<class T>
inline const Vec3<T> operator && (const Vec3<T>& v1, const Vec3<T>& v2)
{
  return Vec3<T>(v1[0]&&v2[0], v1[1]&&v2[1], v1[2]&&v2[2]);
}


template<class T>
inline const Vec3<T> operator || (const Vec3<T>& v1, const Vec3<T>& v2)
{
  return Vec3<T>(v1[0]||v2[0], v1[1]||v2[1], v1[2]||v2[2]);
}


template <class T>
inline const Vec3<T> operator + (const Vec3<T>& v, const T& s)
{
  return Vec3<T>(v) += s;
}


template <class T>
inline const Vec3<T> operator - (const Vec3<T>& v, const T& s)
{
  return Vec3<T>(v) -= s;
}


template <class T>
inline const Vec3<T> operator * (const Vec3<T>& v, const T& s)
{
  return Vec3<T>(v) *= s;
}


template <class T>
inline const Vec3<T> operator / (const Vec3<T>& v, const T& s)
{
  return Vec3<T>(v) /= s;
}


template <class T>
inline const Vec3<T> operator + (const Vec3<T>& v1, const Vec3<T>& v2)
{
  return Vec3<T>(v1) += v2;
}


template <class T>
inline const Vec3<T> operator - (const Vec3<T>& v1, const Vec3<T>& v2)
{
  return Vec3<T>(v1) -= v2;
}


template <class T>
inline const T operator * (const Vec3<T>& v1, const Vec3<T>& v2)
{
  return v1.dot(v2);
}


template <class T>
inline const Vec3<T> operator ^ (const Vec3<T>& v1, const Vec3<T>& v2)
{
  return v1.cross(v2);
}




#endif//Vec3_HPP_