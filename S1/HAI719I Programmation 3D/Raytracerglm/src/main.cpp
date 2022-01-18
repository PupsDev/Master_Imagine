#include <iostream>
#include <cmath>
#include <cfloat>
#include <algorithm>
#include <memory>
#include <math.h>
#include <vector>
#include <ctime>
#include <time.h>
#include <chrono>

#include <GL/glew.h>
#include <GL/glut.h>
#include "Light.h"
#include "Renderer.h"
#include "Scene.h"
#include "Primitive.h"
#include "Triangle.h"
#include "Mesh.h"
#include "Camera.h"
#include "Object.h"
#include "Shader.h"
#include "LoaderMesh.h"
#include "glm/mat4x4.hpp"
#include <glm/ext/matrix_transform.hpp>


#define HEIGHT 360
#define WIDTH 640

using std::cout;
using std::endl;
using std::make_shared;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

glm::mat4 modelMatrix;
glm::mat4 viewMatrix;
glm::mat4 projectionMatrix;


struct Vertex
{
    glm::vec4 pos;
    glm::vec4 color;
};
std::vector< Vertex > verts;

GLuint vbo = 0;

GLuint fboId = 0;
Shader shader;
Renderer * renderer;
Ray * ray=new Ray(glm::vec4(0.,0.,10.,1.),glm::vec4(0.,0.,-1.,1.));
Camera * camera = new Camera(ray);
glm::vec4 * color= new glm::vec4[HEIGHT*WIDTH];
static GLubyte imageTex[HEIGHT][WIDTH][4];

unsigned int VBO, VAO, EBO;
std::vector< glm::vec4 > vertices;
std::vector< int > indices;
bool raytracing = true;
void sendMatrix()
{
    Ray * rayCamera = new Ray( glm::vec4(7.,-7.,5.,1.) , glm::vec4(0.,0.,-1.,0.) ); 
    Camera * cameraToWorld = new Camera(rayCamera);
    
    glm::mat4 transformation = glm::scale(1.)*glm::rotationX(0.)*glm::rotationY(15.)*glm::rotationZ(0.)*glm::translation(glm::vec3(0.,0.,-11.));

    modelMatrix = transformation;

    viewMatrix = glm::lookAt(glm::vec4(4.,3.,3.,1.), glm::vec4(0.,0.,0.,1.),glm::vec4(0.,1.,0.,0.));
    projectionMatrix = glm::perspective(45.,16./9.,0.1,100.);

    shader.setUniformMat4("modelMatrix", modelMatrix);
    shader.setUniformMat4("viewMatrix", viewMatrix);
    shader.setUniformMat4("projectionMatrix", projectionMatrix);

}
void draw()
{
    auto t1 = high_resolution_clock::now();
    renderer->draw(imageTex);
    auto t2 = high_resolution_clock::now();
    duration<double, std::milli> ms_double = t2 - t1;
    std::cout << ms_double.count()/1000.<<"s\n";
    
    static GLuint texId;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

   glGenTextures(1, &texId);
   glBindTexture(GL_TEXTURE_2D, texId);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 
                   GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
                   GL_NEAREST);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, 
                HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, 
                imageTex);

    glGenFramebuffers(1, &fboId);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fboId);
    glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                        GL_TEXTURE_2D, texId, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);  
    glBlitFramebuffer(0, 0, WIDTH, HEIGHT, 0, 0, WIDTH, HEIGHT,
                GL_COLOR_BUFFER_BIT, GL_NEAREST);

    glDeleteTextures(1, &texId);
                  
                

}
void display() {
  glClear(GL_COLOR_BUFFER_BIT);   
  draw();
  glFlush();
  glutSwapBuffers ();
}
void idle()
{
    glutPostRedisplay();
}
glm::vec4 computeDirection(glm::vec4 dir, glm::mat4 rotation)
{
    
    dir = glm::multiply(rotation,dir);
    return dir;
}
void key (unsigned char keyPressed, int x, int y) {
    glm::mat4 rotation = camera->getRotation();
    glm::vec4 front = glm::vec4(0.,0.,-1,0.);
    glm::vec4 left = glm::vec4(-1.,0.,0.,0.);
    front = computeDirection(front,rotation);
    left = computeDirection(left,rotation);


    switch (keyPressed) {
    case 'x':
    case 27:
        exit (0);
        break;
    case 'z':
        camera->setPosition( camera->getPosition()+front);
        display();
        break;
    case 's':
        camera->setPosition( camera->getPosition()+(front*-1.));
        display();
        break;

    case 'q':
       camera->setPosition( camera->getPosition()+left);
        
        display();
        break;
    case 'd':
        camera->setPosition( camera->getPosition()+(left*-1.));
        display();
        break;

    case '8':
        camera->setRotation( rotation*glm::rotationX(0.05));
        display();
        break;
    case '2':
        camera->setRotation( rotation*glm::rotationX(-0.05));
        display();
        break;

    case '4':
        camera->setRotation( rotation*glm::rotationY(-.05));
        
        display();
        break;
    case '6':
        camera->setRotation( rotation*glm::rotationY(0.05));
        display();
        break;
    default:
    idle();
        break;

    }
}
int main(int argc , char *argv[] ) {
        
 
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutInitWindowPosition(80, 80);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("A Simple Raytracing");

    glutDisplayFunc(display);
    glutKeyboardFunc (key);

    key ('?', 0, 0);

    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return 0;
    }
    

    Scene * scene = new Scene();

    glm::vec3 b =glm::vec3(0.,0.,1.);
    glm::vec3 r =glm::vec3(1.,0.,0.);
    glm::vec3 g =glm::vec3(0.,1.,0.);

    Material * blueMat = new Material( glm::vec3(0.,0.,1.));
    Material * redMat = new Material( glm::vec3(1.,0.,0.));
    Material * greenMat = new Material(glm::vec3(0.,1.,0.));
    Material * greyMat = new Material( glm::vec3(1.,1.,1.));

    std::vector<glm::vec3> basicColors;
    basicColors.push_back(b);
    basicColors.push_back(r);
    basicColors.push_back(g);
    
    basicColors.push_back(b+r);
    basicColors.push_back(g+r);
    basicColors.push_back(g+b);
    for(size_t i =0; i < 6 ; i ++)
    {
        basicColors.push_back(basicColors[i]*0.5);
    }
    for(size_t i =6; i < 12 ; i ++)
    {
        basicColors.push_back(basicColors[i]*0.5+basicColors[6-i]);
    }


    Material * mirrorMat = new Material( glm::vec3(0.5,0.5,0.5));
    mirrorMat->setType(Material_Mirror);

    Material * glassMat = new Material( glm::vec3(0.,0.,1.));
    glassMat->setType(Material_Glass);

    std::vector<Material *> materials;
    
    std::srand(std::time(nullptr));
    
    for(size_t i =0; i < 18 ; i ++)
    {
        materials.push_back(new Material(basicColors[i]));

    }
    PointLight * pointLight = new PointLight(glm::vec4(-3.,-2.,-2.,1.),glm::vec3(1.,1.,1.),5.,.5,.2);   

    AreaLight * pointLight2 = new AreaLight(glm::vec4(3.,-2.,-2.,1.),glm::vec4(1.,0.,0.,0.),glm::vec4(0.,1.,0.,0.),glm::vec3(1.,1.,1.),2.,.5,.2); 
    scene->addLight(pointLight);
    scene->addLight(pointLight2);

    LoaderObj* loadedSuzanne = new LoaderObj("ressources/Suzanne.obj");
    LoaderObj* loadedCube = new LoaderObj("ressources/Cube.obj");
    LoaderObj* loadedCubeDefault = new LoaderObj("ressources/CubeDefault.obj");

    shared_ptr suzan = loadedSuzanne->buildMesh();
    shared_ptr cube = loadedCube->buildMesh();
    shared_ptr minicube = loadedCubeDefault->buildMesh();

    std::vector<glm::vec4> vertices = loadedCubeDefault->getVertices();
    std::vector<size_t> indices = loadedCubeDefault->getIndices();
    std::vector<glm::vec4> normals =loadedCubeDefault->getNormals();
    for(size_t i = 0 ; i < 3 ; i++)
    {
        for(size_t j = 0 ; j < 3 ; j++)
        {
            for(size_t k = 0 ; k < 3 ; k++)
            {

                shared_ptr coube = make_shared<Mesh>(vertices,indices,normals);
                coube->setMaterial(materials[(i+j+k)%18]);
                coube->flatShaded();
                coube->apply(glm::scale(1.));

                coube->apply(glm::translation (glm::vec3(-2.+2.*j,-2.+2.*i,2.-2*k) ));
                coube->apply(glm::rotationZ(45.)*glm::rotationY(45.));

                coube->apply(glm::translation (glm::vec3(-5.,0.,-10.) ));

    
                scene->hittables.add(coube);
            }
        }
    }

    delete loadedSuzanne;
    delete loadedCube;
    delete loadedCubeDefault;


    
    suzan->setMaterial(greenMat);
    cube->setMaterial(greyMat);
    minicube->setMaterial(greenMat);

    glm::mat4 scale2 = glm::scale (2);
    glm::mat4 rotX = glm::rotationX (0.*M_PI/180.);
    glm::mat4 rotY = glm::rotationY (0.*M_PI/180.);
    glm::mat4 rotZ = glm::rotationZ (0.*M_PI/180.);
    glm::mat4 transformation3 = glm::translation (glm::vec3(0.,2.,-3.));
    suzan->flatShaded();
    suzan->apply(scale2);
    suzan->apply(rotX*rotY*rotZ);
    suzan->apply(transformation3);


    scale2 = glm::scale (20.,20.,40.);
    transformation3 = glm::translation (glm::vec3(0.,0.,-10.));
    cube->flatShaded();
    cube->apply(scale2);
    cube->apply(transformation3);
    
    cube->setMaterialTriangle(0,redMat); // gauche bas
    cube->setMaterialTriangle(1,greyMat); // face ag ?
    cube->setMaterialTriangle(2,blueMat); // ok
    cube->setMaterialTriangle(3,greyMat); // ??

    cube->setMaterialTriangle(4,greyMat); // bas gauche
    cube->setMaterialTriangle(5,greyMat); // haut droit
    cube->setMaterialTriangle(6,redMat); // gauche haut
    cube->setMaterialTriangle(7,greyMat); // face ad

    cube->setMaterialTriangle(8,blueMat); // droite haut
    cube->setMaterialTriangle(9,greyMat);
    cube->setMaterialTriangle(10,greyMat);
    cube->setMaterialTriangle(11,greyMat);
   
    float rotationCube = 45.*(float)std::rand()/RAND_MAX;
    rotX = glm::rotationX (-rotationCube*M_PI/180.);
    rotY = glm::rotationY (rotationCube*M_PI/180.);
    rotZ = glm::rotationZ (rotationCube*M_PI/180.);
    minicube->flatShaded();
    minicube->apply(rotX*rotY*rotZ);

    minicube->apply(glm::translation (glm::vec3(4.,-4.,-10.)));

    for(size_t i = 0 ; i< 12;i++)
        minicube->setMaterialTriangle(i,materials[i]);


    scene->hittables.add(suzan);
    scene->hittables.add(cube);
    scene->hittables.add(minicube);
    
    scene->hittables.add(make_shared<Sphere>(glm::vec4(4.,-4.,-10.,1.), 4.f,mirrorMat));
    scene->hittables.add(make_shared<Sphere>(glm::vec4(-4.,8.,-11.,1.),  4.f,mirrorMat));
    scene->hittables.add(std::make_shared<Sphere>(glm::vec4(-5.,1.,-4.,1.),4.f,glassMat));


    for(size_t i = 0 ; i < 25 ; i++)
    {
        glm::vec4 pos = glm::vec4(20.*(float)std::rand()/RAND_MAX-10.,
        20.*(float)std::rand()/RAND_MAX-10.,
        40.*(float)std::rand()/RAND_MAX-30.,
        1.);
        float size = std::max((4.*(float)std::rand()/RAND_MAX),1.);

        scene->hittables.add(make_shared<Sphere>(pos,size,materials[i%18]));

    }
    //scene->buildBVH();
    Raytracer * raytracer = new Raytracer(scene, 1);
    renderer = new Renderer((int)HEIGHT, (int)WIDTH,camera,scene,raytracer);

    glutMainLoop();
    delete[] color;
    delete camera;
    delete scene;
    delete blueMat; 
    delete greenMat;
    delete greyMat ; 
    delete redMat ;
    delete mirrorMat  ; 
    delete glassMat  ;

    delete pointLight;
    delete pointLight2;
    delete renderer;
    delete raytracer;

    return 0;
}