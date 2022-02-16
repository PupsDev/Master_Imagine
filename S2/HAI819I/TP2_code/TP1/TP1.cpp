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
#include <iostream>
#include <string>
using namespace glm;

#include <common/shader.hpp>
#include <common/PerlinNoise.h>
#include <common/texture.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>
#include "common/functions.cpp"

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
float zoom = 1.;
/*******************************************************************************/

int sommets = 64;
double resolution=32.;
int orbital = 1;

std::vector<glm::vec2> indexed_uvs;
std::vector<unsigned short> indices2; 
std::vector<glm::vec3> indexed_vertices2;

std::vector<glm::vec2>      wata_uv;
std::vector<unsigned short> wata_indice; 
std::vector<glm::vec3>      wata_vertices;

double OctavePerlin(double x, double y, double z, int octaves, double persistence) {
    PerlinNoise pn(114);
    double total = 0;
    double frequency = 4;
    double amplitude = 128;
    double maxValue = 0;  // Used for normalizing result to 0.0 - 1.0
    for(int i=0;i<octaves;i++) {
        total += pn.noise(x * frequency, y * frequency, z * frequency) * amplitude;
        
        maxValue += amplitude;
        
        amplitude *= persistence;
        frequency *= 2;
    }
    
    return total/maxValue;
}
void createMap(    std::vector<unsigned short> &indices,std::vector<glm::vec3> &indexed_vertices,std::vector<glm::vec2> &indexed_uvs, double sommets)
{
    indexed_vertices.clear();
    indexed_uvs.clear();
    indices.clear();
    float size = 0.2;
    //std::cout<<"sommets:"<<sommets<<"\n";
    int sizeMap = 512;
    unsigned int seed = 237;
    PerlinNoise pn(seed);
    std::vector<std::vector<double>> mapNoise;

    mapNoise.resize(sommets);
    for(auto& mn : mapNoise)mn.resize(sommets);

    for(int i = 0 ; i < sommets ; i++)
        for(int j = 0 ; j < sommets ; j++)
        {
            mapNoise[i][j] = (double)5.* pn.noise(j, i, 0.8) ;
            //std::cout<<mapNoise[i][j]<<std::endl;
        }

    for(int i = 0 ; i < sommets ; i++)
        for(int j = 0 ; j < sommets ; j++)
        {
            indexed_vertices.push_back( glm::vec3((float)(i*sizeMap/sommets)*size - (sizeMap/2)*size ,(float)(j*sizeMap/sommets)*size- (sizeMap/2)*size,0.2*mapNoise[i][j]));//size*rand()/RAND_MAX) );
             
        }
        
    for(int i = 0 ; i < sommets; i++)
        for(int j = 0; j < sommets ; j++)
        {
            indexed_uvs.push_back( glm::vec2(1.-(float)(i)/(sommets+1) ,1.-(float)(j)/(sommets+1) ));

        }
        
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
void createMap2(    std::vector<unsigned short> &indices,std::vector<glm::vec3> &indexed_vertices,std::vector<glm::vec2> &indexed_uvs, double sommets)
{
    indexed_vertices.clear();
    indexed_uvs.clear();
    indices.clear();
    float size = 0.2;
    //std::cout<<"sommets:"<<sommets<<"\n";
    int sizeMap = 128;
    unsigned int seed = 237;
    PerlinNoise pn(seed);
    std::vector<std::vector<double>> mapNoise;

    mapNoise.resize(sommets);
    for(auto& mn : mapNoise)mn.resize(sommets);

    for(int i = 0 ; i < sommets ; i++)
         for(int j = 0; j < sommets ; j++)
        {
            mapNoise[i][j] = (double)5.* pn.noise(j, i, 0.8) ;
            //std::cout<<mapNoise[i][j]<<std::endl;
        }

    for(int i = 0 ; i < sommets ; i++)
        for(int j = 0; j < sommets ; j++)
        {
            indexed_vertices.push_back( glm::vec3((float)(i*sizeMap/sommets)*size - (sizeMap/2)*size ,(float)(j*sizeMap/sommets)*size- (sizeMap/2)*size,0.2*mapNoise[i][j]));//size*rand()/RAND_MAX) );
             
        }
        
    for(int i = 0 ; i < sommets; i++)
         for(int j = 0; j < sommets ; j++)
        {
            indexed_uvs.push_back( glm::vec2(1.-(float)(sommets-i)/(sommets+1) ,1.-(float)(j)/(sommets+1) ));

        }
        
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
    glClearColor(0.8f, 0.8f, 0.8f, 0.0f);

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
    GLuint programID2 = LoadShaders( "vertex_shader2.glsl", "fragment_shader2.glsl" );
    

   

    /*****************TODO***********************/
    // Get a handle for our "Model View Projection" matrices uniforms

    /****************************************/
    std::vector<unsigned short> indices; //Triangles concaténés dans une liste
    std::vector<std::vector<unsigned short> > triangles;
    std::vector<glm::vec3> indexed_vertices;



    //Chargement du fichier de maillage
    //std::string filename("chair.off");
    std::string filename("suzanne.off");
    loadOFF(filename, indexed_vertices, indices, triangles );

    
    float size = 0.2;



    ImageG imageG;
    int nH, nW;
    lire_nb_lignes_colonnes_image_pgm("heightmap.pgm", &nH, &nW);
    resize(imageG,nW,nH);
    
    loadImage("heightmap.pgm",imageG,nW, nH);

    //saveImage( folderOut, (char*)"originale_",inputName,imageG);
    createMap(  indices2,indexed_vertices2,indexed_uvs,  resolution);

    
        
        

    // Load it into a VBO



    GLuint uvbuffer;
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size()* sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

    // Get a handle for our "LightPosition" uniform
    glUseProgram(programID);
    GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

    //GLuint Texture = loadBMP_custom((char *)"maison.bmp");
    GLuint Texture = loadBMP_custom((char *)"mountain.bmp");

    GLuint TextureMountain[7];
    TextureMountain[0] = loadBMP_custom((char *)"textures/tex_grass.bmp");
    TextureMountain[1] = loadBMP_custom((char *)"textures/tex_rock.bmp");
    TextureMountain[2] = loadBMP_custom((char *)"textures/tex_snowrock.bmp");

    TextureMountain[3] = loadBMP_custom((char *)"textures/hmap_cliffs.bmp");
    TextureMountain[4] = loadBMP_custom((char *)"textures/hmap_mountain.bmp");
    //TextureMountain[5] = loadBMP_custom((char *)"textures/hmap_rocky.bmp");
    TextureMountain[5] = loadBMP_custom((char *)"textures/rance.bmp");
    TextureMountain[6] = loadBMP_custom((char *)"textures/wata.bmp");

    GLuint TextureID[7];

    TextureID[0]  = glGetUniformLocation(programID,"myTextureSampler[0]" );
    TextureID[1]  = glGetUniformLocation(programID,"myTextureSampler[1]" );
    TextureID[2]  = glGetUniformLocation(programID,"myTextureSampler[2]" );
    TextureID[3]  = glGetUniformLocation(programID,"myTextureSampler[3]" );
    TextureID[4]  = glGetUniformLocation(programID,"myTextureSampler[4]" );
    TextureID[5]  = glGetUniformLocation(programID,"myTextureSampler[5]" );
    TextureID[6]  = glGetUniformLocation(programID,"myTextureSampler[6]" );
    

    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, Texture2);
    //glUniform1i(TextureID[0], 0);

    for(int i=0;i<7;i++)
    {
        glActiveTexture(GL_TEXTURE0+i);
        glBindTexture(GL_TEXTURE_2D, TextureMountain[i]);
        glUniform1i(TextureID[i], i);

    }

    //glUniform1i(TextureID[0]2, 1);
     

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

    glUseProgram(programID2);
    GLuint TextureID2;
    TextureID2  = glGetUniformLocation(programID2,"myTextureSampler2" );

    GLuint modelID2 = glGetUniformLocation(programID2, "model");
    GLuint viewID2 = glGetUniformLocation(programID2, "view");
    GLuint projectionID2  = glGetUniformLocation(programID2, "projection");


    do{

        // Measure speed
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime(); 
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);
        angleRotation2+=max(1.f,(coeffAngle2/10));

        angleRotation2 = (angleRotation2 > 360) ? 0 : angleRotation2;


        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
       
         glm::mat4 viewMatrix;
         
        if(orbital)
        {
            camera_position = glm::vec3(1.6,137.,-90.);
            camera_position = glm::vec3(0.,40.,50.);
            camera_target= glm::vec3(0.0f, 0.0f, -1.0f);

            camera_position =  glm::rotateY(camera_position,(float)radians(angleRotation2));
            viewMatrix  = glm::lookAt(camera_position,camera_target,camera_up);
        }
        else
        {
            
             viewMatrix  = glm::lookAt(camera_position,camera_position+camera_target,camera_up);
             std::cout<<camera_position[0]<<":"<<camera_position[1]<<":"<<camera_position[2]<<"\n";
        }
            
        
         glUseProgram(programID2);
        modelmatrix = translationMatrix*rotationMatrix *scaleMatrix*modelmatrix;
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture);
        glUniform1i(TextureID2, 0);

        glUniformMatrix4fv(modelID2, 1, GL_FALSE, glm::value_ptr(modelmatrix));
        glUniformMatrix4fv(viewID2, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(projectionID2, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

            GLuint vertexbuffer;
        glGenBuffers(1, &vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

        // Generate a buffer for the indices as well
        GLuint elementbuffer;
        glGenBuffers(1, &elementbuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);
        /*

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                    0,                  // attribute
                    3,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    0,                  // stridedeltaTime
                    (void*)0           // element array buffer offset
                    );
        
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glVertexAttribPointer(
                    1,                  // attribute
                    2,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    0,                  // stridedeltaTime
                    (void*)0           // element array buffer offset
                    );

        //rotationMatrix  *= glm::rotate(glm::mat4(1.0f), (float)radians(angleRotation2), glm::vec3(0.,1.,0.0));
         scaleMatrix  = glm::scale(glm::mat4(1.0f),glm::vec3(5.*scaleFactor));
        rotationMatrix  = glm::rotate(glm::mat4(1.0f), (float)radians(-angleRotation), glm::vec3(0.,0.,1.0));
        rotationMatrix  *= glm::rotate(glm::mat4(1.0f), (float)radians(-angleRotation2), glm::vec3(0.,1.,0.0));
        translationMatrix  = glm::translate(glm::mat4(1.0f),glm::vec3(-10.f, 20.,0.0f));

        modelmatrix = translationMatrix*rotationMatrix *scaleMatrix*idmatrix;
        glUniformMatrix4fv(modelID2, 1, GL_FALSE, glm::value_ptr(modelmatrix));

        // Draw the triangles !
        
        glDrawElements(
                    GL_TRIANGLES,      // mode
                    indices.size(),    // count
                    GL_UNSIGNED_SHORT,   // type
                    (void*)0           // element array buffer offset
                    );
                    
        

        float flip = M_PI/4;
        scaleMatrix  = glm::scale(glm::mat4(1.0f),glm::vec3(5.*scaleFactor));
        
        //rotationMatrix  = glm::rotate(glm::mat4(1.0f), flip, glm::vec3(1.,0.,0.0));

        rotationMatrix  = glm::rotate(glm::mat4(1.0f), (float)radians(-angleRotation), glm::vec3(0.,0.,1.0));
        rotationMatrix  *= glm::rotate(glm::mat4(1.0f), (float)radians(angleRotation2), glm::vec3(0.,1.,0.0));
        //rotationMatrix  = glm::rotate(glm::mat4(1.0f), flip, glm::vec3(1.,0.,0.0));
        
        translationMatrix  = glm::translate(glm::mat4(1.0f),glm::vec3(0.f, 20.,0.0f));

        modelmatrix = translationMatrix*rotationMatrix *scaleMatrix*idmatrix;
        glUniformMatrix4fv(modelID2, 1, GL_FALSE, glm::value_ptr(modelmatrix));
        
        // Draw the triangles !
        glDrawElements(
                    GL_TRIANGLES,      // mode
                    indices.size(),    // count
                    GL_UNSIGNED_SHORT,   // type
                    (void*)0           // element array buffer offset
                    );
                    */
       
        // PLAN
        createMap(  indices2,indexed_vertices2,indexed_uvs,  resolution);
        //wata
        for(int i=0;i<7;i++)
        {
            glActiveTexture(GL_TEXTURE0+i);
            glBindTexture(GL_TEXTURE_2D, TextureMountain[i]);
            glUniform1i(TextureID[i], i);

        }

        glUseProgram(programID);
        scaleMatrix  = glm::scale(glm::mat4(1.0f),glm::vec3(1.5*scaleFactor));
        
        if(orbital)
        {
             rotationMatrix  = glm::rotate(glm::mat4(1.0f), (float)M_PI/2, glm::vec3(-1.,0.,0.0));
             //rotationMatrix  *= glm::rotate(glm::mat4(1.0f), (float)radians(angleRotation2), glm::vec3(0.,0.,1.0));
        }
        else
            rotationMatrix  = glm::rotate(glm::mat4(1.0f), (float)M_PI/2, glm::vec3(-1.,0.,0.0));
            
        translationMatrix  = glm::translate(glm::mat4(1.0f),glm::vec3(-2.f, -5.,0.0f));

        modelmatrix = translationMatrix*rotationMatrix *scaleMatrix*idmatrix;

        glUniformMatrix4fv(modelID2, 1, GL_FALSE, glm::value_ptr(modelmatrix));
        glUniformMatrix4fv(viewID2, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(projectionID2, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        
       
        glGenBuffers(1, &vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, indexed_vertices2.size() * sizeof(glm::vec3), &indexed_vertices2[0], GL_STATIC_DRAW);

        glGenBuffers(1, &elementbuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices2.size() * sizeof(unsigned short), &indices2[0] , GL_STATIC_DRAW);
            
        glGenBuffers(1, &uvbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size()* sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextureMountain[0]);
        glUniform1i(TextureID[0], 0);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                    0,                  // attribute
                    3,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    0,                  // stridedeltaTime
                    (void*)0           // element array buffer offset
                    );
        
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
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
                    /*
        createMap( wata_indice,wata_vertices,wata_uv,resolution);
        
        glUseProgram(programID2);
         modelmatrix = translationMatrix*rotationMatrix *scaleMatrix*idmatrix;
        glUniformMatrix4fv(modelID2, 1, GL_FALSE, glm::value_ptr(modelmatrix));

            glGenBuffers(1, &vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, wata_vertices.size() * sizeof(glm::vec3), &wata_vertices[0], GL_STATIC_DRAW);

        glGenBuffers(1, &elementbuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, wata_indice.size() * sizeof(unsigned short), &wata_indice[0] , GL_STATIC_DRAW);
            
        glGenBuffers(1, &uvbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glBufferData(GL_ARRAY_BUFFER, wata_uv.size()* sizeof(glm::vec2), &wata_uv[0], GL_STATIC_DRAW);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextureMountain[6]);
        glUniform1i(TextureID[6], 6);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                    0,                  // attribute
                    3,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    0,                  // stridedeltaTime
                    (void*)0           // element array buffer offset
                    );
        
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
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
    */
        translationMatrix  = glm::translate(glm::mat4(1.0f),glm::vec3(35.f, -5.,0.0f));
        
        /*for(int i = 0 ; i < resolution ; i++)
        for(int j = 0 ; j < resolution ; j++)
        {
            indexed_vertices2[i][j]=indexed_vertices2[i][j];
             
        }*/
        /*
        createMap2(  indices2,indexed_vertices2,indexed_uvs,  resolution);
             
        glGenBuffers(1, &vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, indexed_vertices2.size() * sizeof(glm::vec3), &indexed_vertices2[0], GL_STATIC_DRAW);
                glGenBuffers(1, &elementbuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices2.size() * sizeof(unsigned short), &indices2[0] , GL_STATIC_DRAW);
            
        glGenBuffers(1, &uvbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size()* sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

        modelmatrix = translationMatrix*rotationMatrix *scaleMatrix*idmatrix;
        glUniformMatrix4fv(modelID2, 1, GL_FALSE, glm::value_ptr(modelmatrix));
                glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                    0,                  // attribute
                    3,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    0,                  // stridedeltaTime
                    (void*)0           // element array buffer offset
                    );
        
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glVertexAttribPointer(
                    1,                  // attribute
                    2,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    0,                  // stridedeltaTime
                    (void*)0           // element array buffer offset
                    );

                glDrawElements(
                    GL_TRIANGLES,      // mode
                    indices2.size(),    // count
                    GL_UNSIGNED_SHORT,   // typecamera_position
                        (void*)0           // element array buffer offset
                    );

    */
    

        // Swap buffers
                // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    // Cleanup VBO and shader
    //glDeleteBuffers(1, &vertexbuffer);
    //glDeleteBuffers(1, &elementbuffer);
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
    if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
        camera_position += cameraSpeed * camera_target;
        
    if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
        camera_position -= cameraSpeed * camera_target;
    
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        camera_position -= cameraSpeed * glm::normalize(glm::cross(camera_target, camera_up));
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        camera_position += cameraSpeed * glm::normalize(glm::cross(camera_target, camera_up));
    
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera_position += cameraSpeed * camera_target;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera_position -= cameraSpeed * camera_target;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera_position += cameraSpeed * glm::vec3(0.,1.,0.);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera_position -= cameraSpeed * glm::vec3(0.,1.,0.);
    
    if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS)
        camera_target =  glm::rotateY(camera_target,angle);
    if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS)
        camera_target =  glm::rotateY(camera_target,-angle);
    
    if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS)
        camera_target =  glm::rotateX(camera_target,angle);
    if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS)
        camera_target =  glm::rotateX(camera_target,-angle);
    
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        coeffAngle2++;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        coeffAngle2 = coeffAngle2--<1?1:coeffAngle2;
    if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
    {

        resolution+=1;
        std::cout<<"resolution : "<<resolution<<std::endl;  
        
        createMap(  indices2,indexed_vertices2,indexed_uvs,  resolution);
    }
    if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
    {

        resolution = resolution-- ==1 ? 1: resolution;
        std::cout<<"resolution : "<<resolution<<std::endl;
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
             
             camera_position = glm::vec3(0.,40.,50.);
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
