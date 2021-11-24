#include "Triangle.hpp"
#include <iostream>
class Mesh: public Object 
{
    private:
        vector<Triangle> faces;
        vector<unsigned int> vertOrder;
        vector<Vec3> vertices;

        Vec3 position;

        Vec3 color;
        vector<Vec3> colors;
        int hit;

 //r,g,b
        float kDiffuse;
        float kSpecular;
        float indiceSpecular;
    public:

        Mesh(vector<unsigned int> vertOrder,
             vector<Vec3> vertices):vertOrder(vertOrder),vertices(vertices){
                color = Vec3 (1.0,0.0,0.0);
                kDiffuse = 0.5;
                kSpecular = 0.8;
                indiceSpecular=5.;
                hit=-1;
                this->SetType(MESH);
             }
        void constructTriangles()
        {
            for(size_t i = 0 ; i < vertOrder.size(); i+=3)
            {
               
                Triangle triangle = Triangle( vertices[vertOrder[i]],
                                              vertices[vertOrder[i+1]],
                                              vertices[vertOrder[i+2]]);
                faces.push_back(triangle);
            }
        }
        void setMaterialFace(int i, Material * material)
        {
            faces[i].setMaterial(material);
        }
        size_t getFacesCount(){return faces.size();}
        Triangle getFace(size_t indice){return faces[indice];}
        bool intersect(const Vec3 &orig, const Vec3 &dir, float &t) const 
        {
            bool inter = false;
            for(size_t i = 0 ; i < faces.size();i++)
            {

                Triangle tri = faces[i];
                if(tri.intersect(orig, dir, t))
                {
                    inter=true;
                    
                }
            }

            return inter;
        }
        Vec3 getPosition(){return position;}
         Vec3 getColor(){return colors[hit];}
        void setColor(Vec3 _color) {
        color = _color;
        }
        float getDiffuse(){return kDiffuse;}
        float getSpecular(){return kSpecular;}
        float getIndiceSpecular(){return indiceSpecular;}

};