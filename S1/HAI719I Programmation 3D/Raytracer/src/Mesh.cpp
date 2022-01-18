#include "Mesh.h"

Mesh::Mesh()
{

}
Mesh::Mesh(std::vector<mlm::vec4> vertices, std::vector<size_t> indices,std::vector<mlm::vec4> normals)
{
    this->vertices = vertices;
    this->indices = indices;
    this->normals = normals;
    buildTriangles();
}

mlm::vec4 Mesh::vertexAt(size_t indice)const
{
    return this->vertices[indice];
}
bool Mesh::intersect(const Ray& r, float t_min, float t_max, Intersection& intersection) const
{
    Intersection temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

       for (size_t i = 0 ; i < triangles.size();i++) {
        Triangle  triangle =  Triangle(
                                        vertices[triangles[i]->v[0]]  ,
                                        vertices[triangles[i]->v[1]],
                                        vertices[triangles[i]->v[2]], triangles[i]->meshMaterial, normals[i/3]);
                triangle.setVertexNormales( verticesNormals[triangles[i]->v[0]],
                                    verticesNormals[triangles[i]->v[1]],
                                    verticesNormals[triangles[i]->v[2]]);
        if (triangle.intersect(r, t_min, t_max, temp_rec)) {
            hit_anything = true;
            if(temp_rec.t<closest_so_far)
            {
                closest_so_far = temp_rec.t;
                intersection = temp_rec;    
            }
        }
    }

    return hit_anything;
}
void Mesh::apply(mlm::mat4 transformation)
{
    this->transformation = transformation*this->transformation;
    for(size_t i = 0, sizeVertices = vertices.size();i<sizeVertices;i++)
    {
        vertices[i] = mlm::multiply(transformation, vertices[i]);
    
    }
    for(size_t j=0, sizeNormals = normals.size();j<sizeNormals;j++)
    {
        normals[j] = mlm::multiply(transformation, normals[j]);
        normals[j].normalize();
    }
    
    buildTriangles();
}
void Mesh::flatShaded()
{

    for(size_t i = 0 ; i < indices.size(); i+=3)
    {
        mlm::vec4 v0 = vertices[indices[i]];
        mlm::vec4 v1 = vertices[indices[i+1]]-v0;
        mlm::vec4 v2 = vertices[indices[i+2]]-v0;
        mlm::vec4 norm = mlm::cross(v1,v2);
        norm.normalize();

        this->normals[i/3]= norm; 
        
    }
    buildTriangles();
}
void Mesh::setMaterialTriangle(size_t indice, Material * material)
{
    triangles[indice]->setMaterial(material);
}
void Mesh::buildTriangles()
{
    triangles.clear();
    
    std::vector<mlm::vec4> sumOfNormals;
    std::vector<float> facePerVertice;

    for(size_t i=0; i < vertices.size();i++)
    {
        sumOfNormals.push_back(mlm::vec4(0.,0.,0.,1.));
        facePerVertice.push_back(0.);
    }
    for(size_t i = 0 ; i < indices.size(); i+=3)
    {
        float weight = 1.;
        for(size_t j =0 ; j<3;j++)
        {


            sumOfNormals[indices[i+j]]  +=  normals[i/3]*weight;
            facePerVertice[indices[i+j]]  +=weight;

        }
    }

    for(size_t i=0; i < vertices.size();i++)
    {   
        sumOfNormals[i]/=facePerVertice[i];
        sumOfNormals[i].normalize();
        sumOfNormals[i][3]=1.;
    }

    this->verticesNormals = sumOfNormals;
    
    for(size_t i = 0 ; i < indices.size(); i+=3)
    {

       MeshTriangle * mTriangle = new MeshTriangle(
                                        indices[i]  ,
                                        indices[i+1],
                                        indices[i+2], meshMaterial);
        triangles.push_back(mTriangle);

    }


}
