#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "Vec3.h"
#include "Ray.h"
#include "Plane.h"

struct RayTriangleIntersection{
    bool intersectionExists;
    float t;
    float w0,w1,w2;
    unsigned int tIndex;
    Vec3 intersection;
    Vec3 normal;
};

class Triangle {
private:
    Vec3 m_c[3] , m_normal;
    float area;
public:
    Triangle() {}
    Triangle( Vec3 const & c0 , Vec3 const & c1 , Vec3 const & c2 ) {
        m_c[0] = c0;
        m_c[1] = c1;
        m_c[2] = c2;
        updateAreaAndNormal();
    }
    void updateAreaAndNormal() {
        Vec3 nNotNormalized = Vec3::cross( m_c[1] - m_c[0] , m_c[2] - m_c[0] );
        float norm = nNotNormalized.length();
        m_normal = nNotNormalized / norm;
        area = norm / 2.f;
    }
    void setC0( Vec3 const & c0 ) { m_c[0] = c0; } // remember to update the area and normal afterwards!
    void setC1( Vec3 const & c1 ) { m_c[1] = c1; } // remember to update the area and normal afterwards!
    void setC2( Vec3 const & c2 ) { m_c[2] = c2; } // remember to update the area and normal afterwards!
    Vec3 const & normal() const { return m_normal; }
    Vec3 projectOnSupportPlane( Vec3 const & p ) const {
        Vec3 result;
        Plane supportPlane = Plane(m_c[0],m_normal);
        result = supportPlane.project(p);
        return result;
    }
    float squareDistanceToSupportPlane( Vec3 const & p ) const {
        float result;
        Plane supportPlane = Plane(m_c[0],m_normal);
        result = supportPlane.squareDistance(p);
        return result;
    }
    float distanceToSupportPlane( Vec3 const & p ) const { return sqrt( squareDistanceToSupportPlane(p) ); }
    bool isParallelTo( Line const & L ) const {
        bool result;
        Plane supportPlane = Plane(m_c[0],m_normal);
        result = supportPlane.isParallelTo(L);
        return result;
    }
    Vec3 getIntersectionPointWithSupportPlane( Line const & L ) const {
        // you should check first that the line is not parallel to the plane!
        Vec3 result;
        Plane supportPlane = Plane(m_c[0],m_normal);
        result = supportPlane.getIntersectionPoint(p);
        return result;
    }
    void computeBarycentricCoordinates( Vec3 const & p , float & u0 , float & u1 , float & u2 ) const {
       // The area of a triangle is 
        // 0 A 1 B 2 C
      float area = Vec3::dot( m_normal, Vec3::cross( (m_c[1] - m_c[0]), (m_c[2] - m_c[0]) )  ) ;
      float areaPBC = Vec3::dot( m_normal, Vec3::cross( (m_c[1] - p), (m_c[2] - p) )  ) ;
      float areaPCA = Vec3::dot( m_normal, Vec3::cross( (m_c[2] - p), (m_c[0] - p) )  ) ;

      u0 = areaPBC / area ; // alpha
      u1 = areaPCA / area ; // beta
      u2 = 1.0f - u0 - u1 ; // gamma

    }

    RayTriangleIntersection getIntersection( Ray const & ray ) const {
        RayTriangleIntersection result;
        // 1) check that the ray is not parallel to the triangle:

        // 2) check that the triangle is "in front of" the ray:

        // 3) check that the intersection point is inside the triangle:
        // CONVENTION: compute u,v such that p = w0*c0 + w1*c1 + w2*c2, check that 0 <= w0,w1,w2 <= 1

        // 4) Finally, if all conditions were met, then there is an intersection! :

        Vec3 intersection = getIntersectionPointWithSupportPlane(ray);
        float t = (intersection - ray.origin())/ray.direction();
        if(t>0) // On face le triangle
        {
            
        }



        return result;
    }
};
#endif
