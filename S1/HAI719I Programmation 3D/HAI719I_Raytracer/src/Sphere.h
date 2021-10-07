#ifndef Sphere_H
#define Sphere_H
#include "Vec3.h"
#include <vector>
#include "Mesh.h"
#include <cmath>

struct RaySphereIntersection{
    bool intersectionExists;
    float t;
    float theta,phi;
    unsigned int sIndex;
    Vec3 intersection;
    Vec3 secondintersection;
    Vec3 normal;
};

static
Vec3 SphericalCoordinatesToEuclidean( Vec3 ThetaPhiR ) {
    return ThetaPhiR[2] * Vec3( cos(ThetaPhiR[0]) * cos(ThetaPhiR[1]) , sin(ThetaPhiR[0]) * cos(ThetaPhiR[1]) , sin(ThetaPhiR[1]) );
}
static
Vec3 SphericalCoordinatesToEuclidean( float theta , float phi ) {
    return Vec3( cos(theta) * cos(phi) , sin(theta) * cos(phi) , sin(phi) );
}

static
Vec3 EuclideanCoordinatesToSpherical( Vec3 xyz ) {
    float R = xyz.length();
    float phi = asin( xyz[2] / R );
    float theta = atan2( xyz[1] , xyz[0] );
    return Vec3( theta , phi , R );
}



class Sphere : public Mesh {
public:
    Vec3 m_center;
    float m_radius;

    Sphere() : Mesh() {}
    Sphere(Vec3 c , float r) : Mesh() , m_center(c) , m_radius(r) {}

    void build_arrays(){
        unsigned int nTheta = 20 , nPhi = 20;
        positions_array.resize(3 * nTheta * nPhi );
        normalsArray.resize(3 * nTheta * nPhi );
        uvs_array.resize(2 * nTheta * nPhi );
        for( unsigned int thetaIt = 0 ; thetaIt < nTheta ; ++thetaIt ) {
            float u = (float)(thetaIt) / (float)(nTheta-1);
            float theta = u * 2 * M_PI;
            for( unsigned int phiIt = 0 ; phiIt < nPhi ; ++phiIt ) {
                unsigned int vertexIndex = thetaIt + phiIt * nTheta;
                float v = (float)(phiIt) / (float)(nPhi-1);
                float phi = - M_PI/2.0 + v * M_PI;
                Vec3 xyz = SphericalCoordinatesToEuclidean( theta , phi );
                positions_array[ 3 * vertexIndex + 0 ] = m_center[0] + m_radius * xyz[0];
                positions_array[ 3 * vertexIndex + 1 ] = m_center[1] + m_radius * xyz[1];
                positions_array[ 3 * vertexIndex + 2 ] = m_center[2] + m_radius * xyz[2];
                normalsArray[ 3 * vertexIndex + 0 ] = xyz[0];
                normalsArray[ 3 * vertexIndex + 1 ] = xyz[1];
                normalsArray[ 3 * vertexIndex + 2 ] = xyz[2];
                uvs_array[ 2 * vertexIndex + 0 ] = u;
                uvs_array[ 2 * vertexIndex + 1 ] = v;
            }
        }
        triangles_array.clear();
        for( unsigned int thetaIt = 0 ; thetaIt < nTheta - 1 ; ++thetaIt ) {
            for( unsigned int phiIt = 0 ; phiIt < nPhi - 1 ; ++phiIt ) {
                unsigned int vertexuv = thetaIt + phiIt * nTheta;
                unsigned int vertexUv = thetaIt + 1 + phiIt * nTheta;
                unsigned int vertexuV = thetaIt + (phiIt+1) * nTheta;
                unsigned int vertexUV = thetaIt + 1 + (phiIt+1) * nTheta;
                triangles_array.push_back( vertexuv );
                triangles_array.push_back( vertexUv );
                triangles_array.push_back( vertexUV );
                triangles_array.push_back( vertexuv );
                triangles_array.push_back( vertexUV );
                triangles_array.push_back( vertexuV );
            }
        }
    }

    bool solveQuadratic(float a, float b, float c, float &x0, float &x1) const
    {
        float delta = b*b - 4*a*c;
        float q;
        float tmp;

        if(delta < 0)
            return false;
        else if(delta ==0)
        {
            x0=x1=-0.5*b/a;
        }
        else
        {
            if (b>0)
            {
                q = -0.5 * (b+sqrt(delta));
            }
            else
            {   
                q = -0.5 * (b-sqrt(delta));
            }   
            x0=q/a;
            x1=c/q;
            if(x0>x1)
            {
                tmp = x1;
                x1 = x0;
                x0=tmp;
            }
        }
        return true;
        

    }
    RaySphereIntersection intersect(const Ray &ray) const {
        RaySphereIntersection intersection;
        //TODO calcul l'intersection rayon sphere

        float t0,t1;

        Vec3 L = ray.origin()-m_center; 
        float a = Vec3::dot(ray.direction(),ray.direction());
        float b = 2* Vec3::dot(L,ray.direction());
        float c = Vec3::dot(L,L) - m_radius*m_radius;

        if( solveQuadratic(a,b,c, t0, t1))
        {
            if( t0 < 0 )
                t0 = t1;
            if(t0>0) // t1 > 0
            {
                Vec3 pHit = ray.origin() + ray.direction()*t0;
                Vec3 normHit = pHit-m_center;
                normHit.normalize();

                intersection.intersectionExists = true;
                intersection.t = t0;
                intersection.intersection = pHit;
                intersection.normal = normHit;
                
            }
            else{
                intersection.intersectionExists = false;
            }



        }
        else{
                intersection.intersectionExists = false;
            }
        return intersection;
    }
};
#endif
