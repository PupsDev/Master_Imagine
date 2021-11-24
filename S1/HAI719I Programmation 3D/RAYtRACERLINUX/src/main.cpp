
#include <cmath>
#include <cfloat>
#include <algorithm>
#include <memory>
#include <math.h>
#include <vector>
#include <time.h>

#define M_PI 3.14159265358979323846
#include <GL/glut.h>


#include "Vec3.h"
#include "Ray.hpp"
#include "Camera.hpp"
#include "Option.hpp"
#include "Configuration.hpp"
#include "Material.hpp"
#include "Scene.hpp"
#include "Raytracer.hpp"
#include "Render.hpp"


using namespace std;

std::vector<std::vector <Vec3>> image;
    Vec3 origin(0.,0.,0.);
    Vec3 dir(0.,0.,-1.);
    //Vec3 originSphere(0.0,0.0,-2.5);
    Vec3 originLight(-1.,3.0,2.0);
    Vec3 originSphere(3.0,0.0,-12.0);
    Vec3 originSphere2(0.0,1.0,-12.0);
    //Vec3 originLight(0.,8.0,-10.0);
    //Vec3 originLight(-6.0,1.0,.0);
    Vec3 dirCamera=dir;
    

    Configuration *myconfig= new Configuration();
    Ray *raycam = new Ray(origin,dir);
     Ray* newRayCam;
    Scene * scene = new Scene();
    Camera * camera = new Camera(raycam);
    Render* render = new Render(myconfig->GetOption(), camera, scene);

    //bool realtime = true;
    bool realtime;
void drawImageRealTime(std::vector<std::vector <Vec3>> image)
{
    glPointSize(8.);
    glBegin(GL_POINTS);
    

    size_t bloc = 1;

    size_t h = image.size();
    float h2 = (float)h/2.;

    size_t w = image[0].size();
    float w2 = (float)w/2.;

    
    
    for(size_t i = 0 ; i < h;i++)
    {
        
        float y = ((float)(2*i+1)/ h)-1.;
        float y2 = ((float)(2*i+1) / h)-1.;
        
        for(size_t j = 0 ; j < w;j++)
        {
                //i = (h-1-i);
                float x = ((float)(2*j+1) / w)-1.;
                float x2 =(((float)(2*j+2)/w)-1);
                
                Vec3 color = image[(h-1-i)][j];

                glColor3f(color[0],color[1],color[2]);

                if( j%2==1)
                    glColor3f(image[(h-1-i)][j-1][0],image[(h-1-i)][j-1][1],image[(h-1-i)][j-1][2]);
                if((h-1-i)%2==1)
                    glColor3f(image[(h-i)-1][j-1][0],image[(h-i)-1][j-1][1],image[(h-i)-1][j-1][2]);

                glVertex3f(x, y, 0.5);
                glVertex3f(x2, y2, 0.5);

                //glVertex3f(x+x, y, 0.5);

        }


    }
    glEnd();
}
void drawImage(std::vector<std::vector <Vec3>> image)
{
    glPointSize(1.);
    glBegin(GL_POINTS);
    
    size_t h = image.size();
    float h2 = (float)h/2.;

    size_t w = image[0].size();
    float w2 = (float)w/2.;

    
    for(size_t i = 0 ; i < h;i++)
    {
        float y = ((float)i/ h2)-1.;
        //y*=-1;

        for(size_t j = 0 ; j < w;j++)
        {
                float x = ((float)j / w2)-1.;

                Vec3 color = image[(h-1-i)][j];

                glColor3f(color[0],color[1],color[2]);

                glVertex3f(x, y, 0.5);

        }


    }
    glEnd();

}
void display() {

  // Set every pixel in the frame buffer to the current clear color.
  glClear(GL_COLOR_BUFFER_BIT);

        render->draw(origin);
        image = render->getRender();
        drawImage(image);        
    

  // Flush drawing command buffer to make drawing happen as soon as possible.
  glFlush();
  glutSwapBuffers ();
}
void displayRealtime() {
      // Set every pixel in the frame buffer to the current clear color.
  glClear(GL_COLOR_BUFFER_BIT);
        render->drawRealtime(origin);
        image = render->getRender();
        drawImageRealTime(image);
          // Flush drawing command buffer to make drawing happen as soon as possible.
  glFlush();
  glutSwapBuffers ();
}

void key (unsigned char keyPressed, int x, int y) {
    Vec3 pos , dir;
    switch (keyPressed) {
    case 'q':
    case 27:
        exit (0);
        break;
    case 'r':
        //rays.clear();
        //ray_trace_from_camera();
        break;
    case '8':
        origin += Vec3(0.,0.,.5);
        if(realtime)
            displayRealtime();
        else
            display();
        break;
    case '2':
        origin -= Vec3(0.,0.,0.5);
        if(realtime)
            displayRealtime();
        else
            display();
        break;
    case '4':
        origin += Vec3(0.5,0.,0.);
        if(realtime)
            displayRealtime();
        else
            display();
        break;
    case '6':
        origin -= Vec3(0.5,0.,0.);
        if(realtime)
            displayRealtime();
        else
            display();
        break;

    default:
    displayRealtime();
        break;

    }

}
Mesh makeCornell();
Mesh makeCube(Vec3 pos, float size);
int main(int argc , char *argv[]) {
    
  
    //Light light(originLight, 1.,LightType_Point);
    Light light(originLight, 2.0,LightType_Regular_Area);
    light.SetIntensitySpec(2.);
    Vec3 v1=Vec3(6.0,0.0,0.0);
    Vec3 v2=Vec3(0.0,0.0,6.0);
    light.setV1(v1);
    light.setV2(v2);

    Sphere *sphere= new Sphere(originSphere, 4.0);
    Sphere *sphere2= new Sphere(originSphere2, 2.0);
    Sphere *sphere3= new Sphere(originSphere2+Vec3(-1.,1.,-1.), 2.0);
    Sphere *sphere4= new Sphere(originSphere2+Vec3(-3.,2.,2.), 3.0);
 

    Material* basic_mat= new Material();
    Material* basic_mat_Red_Spec= new Material();
    basic_mat_Red_Spec->SetSpecular_material(Vec3(1.,0.,0.));

    Material* basic_red= new Material();
    basic_red->SetDiffuse_material(Vec3(1.,1.,0.));
    
    Material* basic_green= new Material();
    basic_green->SetDiffuse_material(Vec3(0.,1.,0.));
    basic_green->SetType(Material_Mirror);
    basic_green->SetIndex_medium(1.5);

    Material* basic_blue= new Material();
    basic_blue->SetDiffuse_material(Vec3(0.,0.,1.));
    basic_blue->SetType(Material_Glass);
    basic_blue->SetIndex_medium(1.5);

    Material* basic_white= new Material();
    basic_white->SetDiffuse_material(Vec3(1.,0.,1.));
    

    Mesh mesh = makeCornell();
    /*Mesh mesh2 = makeCube(Vec3(-.5,-1.,-2.),0.5);
    mesh.setMaterial(basic_mat);
    mesh2.setMaterial(basic_red);
    
    mesh2.setMaterialFace(0,basic_red);
    mesh2.setMaterialFace(1,basic_red);

    mesh2.setMaterialFace(2,basic_green);
    mesh2.setMaterialFace(3,basic_green);

    mesh2.setMaterialFace(4,basic_blue);
    mesh2.setMaterialFace(5,basic_blue);*/


    basic_red->SetDiffuse_material(Vec3(1.,0.,0.));

    sphere->setMaterial(basic_green);
    sphere2->setMaterial(basic_blue);
    sphere3->setMaterial(basic_white);
    sphere4->setMaterial(basic_red);

    scene->addLight(light);

    scene->addObject(sphere);  
    scene->addObject(sphere2); 
    scene->addObject(sphere3); 
    scene->addObject(sphere4); 
    
    scene->addMesh(mesh);
    //scene->addMesh(mesh2); 

    Configuration *myconfig2=myconfig;

    if(argc>1)
    {
        myconfig2= new Configuration(argv[1]);
        render->setConfig(myconfig2->GetOption());

        realtime = myconfig2->GetOption()->realtime;

    }
    //render->drawRealtime(origin);

    if(realtime)
    {
        render->drawRealtime(origin);
    }
    else
        render->draw(origin);
    image = render->getRender();
    render->saveImage("outputN.ppm");

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutInitWindowPosition(80, 80);
    
    if(realtime)
    {
        glutInitWindowSize(2*myconfig2->GetOption()->width, 2*myconfig2->GetOption()->height);
        glutDisplayFunc(displayRealtime);
    }
    else
    {
        glutInitWindowSize(myconfig2->GetOption()->width, myconfig2->GetOption()->height);
        glutDisplayFunc(display);
    }
    glutCreateWindow("A Simple Raytracing");
    
    glutKeyboardFunc (key);

    key ('?', 0, 0);

    glutMainLoop();

    return 0; 
}
Mesh makeCube(Vec3 pos, float size)
{
    vector<Vec3> vertices;
    vector<unsigned int> vertOrder;
    
    vertices.clear();
    vertices.resize(8);

    float side = (float) size;
    vertices[0] = pos;
    vertices[1] = pos + Vec3(side,0.,0.);
    vertices[2] = pos + Vec3(side,0.,side);
    vertices[3] = pos + Vec3(0.,0.,side);

    vertices[4] = vertices[0] +  Vec3(0.,side,0.);
    vertices[5] = vertices[1] +  Vec3(0.,side,0.);
    vertices[6] = vertices[2] +  Vec3(0.,side,0.);
    vertices[7] = vertices[3] +  Vec3(0.,side,0.);

    //DOWN
    
    vertOrder.push_back(1);
    vertOrder.push_back(0);
    vertOrder.push_back(3);
   
    vertOrder.push_back(3);
    vertOrder.push_back(2);
    vertOrder.push_back(1);
    
    /*
    vertOrder.push_back(3);
    vertOrder.push_back(2);
    vertOrder.push_back(0);
   
    vertOrder.push_back(2);
    vertOrder.push_back(1);
    vertOrder.push_back(0);
    */

    //LEFT
    /*vertOrder.push_back(0);
    vertOrder.push_back(3);
    vertOrder.push_back(4);*/

    vertOrder.push_back(4);
    vertOrder.push_back(3);
    vertOrder.push_back(0);

    vertOrder.push_back(3);
    vertOrder.push_back(4);
    vertOrder.push_back(7);

    //FAR
    vertOrder.push_back(3);
    vertOrder.push_back(2);
    vertOrder.push_back(6);

    vertOrder.push_back(3);
    vertOrder.push_back(6);
    vertOrder.push_back(7);
    
    //TOP
    vertOrder.push_back(4);
    vertOrder.push_back(5);
    vertOrder.push_back(7);

    vertOrder.push_back(5);
    vertOrder.push_back(6);
    vertOrder.push_back(7);

    //FRONT
    vertOrder.push_back(4);
    vertOrder.push_back(0);
    vertOrder.push_back(5);

    vertOrder.push_back(5);
    vertOrder.push_back(0);
    vertOrder.push_back(1);

    //RIGHT
    vertOrder.push_back(1);
    vertOrder.push_back(2);
    vertOrder.push_back(6);

    vertOrder.push_back(1);
    vertOrder.push_back(3);
    vertOrder.push_back(6);
    


    Mesh mesh(vertOrder,vertices);
    mesh.constructTriangles();
    return mesh;


}
Mesh makeCornell()
{
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
        Material* basic_red= new Material();
    basic_red->SetDiffuse_material(Vec3(1.,0.,0.));
    Material* basic_green= new Material();
    basic_green->SetDiffuse_material(Vec3(0.,1.,0.));

    mesh.setMaterialFace(0,basic_red);
    mesh.setMaterialFace(1,basic_red);
    mesh.setMaterialFace(6,basic_green);
    mesh.setMaterialFace(7,basic_green);



    return mesh;

}