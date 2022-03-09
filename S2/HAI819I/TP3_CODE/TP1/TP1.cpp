// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>
#include <string>
using namespace glm;

#include <common/shader.hpp>
#include <common/PerlinNoise.h>
#include <common/texture.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>
#include "common/functions.cpp"

#include "common/Test.hpp"
#include "common/sceneGraph.hpp"

void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
glm::vec3 camera_position   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 camera_target = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camera_up    = glm::vec3(0.0f, 1.0f,  0.0f);

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

//rotation
float angleRotation = 1.;
float coeffAngle2 = 1.;
float angleRotation2=0.;
float angleRotation3=0.;
float angleRotation4=0.;
float zoom = 1.;
/*******************************************************************************/

int sommets = 64;
double resolution=32.;
int orbital = 1;

std::vector<glm::vec2> indexed_uvs;
std::vector<unsigned short> indices2; 
std::vector<glm::vec3> indexed_vertices2;


void createMap(    std::vector<unsigned short> &indices,std::vector<glm::vec3> &indexed_vertices,std::vector<glm::vec2> &indexed_uvs, double sommets)
{

                 indexed_vertices.clear();
    indexed_uvs.clear();
    indices.clear();
    float size = 0.2;

    int sizeMap = 512;
    float const R = 1./(float)(sommets-1);
    float const S = 1./(float)(sommets-1);
    float radius =1.;

    for(int i = 0 ; i < sommets ; i++)
        for(int j = 0 ; j < sommets ; j++)
        {
            float const y = sin( -M_PI_2 + M_PI * i * R );
            float const x = cos(2*M_PI * j * S) * sin( M_PI * i * R );
            float const z = sin(2*M_PI * j * S) * sin( M_PI * i * R );
            indexed_uvs.push_back(glm::vec2(j*S, i*R));
            indexed_vertices.push_back(vec3(x,y,z) * radius);
            
             
        }
        
    /*for(int i = 0 ; i < sommets; i++)
        for(int j = 0; j < sommets ; j++)
        {
            indexed_uvs.push_back( glm::vec2(1.-(float)(i)/(sommets+1) ,1.-(float)(j)/(sommets+1) ));

        }*/
        
     for(int i = 0 ; i < sommets -1; i++)
        for(int j = 0 ; j < sommets-1 ; j++)
        {
            indices.push_back(i*sommets +j);
            indices.push_back(i*sommets +j+1); 
            indices.push_back((i+1)*sommets +j); 

            indices.push_back(i*sommets +j+1); 
            indices.push_back((i+1)*sommets +j); 
            indices.push_back((i+1)*sommets +j+1); 

        }

        
}

void loadTextures(GLuint (&TextureMountain)[8],GLuint (&TextureID)[8],  GLuint programID)
{
    TextureMountain[0] = loadBMP_custom((char *)"textures/sun.bmp");
    TextureMountain[1] = loadBMP_custom((char *)"textures/terre.bmp");
    TextureMountain[2] = loadBMP_custom((char *)"textures/tex_snowrock.bmp");

    TextureMountain[3] = loadBMP_custom((char *)"textures/hmap_cliffs.bmp");

    TextureMountain[4] = loadBMP_custom((char *)"textures/sand.bmp");
    TextureMountain[5] = loadBMP_custom((char *)"textures/rance.bmp");
    TextureMountain[6] = loadBMP_custom((char *)"textures/wata.bmp");
    TextureMountain[7] = loadBMP_custom((char *)"textures/color.bmp");

    

    TextureID[0]  = glGetUniformLocation(programID,"myTextureSampler[0]" );
    TextureID[1]  = glGetUniformLocation(programID,"myTextureSampler[1]" );
    TextureID[2]  = glGetUniformLocation(programID,"myTextureSampler[2]" );
    TextureID[3]  = glGetUniformLocation(programID,"myTextureSampler[3]" );
    TextureID[4]  = glGetUniformLocation(programID,"myTextureSampler[4]" );
    TextureID[5]  = glGetUniformLocation(programID,"myTextureSampler[5]" );
    TextureID[6]  = glGetUniformLocation(programID,"myTextureSampler[6]" );
    TextureID[7]  = glGetUniformLocation(programID,"myTextureSampler[7]" );

    for(int i=0;i<8;i++)
    {
        glActiveTexture(GL_TEXTURE0+i);
        glBindTexture(GL_TEXTURE_2D, TextureMountain[i]);
        glUniform1i(TextureID[i], i);

    }   

}
Mesh * createSphere()
{
    Mesh * mesh = new Mesh();

    createMap(mesh->indices,mesh->points,mesh->uvs, resolution);
    return mesh;
}
typedef struct BUFFER
{
    GLuint vertexbuffer;
    GLuint elementbuffer;
    GLuint uvbuffer;

}BUFFER;
void sendGPU(BUFFER &buffer, Mesh * mesh)
{
    glGenBuffers(1, &buffer.vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer.vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, mesh->points.size() * sizeof(glm::vec3), &mesh->points[0], GL_STATIC_DRAW);

    glGenBuffers(1, &buffer.elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size() * sizeof(unsigned short), &mesh->indices[0] , GL_STATIC_DRAW);
        
    glGenBuffers(1, &buffer.uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer.uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, mesh->uvs.size()* sizeof(glm::vec2), &mesh->uvs[0], GL_STATIC_DRAW);

}
int main( void )
{
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( 1024, 768, "TP1 - GLFW", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    //  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1024/2, 768/2);

    // Dark blue background
    glClearColor(0.015f, 0.11f, 0.196f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    //glEnable(GL_CULL_FACE);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);


    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders( "vertex_shader.glsl", "fragment_shader.glsl" );

    /*****************TODO***********************/
    // Get a handle for our "Model View Projection" matrices uniforms

    /****************************************/
    std::vector<unsigned short> indices; //Triangles concaténés dans une liste
    std::vector<std::vector<unsigned short> > triangles;
    std::vector<glm::vec3> indexed_vertices;
    float size = 0.2;

    glUseProgram(programID);
    GLuint TextureMountain[8];
    GLuint TextureID[8];

    loadTextures(TextureMountain,TextureID, programID);
    

 

    // For speed computation
    double lastTime = glfwGetTime();
    int nbFrames = 0;

    
    float transflateFactor = 1.5;
    float scaleFactor =1.;
    glm::mat4 modelmatrix  = glm::mat4( 1.0f );
    glm::mat4 idmatrix  = glm::mat4( 1.0f );
    
    glm::mat4 projectionMatrix  = glm::perspective(45.,4./3.,0.1,1000.);

    glm::mat4 scaleMatrix  = glm::scale(glm::mat4(1.0f),glm::vec3(1.));
    glm::mat4 rotationMatrix  = glm::rotate(glm::mat4(1.0f), 0.f, glm::vec3(1.0));
    glm::mat4 translationMatrix  = glm::translate(glm::mat4(1.0f),glm::vec3(0.f, 0.,0.0f));

    GLuint modelID = glGetUniformLocation(programID, "model");
    GLuint viewID = glGetUniformLocation(programID, "view");
    GLuint projectionID  = glGetUniformLocation(programID, "projection");

    
    /*GLuint vertexbuffer;*/
    /*GLuint elementbuffer;*/
    /*GLuint uvbuffer;*/
    


    createMap(  indices2,indexed_vertices2,indexed_uvs,  resolution);
    /*
    Test * grapheDeScene = new Test();
    grapheDeScene->run();
    grapheDeScene->runRotation();
    */

    Mesh * mesh_sun = createSphere();
    Mesh * mesh_earth = createSphere();
    Mesh * mesh_mars = createSphere();
    Mesh * mesh_moon = createSphere();

    SceneGraphComposite* sunGraph = new SceneGraphComposite();
    SceneGraphComposite* earthGraph = new SceneGraphComposite();
    SceneGraphComposite* marsGraph = new SceneGraphComposite();
    SceneGraphLeaf* moonGraph = new SceneGraphLeaf();

    sunGraph->gameObject = new GameObject();
    earthGraph->gameObject = new GameObject();
    marsGraph->gameObject = new GameObject();
    moonGraph->gameObject = new GameObject();


    sunGraph->gameObject->mesh = mesh_sun;
    earthGraph->gameObject->mesh = mesh_earth;
    marsGraph->gameObject->mesh = mesh_mars;

    moonGraph->gameObject->mesh = mesh_moon;

    BUFFER buffer;
    BUFFER bufferEarth;
    BUFFER bufferMars;
    BUFFER bufferMoon;
    glm::mat3 rotationAxe = Transform::convertMat4(glm::rotate(glm::mat4(1.0f), (float)M_PI+(float)M_PI_2,glm::vec3(0.f,1.f,0.0)));
    glm::mat3 rotationAxe2 = Transform::convertMat4(glm::rotate(glm::mat4(1.0f), (float)M_PI_2/4.f,glm::vec3(0.f,1.f,0.0)));

    glm::mat3 orientationAxe = Transform::convertMat4(glm::rotate(glm::mat4(1.0f), (float)M_PI_2 ,glm::vec3(0.f,1.f,0.0)));
    
    
    earthGraph->add(moonGraph);
    earthGraph->add(marsGraph);
    //earthGraph->inverse();
    //earthGraph->apply();
    
    


    
    sendGPU(buffer,mesh_sun);
    sendGPU(bufferEarth,mesh_earth);
    sendGPU(bufferMars,mesh_mars);
    sendGPU(bufferMoon,mesh_moon);


    

    do{

        // Measure speed
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime(); 
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);
        angleRotation2+=max(1.f,(coeffAngle2/10.f));

        angleRotation3+=2.*coeffAngle2;
        angleRotation4+=0.5*coeffAngle2;


        angleRotation2 = (angleRotation2 > 360) ? 0 : angleRotation2;
        angleRotation3 = (angleRotation3 > 360) ? 0 : angleRotation3;
        angleRotation4 = (angleRotation4 > 360) ? 0 : angleRotation4;
        //std::cout<<"\n"<<angleRotation2<<"\n";


        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
       
         glm::mat4 viewMatrix;
         float flip = M_PI/4;

         
        if(orbital)
        {
            camera_position = glm::vec3(0,50.,0.);
            camera_target= glm::vec3(0.0f, 0.0f, -1.0f);

            camera_position =  glm::rotateY(camera_position,(float)radians(angleRotation2));
            viewMatrix  = glm::lookAt(camera_position,camera_target,camera_up);
        }
        else
        {
            
             viewMatrix  = glm::lookAt(camera_position,camera_position+camera_target,camera_up);
             
        }

    
        // TERRAIN

        glm::mat3 rotationAxe = Transform::convertMat4(glm::rotate(glm::mat4(1.0f), glm::radians(angleRotation2),glm::vec3(0.f,1.f,0.0)));
        glm::mat3 rotationAxe2 = Transform::convertMat4(glm::rotate(glm::mat4(1.0f), glm::radians(angleRotation3),glm::vec3(0.f,1.f,0.0)));
        glm::mat3 rotationAxe3 = Transform::convertMat4(glm::rotate(glm::mat4(1.0f), glm::radians(angleRotation4),glm::vec3(0.f,1.f,0.0)));

        Transform * earthOrientation = new Transform(rotationAxe);
        Transform * earthOrientation2 = new Transform(rotationAxe2);
        Transform * earthOrientation3 = new Transform(rotationAxe3);

        Transform * earthTranslation = new Transform(glm::vec3(10.f,0.,0.0) );
        Transform * marsTranslation = new Transform(glm::vec3(7.f,0.,0.0) );
        Transform * moonTranslation= new Transform(glm::vec3(3.f,0.,0.0) );

        //Transform * moonOrientation = new Transform(rotationAxe2);
        
        earthGraph->gameObject->transformation.clear();
        marsGraph->gameObject->transformation.clear();
        moonGraph->gameObject->transformation.clear();
        
        earthGraph->gameObject->transformation.push_back(earthTranslation);
        earthGraph->gameObject->transformation.push_back(earthOrientation3);

        marsGraph->gameObject->transformation.push_back(marsTranslation);
        marsGraph->gameObject->transformation.push_back(earthOrientation);
        
        moonGraph->gameObject->transformation.push_back(moonTranslation);
        moonGraph->gameObject->transformation.push_back(earthOrientation2);
        
        //glm::mat4 mat = earthTranslation->getMat4(); // moonTranslation->getMat4()* earthOrientation->getMat4()* earthTranslation->getMat4();
        //Transform * mo = new Transform(Transform::convertMat4(glm::rotate(mat, (float)M_PI_2/4.f,glm::vec3(0.f,1.f,0.0))));
        //moonGraph->gameObject->transformation.push_back(mo);
        

        

        earthGraph->apply();

        sendGPU(buffer,mesh_sun);
        sendGPU(bufferEarth,mesh_earth);
        sendGPU(bufferMars,mesh_mars);
        sendGPU(bufferMoon,mesh_moon);

        earthGraph->inverse();

        glUseProgram(programID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextureMountain[0]);
        glUniform1i(TextureID[0], 0);

        //scaleMatrix  = glm::scale(glm::mat4(1.0f),glm::vec3(20.*scaleFactor));
        //rotationMatrix  = glm::rotate(glm::mat4(1.0f), (float)M_PI/2, glm::vec3(-1.,0.,0.0));
        
        //translationMatrix  = glm::translate(glm::mat4(1.0f),glm::vec3(-2.f, -5.,0.0f));

        modelmatrix = translationMatrix*rotationMatrix *scaleMatrix*idmatrix;

        glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(modelmatrix));
        glUniformMatrix4fv(viewID, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(projectionID, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        

        
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, buffer.vertexbuffer);
        glVertexAttribPointer(
                    0,                  // attribute
                    3,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    0,                  // stridedeltaTime
                    (void*)0           // element array buffer offset
                    );
        
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, buffer.uvbuffer);
        glVertexAttribPointer(
                    1,                  // attribute
                    2,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    0,                  // stridedeltaTime
                    (void*)0           // element array buffer offset
                    );

                 // Draw the triangles !
        
        glDrawElements(
                    GL_TRIANGLES,      // mode
                    indices2.size(),    // count
                    GL_UNSIGNED_SHORT,   // typecamera_position
                        (void*)0           // element array buffer offset
                    );
    
        // wata ?
        
        //scaleMatrix  = glm::scale(glm::mat4(1.0f),glm::vec3(10.f));
        //translationMatrix  = glm::translate(glm::mat4(1.0f),glm::vec3(50.f, -5.,0.0f));
        modelmatrix = translationMatrix*rotationMatrix *scaleMatrix*idmatrix;
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextureMountain[1]);
        glUniform1i(TextureID[1], 1);


        glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(modelmatrix));
        glUniformMatrix4fv(viewID, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(projectionID, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, bufferEarth.vertexbuffer);
        glVertexAttribPointer(
                    0,                  // attribute
                    3,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    0,                  // stridedeltaTime
                    (void*)0           // element array buffer offset
                    );
        
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, bufferEarth.uvbuffer);
        glVertexAttribPointer(
                    1,                  // attribute
                    2,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    0,                  // stridedeltaTime
                    (void*)0           // element array buffer offset
                    );

                 // Draw the triangles !
        
        glDrawElements(
                    GL_TRIANGLES,      // mode
                    indices2.size(),    // count
                    GL_UNSIGNED_SHORT,   // typecamera_position
                        (void*)0           // element array buffer offset
                    );

                    
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, bufferMars.vertexbuffer);
        glVertexAttribPointer(
                    0,                  // attribute
                    3,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    0,                  // stridedeltaTime
                    (void*)0           // element array buffer offset
                    );
        
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, bufferMars.uvbuffer);
        glVertexAttribPointer(
                    1,                  // attribute
                    2,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    0,                  // stridedeltaTime
                    (void*)0           // element array buffer offset
                    );

                 // Draw the triangles !
        
        glDrawElements(
                    GL_TRIANGLES,      // mode
                    indices2.size(),    // count
                    GL_UNSIGNED_SHORT,   // typecamera_position
                        (void*)0           // element array buffer offset
                    );
                    
        //scaleMatrix  = glm::scale(glm::mat4(1.0f),glm::vec3(5.f));
        //translationMatrix  = glm::translate(glm::mat4(1.0f),glm::vec3(50.f, 10.,20.0f));
        modelmatrix = translationMatrix*rotationMatrix *scaleMatrix*idmatrix;
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextureMountain[2]);
        glUniform1i(TextureID[0], 0);


        glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(modelmatrix));
        glUniformMatrix4fv(viewID, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(projectionID, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, bufferMoon.vertexbuffer);
        glVertexAttribPointer(
                    0,                  // attribute
                    3,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    0,                  // stridedeltaTime
                    (void*)0           // element array buffer offset
                    );
        
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, bufferMoon.uvbuffer);
        glVertexAttribPointer(
                    1,                  // attribute
                    2,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    0,                  // stridedeltaTime
                    (void*)0           // element array buffer offset
                    );

                 // Draw the triangles !
        
        glDrawElements(
                    GL_TRIANGLES,      // mode
                    indices2.size(),    // count
                    GL_UNSIGNED_SHORT,   // typecamera_position
                        (void*)0           // element array buffer offset
                    );
        //translationMatrix  = glm::translate(glm::mat4(1.0f),glm::vec3(35.f, -5.,0.0f));
       
    

        // Swap buffers
                // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    // Cleanup VBO and shader
    glDeleteBuffers(1, &buffer.vertexbuffer);
    glDeleteBuffers(1, &buffer.elementbuffer); 
    glDeleteBuffers(1, &buffer.uvbuffer);

    glDeleteProgram(programID);

    glDeleteVertexArrays(1, &VertexArrayID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //Camera zoom in and out
    float cameraSpeed = 20. * deltaTime;
    float angle = 0.01;

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        camera_position -= deltaTime * glm::cross(camera_target, camera_up);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        camera_position += deltaTime * glm::cross(camera_target, camera_up);
    
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera_position += deltaTime * camera_target;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera_position -= deltaTime * camera_target;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera_position += cameraSpeed * glm::vec3(0.,1.,0.);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera_position -= cameraSpeed * glm::vec3(0.,1.,0.);
    
    if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS)
    {
        camera_target =  glm::vec3(glm::rotate(glm::mat4(1),2*angle,camera_up)*glm::vec4(camera_target,1));
    }
    if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS)
    {
        camera_target =  glm::vec3(glm::rotate(glm::mat4(1),-2*angle,camera_up)*glm::vec4(camera_target,1));
    }
    
    if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS)
    {
        camera_target =  glm::vec3(glm::rotate(glm::mat4(1),2*angle,glm::cross(camera_target, camera_up))*glm::vec4(camera_target,1));
    }    
    if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS)
    {
        camera_target =  glm::vec3(glm::rotate(glm::mat4(1),-2*angle,glm::cross(camera_target, camera_up))*glm::vec4(camera_target,1));
    }    
    
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        coeffAngle2++;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        coeffAngle2 = coeffAngle2--<1?1:coeffAngle2;
    if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
    {

        resolution+=1;
        std::cout<<"resolution : "<<resolution<<std::endl;
        std::cout<<camera_position[0]<<":"<<camera_position[1]<<":"<<camera_position[2]<<"\n";  
        
        createMap(  indices2,indexed_vertices2,indexed_uvs,  resolution);
       
    }
    if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
    {

        resolution = resolution-- ==1 ? 1: resolution;
        std::cout<<"resolution : "<<resolution<<std::endl;
        std::cout<<camera_position[0]<<":"<<camera_position[1]<<":"<<camera_position[2]<<"\n";
        createMap(  indices2,indexed_vertices2,indexed_uvs,  resolution);
       

    }  

    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
        angleRotation ++;
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
        angleRotation --;
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    {
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE)
        {
            orbital =!orbital;
             
             camera_position = glm::vec3(20.,40.,-90.);
             camera_target-=camera_position;
             
        }
        
            
    }

    //TODO add translations

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
