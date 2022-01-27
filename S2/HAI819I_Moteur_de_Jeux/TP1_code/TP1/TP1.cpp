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
#include <iostream>

using namespace glm;

#include <common/shader.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>

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

    //Chargement du fichier de maillage
    //std::string filename("chair.off");
    std::string filename("suzanne.off");
    //loadOFF(filename, indexed_vertices, indices, triangles );

    int sommets = 16;

for(int i = 0 ; i < sommets ; i++)
        for(int j = 0 ; j < sommets ; j++)
        {
            indexed_vertices.push_back( glm::vec3((float)i,(float)j,0.) );
             indices.push_back(i*sommets +j);
        }
        
    /*
    for(int i = 0 ; i < sommets-1 ; i++)
        for(int j = 0 ; j < sommets-1 ; j++)
        {
            triangles[k].push_back(i);
            triangles[k].push_back(i+1);
            triangles[k].push_back(i*sommets+j);
           

            triangles[k].push_back(i+1);
            triangles[k].push_back(i*sommets+j);
            triangles[k].push_back(i*sommets+j+1);

        }*/
        /*
        unsigned short k =0;
     for(int i = 0 ; i < indexed_vertices.size()/sommets ; i++)
     {
            triangles.push_back( std::vector<unsigned short>{ i, i+1, i*sommets });
            triangles.push_back( std::vector<unsigned short>{ i+1, i*sommets, i*sommets+1 });          
     }*/
     for(int i = 0 ; i < sommets -1; i++)
        for(int j = 0 ; j < sommets-1 ; j++)
        {
             triangles.push_back( std::vector<unsigned short>{ i*sommets +j, i*sommets +j+1, (i+1)*sommets +j });
            triangles.push_back( std::vector<unsigned short>{ i*sommets +j+1, (i+1)*sommets +j, (i+1)*sommets +j+1});      
        }
     /*
    indexed_vertices.push_back( glm::vec3(0.,0.,0.) );
    indexed_vertices.push_back( glm::vec3(1.,0.,0.) );
    indexed_vertices.push_back( glm::vec3(0.,1.,0.) );
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);

    indexed_vertices.push_back( glm::vec3(1.,1.,0.) );

    triangles.resize(1);
    triangles[0].push_back(0);
    triangles[0].push_back(1);
    triangles[0].push_back(2);
    
/*
    triangles[1].push_back(1);
    triangles[1].push_back(2);
    triangles[1].push_back(3);
    indices.push_back(1);*/

    // Load it into a VBO

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

    // Generate a buffer for the indices as well
    GLuint elementbuffer;
    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);

    // Get a handle for our "LightPosition" uniform
    glUseProgram(programID);
    GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");



    // For speed computation
    double lastTime = glfwGetTime();
    int nbFrames = 0;

    do{

        // Measure speed
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime(); 
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);


        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        glUseProgram(programID);


        /*****************TODO***********************/
        // Model matrix : an identity matrix (model will be at the origin) then change

        // View matrix : camera/view transformation lookat() utiliser camera_position camera_target camera_up

        // Projection matrix : 45 Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units

        // Send our transformation to the currently bound shader,
        // in the "Model View Projection" to the shader uniforms

        float transflateFactor = 1.5;
        float scaleFactor =1.;
        glm::mat4 modelmatrix  = glm::mat4( 1.0f );
         glm::mat4 idmatrix  = glm::mat4( 1.0f );
        glm::mat4 viewMatrix  = glm::lookAt(camera_position,camera_position+camera_target,camera_up);
        glm::mat4 projectionMatrix  = glm::perspective(45.,4./3.,0.1,100.);

        glm::mat4 scaleMatrix  = glm::scale(glm::mat4(1.0f),glm::vec3(scaleFactor));
        glm::mat4 rotationMatrix  = glm::rotate(glm::mat4(1.0f), 0.f, glm::vec3(1.0));
        glm::mat4 translationMatrix  = glm::translate(glm::mat4(1.0f),glm::vec3(-1.5f, -transflateFactor,0.0f));

        GLuint modelID = glGetUniformLocation(programID, "model");
        GLuint viewID = glGetUniformLocation(programID, "view");
        GLuint projectionID  = glGetUniformLocation(programID, "projection");

        modelmatrix = translationMatrix*rotationMatrix *scaleMatrix*modelmatrix;

        glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(modelmatrix));
        glUniformMatrix4fv(viewID, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(projectionID, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

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


        // Draw the triangles !
        glDrawElements(
                    GL_TRIANGLES,      // mode
                    indices.size(),    // count
                    GL_UNSIGNED_SHORT,   // type
                    (void*)0           // element array buffer offset
                    );
        /*
        transflateFactor = 1.;
        scaleFactor =1.;
        float flip = M_PI;
        scaleMatrix  = glm::scale(glm::mat4(1.0f),glm::vec3(scaleFactor));
        rotationMatrix  = glm::rotate(glm::mat4(1.0f), flip, glm::vec3(0.,1.,0.0));
        translationMatrix  = glm::translate(glm::mat4(1.0f),glm::vec3(1.5f, -1.5,0.0f));

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
        rotationMatrix  = glm::rotate(glm::mat4(1.0f), (float)radians(angleRotation), glm::vec3(0.,0.,1.0));
        translationMatrix  = glm::translate(glm::mat4(1.0f),glm::vec3(0.f, 0.,0.0f));

        modelmatrix = translationMatrix*rotationMatrix *scaleMatrix*idmatrix;
        glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(modelmatrix));

                 // Draw the triangles !
        glDrawElements(
                    GL_TRIANGLES,      // mode
                    indices.size(),    // count
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
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &elementbuffer);
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
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera_position += cameraSpeed * camera_target;
        
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera_position -= cameraSpeed * camera_target;
    
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera_position += cameraSpeed * glm::vec3(1.,0.,0.);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera_position -= cameraSpeed * glm::vec3(1.,0.,0.);


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
