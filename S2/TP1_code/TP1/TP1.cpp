// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <iterator>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include <iostream>

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
float angleRotation = 0.;
float zoom = 1.;
/*******************************************************************************/
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
glm::vec3 BBmin(std::vector<glm::vec3> indexed_vertices)
{
    glm::vec3 mini= glm::vec3(FLT_MAX,FLT_MAX,FLT_MAX);

    for(auto& vert : indexed_vertices)
    {
        for(int i = 0;i <3;i++)
        {
            if(vert[i]<mini[i])
            {
                mini[i]=vert[i];
            }

        }

    }
    return mini;

}
glm::vec3 BBmax(std::vector<glm::vec3> indexed_vertices)
{
    glm::vec3 mini= glm::vec3(-FLT_MAX,-FLT_MAX,-FLT_MAX);

    for(auto& vert : indexed_vertices)
    {
        for(int i = 0;i <3;i++)
        {
            if(vert[i]>mini[i])
            {
                mini[i]=vert[i];
            }

        }

    }
    return mini;

}

float computeRange(float mini,float maxi)
{
    return abs(maxi-mini);
}/*
float quantification(int qp,float coordinate, float range, float BBC)
{
    return (float) (int)(coordinate *range/ pow(2,qp)) -BBC;
}
float dequantification(int qp,float coordinate, float range, float BBC)
{

    return (float)((coordinate+BBC)*pow(2,qp))/range;
}*/
float quantification(int qp,float coordinate, float range, float BBC)
{
    return (float) (int)( (coordinate-BBC) *pow(2,qp)/range ) ;
}
float dequantification(int qp,float coordinate, float range, float BBC)
{

    return (float)( coordinate* range)/pow(2,qp) + BBC;
}

float RMSE(std::vector<glm::vec3> mesh1,std::vector<glm::vec3> mesh2)
{
    float err = 0.;
    for(int i = 0 ; i < mesh1.size();i++)
    {
        
        glm::vec3 v((mesh1[i]-mesh2[i]));
        err += (v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
    }
    return sqrt(err / mesh1.size());
}
float compressMesure(std::vector<glm::vec3> indexed_vertices,std::vector<glm::vec3> vert_original, int qp)
{
    
    glm::vec3 mini = BBmin(indexed_vertices);
    glm::vec3 maxi = BBmax(indexed_vertices);
    glm::vec3 range = glm::vec3(abs(maxi[0]-mini[0]),abs(maxi[1]-mini[1]),abs(maxi[2]-mini[2]));
    float rangeMax = std::fmax( std::fmax(range[0],range[1]),range[2]);

    for(int i=0; i < indexed_vertices.size() ; i ++)
    {
        for(int j=0; j < 3 ; j ++)
        {
            indexed_vertices[i][j] = quantification(qp,indexed_vertices[i][j], rangeMax, mini[j]);
            indexed_vertices[i][j] = dequantification( qp,indexed_vertices[i][j], rangeMax, mini[j]);
        }
    }
    
    return RMSE(indexed_vertices,vert_original); 

}
void encodageRANS()
{
    std::vector<int> alphabet={0,1,2};
    std::vector<int> sequence={0,1,0,2,2,0};
    std::vector<int> frequence={3,1,2};
    std::vector<int> cumul={0,3,4};
    int  M = sequence.size();

    int x = 0,i=0;
    std::cout<<"X["<<i<<"]: "<<x<<"\n";
    while(i<M)
    {
        int s = sequence[i];
        int fst =frequence[s];
        
        x = floor(x/fst)*M + cumul[s] + x%fst;
        std::cout<<"X["<<i+1<<"]: "<<x<<"\n";
        i++;
    }

}
int cinverse(std::vector<int> cumul,int x)
{
    int res =-1;
    for(int i = 0 ; i < cumul.size();i++)
    {
        if(cumul[i]==x)
            res = i;
    }
    return res;
}
void decodageRANS(int last)
{
    std::vector<int> alphabet={0,1,2};
    std::vector<int> sequence={0,1,0,2,2,0};
    std::vector<int> frequence={3,1,2};
    std::vector<int> cumul={0,3,4};
    int  M = sequence.size();
    /*
    int x = 0,i=0;
    std::cout<<"X["<<i<<"]: "<<x<<"\n";
    */
    /*
    while(i>=0)
    {
        
        
        x = floor(x/fst)*M + cumul[s] + x%fst;
        std::cout<<"X["<<i+1<<"]: "<<x<<"\n";
        i++;
    }
    */
    int slot = last%M;
    std::cout<<cinverse(cumul,frequence[slot]);

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
    

   

    /*****************TODO***********************/
    // Get a handle for our "Model View Projection" matrices uniforms

    /****************************************/
    std::vector<unsigned short> indices; //Triangles concaténés dans une liste
    std::vector<std::vector<unsigned short> > triangles;
    std::vector<glm::vec3> indexed_vertices;
    std::vector<glm::vec3> vert_original;
    std::vector<glm::vec2> indexed_uvs;

    std::vector<unsigned short> indices2; 
    std::vector<glm::vec3> indexed_vertices2;

    //Chargement du fichier de maillage
    //std::string filename("chair.off");
    std::string filename("suzanne.off");
    loadOFF(filename, indexed_vertices, indices, triangles );
    loadOFF(filename, vert_original, indices, triangles );
    /*
    glm::vec3 mini = BBmin(indexed_vertices);
    glm::vec3 maxi = BBmax(indexed_vertices);
    glm::vec3 range = glm::vec3(abs(maxi[0]-mini[0]),abs(maxi[1]-mini[1]),abs(maxi[2]-mini[2]));
    float rangeMax = std::fmax( std::fmax(range[0],range[1]),range[2]);
    int qp =20;
    
    for(int i=0; i < indexed_vertices.size() ; i ++)
    {
        for(int j=0; j < 3 ; j ++)
        {
            indexed_vertices[i][j] = quantification(qp,indexed_vertices[i][j], rangeMax, mini[j]);
            indexed_vertices[i][j] = dequantification( qp,indexed_vertices[i][j], rangeMax, mini[j]);
        }
    }*/
       /* glm::vec3 mini = BBmin(indexed_vertices);
    glm::vec3 maxi = BBmax(indexed_vertices);
    glm::vec3 range = glm::vec3(abs(maxi[0]-mini[0]),abs(maxi[1]-mini[1]),abs(maxi[2]-mini[2]));
    float rangeMax = std::fmax( std::fmax(range[0],range[1]),range[2]);
    int qp = 1;
    for(int i=0; i < indexed_vertices.size() ; i ++)
    {
        for(int j=0; j < 3 ; j ++)
        {
            indexed_vertices[i][j] = quantification(qp,indexed_vertices[i][j], rangeMax, mini[j]);
            indexed_vertices[i][j] = dequantification( qp,indexed_vertices[i][j], rangeMax, mini[j]);
        }
    }*/
    //std::cout<<"RMSE: "<<RMSE(indexed_vertices,vert_original)<<"\n";
    //int qp =20;
    /*FILE *fp = fopen("rmse.dat", "w+");
    for(int i = 5 ; i < 30;i++ )
    {
        float rmse = compressMesure(indexed_vertices,vert_original, i);
        fprintf(fp, "%d %f\n", i ,rmse);

    }
    fclose(fp);*/
    encodageRANS();
    decodageRANS(139);



    int sommets = 128;
    float size = 0.2;
    unsigned int seed = 237;
    PerlinNoise pn(seed);
    std::vector<std::vector<double>> mapNoise;
    mapNoise.resize(sommets);
    for(auto& mn : mapNoise)mn.resize(sommets);

    std::vector<std::vector<double>> mapHeight;
    mapHeight.resize(sommets);
    for(auto& mn : mapHeight)mn.resize(sommets);

    ImageG imageG;
    int nH, nW;
    lire_nb_lignes_colonnes_image_pgm("heightmap.pgm", &nH, &nW);
    resize(imageG,nW,nH);
    
    loadImage("heightmap.pgm",imageG,nW, nH);

    //saveImage( folderOut, (char*)"originale_",inputName,imageG);

    for(int i = 0 ; i < 128 ; i++)
        for(int j = 0 ; j < 128 ; j++)
        {
            mapHeight[i][j]=(5.*(double)imageG[2*i][2*j])/255.;
        }
    for(int i = 0 ; i < sommets ; i++)
        for(int j = 0 ; j < sommets ; j++)
        {
            mapNoise[i][j] = (double)2.* OctavePerlin(j, i,0.8, 1,1.5) ;
            //std::cout<<mapNoise[i][j]<<std::endl;
        }
    
    PerlinNoise pn2(114);
    std::vector<std::vector<double>> mapNoise2;
    mapNoise2.resize(sommets);
    for(auto& mn : mapNoise2)mn.resize(sommets);

    for(int i = 0 ; i < sommets ; i+=2)
        for(int j = 0 ; j < sommets ; j+=2)
        {
            mapNoise2[i][j] = (double)2.* pn2.noise(j, i, 0.8) ;
            mapNoise2[i+1][j] = (double)2.* pn2.noise(j, i, 0.8) ;
            mapNoise2[i][j+1] = (double)2.* pn2.noise(j, i, 0.8) ;
            mapNoise2[i+1][j+1] = (double)2.* pn2.noise(j, i, 0.8) ;
            //std::cout<<mapNoise2[i][j]<<std::endl;
        }

    for(int i = 0 ; i < sommets ; i++)
        for(int j = 0 ; j < sommets ; j++)
        {
            indexed_vertices2.push_back( glm::vec3((float)i*size - (sommets/2)*size ,(float)j*size- (sommets/2)*size, mapHeight[i][j]+0.2*mapNoise[i][j]+0.01*mapNoise2[i][j] ));//size*rand()/RAND_MAX) );
            
             
        }
        
    for(int i = 0 ; i < sommets; i++)
        for(int j = 0; j < sommets ; j++)
        {


            indexed_uvs.push_back( glm::vec2(1.-(float)(i)/(sommets+1) ,1.-(float)(j)/(sommets+1) ));
   
        }
        

     for(int i = 0 ; i < sommets -1; i++)
        for(int j = 0 ; j < sommets-1 ; j++)
        {
            indices2.push_back(i*sommets +j);
            indices2.push_back(i*sommets +j+1); 
            indices2.push_back((i+1)*sommets +j); 

            indices2.push_back(i*sommets +j+1); 
            indices2.push_back((i+1)*sommets +j); 
            indices2.push_back((i+1)*sommets +j+1); 

        }
        
        
        

    // Load it into a VBO



    GLuint uvbuffer;
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size()* sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

    // Get a handle for our "LightPosition" uniform
    glUseProgram(programID);
    GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

    //GLuint Texture = loadBMP_custom((char *)"maison.bmp");
    GLuint Texture = loadBMP_custom((char *)"mountain2.bmp");
    GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Texture);

    glUniform1i(TextureID, 0);
     

    // For speed computation
    double lastTime = glfwGetTime();
    int nbFrames = 0;
    float angleRotation2=0.;
    
    float transflateFactor = 1.5;
    float scaleFactor =1.;
    glm::mat4 modelmatrix  = glm::mat4( 1.0f );
    glm::mat4 idmatrix  = glm::mat4( 1.0f );
    
    glm::mat4 projectionMatrix  = glm::perspective(45.,4./3.,0.1,100.);

    glm::mat4 scaleMatrix  = glm::scale(glm::mat4(1.0f),glm::vec3(1.));
    glm::mat4 rotationMatrix  = glm::rotate(glm::mat4(1.0f), 0.f, glm::vec3(1.0));
    glm::mat4 translationMatrix  = glm::translate(glm::mat4(1.0f),glm::vec3(0.f, 0.,0.0f));

    GLuint modelID = glGetUniformLocation(programID, "model");
    GLuint viewID = glGetUniformLocation(programID, "view");
    GLuint projectionID  = glGetUniformLocation(programID, "projection");


    do{

        // Measure speed
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime(); 
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);
        angleRotation2++;

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        glUseProgram(programID);
        glm::mat4 viewMatrix  = glm::lookAt(camera_position,camera_position+camera_target,camera_up);


        modelmatrix = translationMatrix*rotationMatrix *scaleMatrix*modelmatrix;

        glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(modelmatrix));
        glUniformMatrix4fv(viewID, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(projectionID, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

            GLuint vertexbuffer;
        glGenBuffers(1, &vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

        // Generate a buffer for the indices as well
        GLuint elementbuffer;
        glGenBuffers(1, &elementbuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);
        

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
        rotationMatrix  = glm::rotate(glm::mat4(1.0f), (float)radians(-angleRotation), glm::vec3(0.,0.,1.0));
        rotationMatrix  *= glm::rotate(glm::mat4(1.0f), (float)radians(-angleRotation2), glm::vec3(0.,1.,0.0));
        translationMatrix  = glm::translate(glm::mat4(1.0f),glm::vec3(-4.f, 2.,0.0f));

        modelmatrix = translationMatrix*rotationMatrix *scaleMatrix*idmatrix;
        glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(modelmatrix));

        // Draw the triangles !
        glDrawElements(
                    GL_TRIANGLES,      // mode
                    indices.size(),    // count
                    GL_UNSIGNED_SHORT,   // type
                    (void*)0           // element array buffer offset
                    );
        

        float flip = M_PI/4;
        scaleMatrix  = glm::scale(glm::mat4(1.0f),glm::vec3(1.5*scaleFactor));
        
        //rotationMatrix  = glm::rotate(glm::mat4(1.0f), flip, glm::vec3(1.,0.,0.0));
        rotationMatrix  = glm::rotate(glm::mat4(1.0f), (float)radians(angleRotation), glm::vec3(0.,0.,1.0));
        rotationMatrix  *= glm::rotate(glm::mat4(1.0f), (float)radians(angleRotation2), glm::vec3(0.,1.,0.0));
        //rotationMatrix  = glm::rotate(glm::mat4(1.0f), flip, glm::vec3(1.,0.,0.0));
        
        translationMatrix  = glm::translate(glm::mat4(1.0f),glm::vec3(0.f, 2.,0.0f));

        modelmatrix = translationMatrix*rotationMatrix *scaleMatrix*idmatrix;
        glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(modelmatrix));
        
        // Draw the triangles !
        glDrawElements(
                    GL_TRIANGLES,      // mode
                    indices.size(),    // count
                    GL_UNSIGNED_SHORT,   // type
                    (void*)0           // element array buffer offset
                    );
       

        scaleMatrix  = glm::scale(glm::mat4(1.0f),glm::vec3(1.5*scaleFactor));
        rotationMatrix  = glm::rotate(glm::mat4(1.0f), (float)M_PI/2, glm::vec3(-1.,0.,0.0));
        translationMatrix  = glm::translate(glm::mat4(1.0f),glm::vec3(-2.f, -5.,0.0f));

        modelmatrix = translationMatrix*rotationMatrix *scaleMatrix*idmatrix;
        glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(modelmatrix));

        glGenBuffers(1, &vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, indexed_vertices2.size() * sizeof(glm::vec3), &indexed_vertices2[0], GL_STATIC_DRAW);

        // Generate a buffer for the indices as well

        glGenBuffers(1, &elementbuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices2.size() * sizeof(unsigned short), &indices2[0] , GL_STATIC_DRAW);
        

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
    float cameraSpeed = 2.5 * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
        camera_position += cameraSpeed * camera_target;
        
    if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
        camera_position -= cameraSpeed * camera_target;
    
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        camera_position -= cameraSpeed * glm::vec3(1.,0.,0.);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        camera_position += cameraSpeed * glm::vec3(1.,0.,0.);
    
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera_position -= cameraSpeed * glm::vec3(0.,0.,1.);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera_position += cameraSpeed * glm::vec3(0.,0.,1.);

    if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS)
        camera_position += cameraSpeed * glm::vec3(0.,1.,0.);
    if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS)
        camera_position -= cameraSpeed * glm::vec3(0.,1.,0.);

    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        angleRotation ++;
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        angleRotation --;

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
