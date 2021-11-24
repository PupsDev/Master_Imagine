
#include <cmath>
#include <cfloat>

#include "useful.hpp"
#include <algorithm>
#include <memory>
#include "vector3.hpp"

#include "Scene.hpp"
#include "Ray.hpp"
#include "Render.hpp"


using namespace std;

//make && bin\main.exe &&  output.ppm

typedef Vec3 Vec3;
class mWindow
{
    private:
    float width;
    float height;
    public:
        mWindow(float width, float height):width(width),height(height){}


};

void write(string filename, string data);
template <typename T>
ostream& operator << (ostream& out, vector3<T> v);

void write(string filename, string data)
{
    ofstream myfile;
    myfile.open (filename);

    myfile << data<<endl;

    
    myfile.close();

}
template <typename T>
ostream& operator << (ostream& out, vector3<T> v)
{
  out << "[" << v[0] << " " << v[1] << " " << v[2] << "]" << endl;
  return out;
}
void printV(Vec3 v)
{
    for(int i =0 ; i < 3;i++)
        cout<<i<<":"<<v[i]<<" > ";
    cout<<endl;
}
int main() {
    Vec3 origin(0.0,0.0,0.0);
    Vec3 cameraDirection(0.0,0.0,-1.0);

    Option option;


    Vec3 originSphere(3.0,0.0,-12.0);
    Vec3 originSphere2(-4.,0.0,-12.0);
    //Vec3 originSphere3(0.,1.0,-15.0);
    
    
    //Vec3 originSphere(3.0,-4.0,-12.0);
    /*Vec3 originSphere2(-4.,-4.0,-10.0);*/
   Vec3 originSphere3(0.,-4.0,-14.0);
    
     Vec3 originSphere4(0.,1.0,-14.0);

    //Vec3 originLight(-6.0,1.0,-.0);
    Vec3 originLight(0.,8.0,-14.0);

    Vec3 nearClipV1(0.0,1.0,-option.nearClip);
    Vec3 nearClipV2(1.0,0.0,-option.nearClip);

    Vec3 nearClipPoint(0.0,0.0,-option.nearClip);

    Camera camera(origin,cameraDirection);
    camera.SetNearClip(option.nearClip);
    camera.SetFarClip(option.farClip);
    camera.SetAngleCamera(option.angleCamera);
/*
    Sphere sphere(originSphere, 2.0);
    Sphere sphere2(originSphere2, 2.0);
    Sphere sphere3(originSphere3, 5.0);
    sphere2.setColor(Vec3(0.,1.,0.));
    sphere3.setColor(Vec3(100./255,149./255.,237./255));
    */
    Light light(originLight, 2.0);
    /*
    Vec3 v1=Vec3(1.0,1.0,1.0);
    Vec3 v2=Vec3(2.0,1.0,1.0);
    AreaLight light(originLight, 2.0,v1,v2);
    light.SetIntensitySpec(0.75);*/


    vector<Vec3> vertices;
    vector<unsigned int> vertOrder;

    vertices.clear();
    vertices.resize(8);


    vertices[0] = Vec3(0.-10., -7., -20.);
    vertices[1] = Vec3(1.-10. , -7., -7.);
    vertices[2] = Vec3(1.-10. , 7., -7.);
    vertices[3] = Vec3(0.-10., 7., -20.);

    vertices[4] = Vec3(0.+10., -7., -20.);
    vertices[5] = Vec3(0.+10. , 7., -20.);
    vertices[6] = Vec3(1.+10. , -7., -7.);
    vertices[7] = Vec3(1.+10., 7., -7.);

    vertOrder.push_back(1);
    vertOrder.push_back(0);
    vertOrder.push_back(2);
    
    vertOrder.push_back(2);
    vertOrder.push_back(0);
    vertOrder.push_back(3);

    vertOrder.push_back(0);
    vertOrder.push_back(4);
    vertOrder.push_back(3);

    vertOrder.push_back(1);
    vertOrder.push_back(4);
    vertOrder.push_back(0);

    vertOrder.push_back(4);
    vertOrder.push_back(5);
    vertOrder.push_back(3);

    vertOrder.push_back(1);
    vertOrder.push_back(6);
    vertOrder.push_back(4);

    vertOrder.push_back(4);
    vertOrder.push_back(6);
    vertOrder.push_back(7);

    vertOrder.push_back(4);
    vertOrder.push_back(7);
    vertOrder.push_back(5);

    //up
    vertOrder.push_back(2);
    vertOrder.push_back(5);
    vertOrder.push_back(3);

    vertOrder.push_back(2);
    vertOrder.push_back(7);
    vertOrder.push_back(5);
    
    //Mesh* mesh = new Mesh(vertOrder,vertices);
    //mesh->constructTriangles();

    Mesh mesh(vertOrder,vertices);
    mesh.constructTriangles();
    //Scene scene0;
    /*
    scene0.addSphere(sphere);
    scene0.addSphere(sphere2);
    scene0.addSphere(sphere3);
    //scene0.addTriangle(t);
    scene0.addMesh(mesh);
    scene0.addLight(light);
    Sphere * sphereptr = new Sphere(originSphere, 2.0);
    scene0.addObject(sphereptr);
    scene0.addObject(sphere2);
    scene0.addObject(sphere3);
    scene0.addObject(mesh);
    scene0.addObject(light);*/

    vector<Vec3> verticesA;
    vector<unsigned int> vertOrderA;
    verticesA.clear();
    verticesA.resize(4);
    Vec3 Apos =originLight;
    Vec3 v1=Vec3(6.,0.0,0.0);
    Vec3 v2=Vec3(0.0,0.,6.0);

    verticesA[0] = originLight;
    verticesA[1] = originLight+v1;
    verticesA[2] = originLight+v2;
    verticesA[3] = originLight+v2+v1;

    vertOrderA.push_back(1);
    vertOrderA.push_back(0);
    vertOrderA.push_back(2);

    vertOrderA.push_back(1);
    vertOrderA.push_back(2);
    vertOrderA.push_back(3);

    Mesh meshA(vertOrderA,verticesA);
    meshA.constructTriangles();
    
    Sphere *sphere= new Sphere(originSphere, 4.0);
    Sphere *sphere2= new Sphere(originSphere2, 6.0);
    Sphere *sphere3= new Sphere(originSphere3, 4.0);
    Sphere *sphere4= new Sphere(originSphere4, 4.0);
    sphere2->setColor(Vec3(0.,1.,0.));
    sphere3->setColor(Vec3(100./255,149./255.,237./255));
     sphere4->setColor(Vec3(100./255,149./255.,237./255));
    //Light *light=new Light(originLight, 2.0);
    //light->SetIntensitySpec(0.25);

    Scene scene0;
     scene0.addObject(sphere);
     scene0.addObject(sphere2);
     scene0.addObject(sphere3);
     scene0.addObject(sphere4);
     scene0.addMesh(mesh);
     scene0.addMesh(meshA);
     scene0.addLight(light);
    

    
    mWindow window(option.width,option.height);
    Vec3 pos = originLight;
    Render render(scene0,option);
    //render.Raytrace(origin);
    Vec3 o = origin+Vec3(-1.,0.,0.);
    write("out.ppm",render.Raytrace(o));
   
    /*for(int i = 0 ; i< 10; i++)
    {
        Scene scene0;
        scene0.addObject(sphere);
        scene0.addObject(sphere2);
        scene0.addObject(sphere3);
        scene0.addMesh(mesh);

        Light light(pos += Vec3(1.0,0.,0.), 2.0);
        light.SetIntensitySpec(0.75);
        scene0.addLight(light);
        
        Render render(scene0,option);
        
        string out = "output";
        out +=char(i+'0');
        out+=".ppm";
        cout<<out<<endl;
        //write(out,"coucou");
        write(out,render.Raytrace(origin));

    }*/

    //system("pause");
    

    return 0;
}
