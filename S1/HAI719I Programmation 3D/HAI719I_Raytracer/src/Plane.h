#ifndef PLANE_H
#define PLANE_H
#include "Vec3.h"
#include "Line.h"
class Plane {
private:
    Vec3 m_center , m_normal;
public:
    Plane() {}
    Plane( Vec3 const & c , Vec3 const & n ) {
        m_center = c;
        m_normal = n; m_normal.normalize();
    }
    void setCenter( Vec3 const & c ) { m_center = c; }
    void setNormal( Vec3 const & n ) { m_normal = n; m_normal.normalize(); }
    Vec3 const & center() const { return m_center; }
    Vec3 const & normal() const { return m_normal; }
    Vec3 project( Vec3 const & p ) const {
        Vec3 result;
        Vec3 pn = p+m_normal;

        if( Vec3::dot(pn,m_normal) == pn.length()*m_normal.length()) // même sens donc normal pointe vers p
        {
            result = getIntersectionPoint(Line(p,Vec3(0.,0.,0.)-m_normal));
        }
        else
        {
            result = getIntersectionPoint(Line(p,m_normal));
        }

        
        return result;
    }
    float squareDistance( Vec3 const & p ) const { return (project(p) - p).squareLength(); }
    float distance( Vec3 const & p ) const { return sqrt( squareDistance(p) ); }
    bool isParallelTo( Line const & L ) const {
        float dot = Vec3::dot(m_normal,L.direction());

        return dot<1e-6;
    }
    Vec3 getIntersectionPoint( Line const & L ) const {
        // you should check first that the line is not parallel to the plane!
        Vec3 result;
        
        if(! isParallelTo(L))
        {
            Vec3 co = m_center - L.origin();
            float dot = Vec3::dot(m_normal,L.direction());
            float t = Vec3::dot(co,m_normal)/dot;
            result = t*L.direction()+L.origin();

        }
        return result;
    }
};
#endif
