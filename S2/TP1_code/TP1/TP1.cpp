// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <iterator>
#include <map>
#include <algorithm>
#include <string>

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
}
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
int find(std::vector<int> v, int K)
{
    return  find(v.begin(), v.end(), K)-v.begin();
}
int encodageRANS( std::vector<int> sequence,std::map<int,int> counts)
{
    std::vector<int> alphabet;
    std::vector<int> frequence;

    std::map<int, int>::iterator it;
    for (it = counts.begin(); it != counts.end(); it++)
    {
        alphabet.push_back(it->first);    
        frequence.push_back(it->second);
                
    }
    
    std::vector<int> cumul;
    cumul.push_back(0);
    int c =0;
    for(int i = 0; i <frequence.size()-1;i++)
    {
        c += frequence[i];
        cumul.push_back(c);
        
    }
    int  M = sequence.size();

    int x = 0,i=0;
    
    while(i<M)
    {
        int s = sequence[i];
        int fst =frequence[find(alphabet,s)];

        x = floor(x/fst)*M + cumul[find(alphabet,s)] + x%fst;
        //std::cout<<"X["<<i+1<<"]: "<<x<<"\n";
        i++;
    }
    return x;
}
int cinverse(std::vector<int> cumul,int x)
{
    int res =-1;
    int temp=-1;
    for(int i = 0 ; i < cumul.size();i++)
    {   
        if(cumul[i]<x)
        
            temp = i;
        else if(cumul[i]==x)
            res = i;
    }
    return res!=-1 ? res : temp;
}
std::vector<int> decodageRANS(int last, std::map<int,int> counts, int size)
{
    std::vector<int> alphabet;
    std::vector<int> frequence;

    std::map<int, int>::iterator it;
    for (it = counts.begin(); it != counts.end(); it++)
    {
        alphabet.push_back(it->first);    
        frequence.push_back(it->second);
                
    }
    
    std::vector<int> cumul;
    cumul.push_back(0);
    int c =0;
    for(int i = 0; i <frequence.size()-1;i++)
    {
        c += frequence[i];
        cumul.push_back(c);
    }
    int  M = size;
    int x;
    int i = M;

    std::vector<int> sequence;
    sequence.resize(M);
    while(i>0)
    {
        
        int slot = last%M;
        int st = cinverse(cumul,slot); 

        sequence[i-1]=alphabet[st];
        
        x = floor(last/M)*frequence[st] + slot -cumul[st];
        //std::cout<<"X["<<i<<"]: "<<x<<"\n";
        
        last = x;
        i--;
    }
    /*for(int i = 0 ; i < sequence.size();i++)
         std::cout<<sequence[i]<<" ";*/

   return sequence; 

}
bool isIn(std::vector<int> vec, int item)
{
    return std::find(vec.begin(), vec.end(), item) != vec.end();
}
std::map<int, int> count(std::vector<int> points)
{
    typedef std::map<int, int> CounterMap;
    CounterMap counts;
    for (int i = 0; i < points.size(); ++i)
    {
    CounterMap::iterator it(counts.find(points[i]));
    if (it != counts.end()){
        it->second++;
    } else {
        counts[points[i]] = 1;
    }
    }
    
    return counts;

}
void draco(std::vector<glm::vec3>  indexed_vertices, int qp,int packetSize)
{
    glm::vec3 mini = BBmin(indexed_vertices);
    glm::vec3 maxi = BBmax(indexed_vertices);
    glm::vec3 range = glm::vec3(abs(maxi[0]-mini[0]),abs(maxi[1]-mini[1]),abs(maxi[2]-mini[2]));
    float rangeMax = std::fmax( std::fmax(range[0],range[1]),range[2]);

    std::vector<int> input;
    
    std::vector<int> sequence;
    
    for(int i=0; i < indexed_vertices.size() ; i ++)
    {
        for(int j=0; j < 3 ; j ++)
        {

            sequence.push_back(quantification(qp,indexed_vertices[i][j], rangeMax, mini[j]));
            
        }
    }
    
    int sequenceSize=sequence.size(); 
    int nbPackets = sequenceSize/packetSize;
    int remaining = sequenceSize%packetSize;
   

    std::vector<std::vector<int>> sequence_chunks;
    std::vector<std::vector<int>> outputs;

    int j =0;
    for (int i = 0; i < nbPackets+1; ++i)
    {   
        std::vector<int> seq;
        while(j<packetSize && (i*packetSize +j)<sequenceSize)
        {
            seq.push_back(sequence[i*packetSize +j]);
            j++;
        }
        j=0;
        sequence_chunks.push_back(seq);
    }
    std::string outputString = "lapinCompression"+std::to_string(qp)+".mlm";
    FILE * fp = fopen(&outputString[0],"wb");
    std::vector<int> lasts;
    typedef std::map<int,int> CounterMap;
    for(int i = 0 ; i < nbPackets+1;i++)
    {   
        std::vector<int> sequence = sequence_chunks[i];
        CounterMap alphaSequence =count(sequence);
        int last = encodageRANS(sequence,alphaSequence);
        std::map<int,  int>::iterator it;
        for (it = alphaSequence.begin(); it != alphaSequence.end(); it++)
        {


                    fputc(it->first, fp);
                    fputc(':',fp);
                    fputc(it->second,fp);


        }
        fputc('s',fp);
        fputc(sequence.size(),fp);
        fputc(':',fp);
        fputc(last,fp);

        
        outputs.push_back(decodageRANS(last,alphaSequence,sequence.size()));

    }


     std::vector<int> output;
     for(int j = 0 ; j < nbPackets+1;j++)
        for(int i = 0 ; i < outputs[j].size();i++)
        {
           
            output.push_back(outputs[j][i]);
            
        }
        
    for(int i=0; i < output.size() ; i +=3)
    {
        for(int j=0; j < 3 ; j ++)
        {
            indexed_vertices[i/3][j] = dequantification(qp,output[i+j], rangeMax, mini[j]);
            
        }
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
    Pour créer plusieurs fichiers compressés du lapin
    for(int i = 5 ; i < 31 ; i++)
        draco(indexed_vertices, i,10);
    */

   // Pour faire une compression decompression à la volée 
    glm::vec3 mini = BBmin(indexed_vertices);
    glm::vec3 maxi = BBmax(indexed_vertices);
    glm::vec3 range = glm::vec3(abs(maxi[0]-mini[0]),abs(maxi[1]-mini[1]),abs(maxi[2]-mini[2]));
    float rangeMax = std::fmax( std::fmax(range[0],range[1]),range[2]);
    int qp =25;
    std::vector<int> input;

    
    std::vector<int> sequence;
    
    // On quantifie
    for(int i=0; i < indexed_vertices.size() ; i ++)
    {
        for(int j=0; j < 3 ; j ++)
        {

            sequence.push_back(quantification(qp,indexed_vertices[i][j], rangeMax, mini[j]));
            
        }
    }
    
    
    int packetSize = 8; 
    int sequenceSize=sequence.size(); 
    int nbPackets = sequenceSize/packetSize;
    int remaining = sequenceSize%packetSize;
   

    std::vector<std::vector<int>> sequence_chunks;
    std::vector<std::vector<int>> outputs;
    // On découpe en paquets
    int j =0;
    for (int i = 0; i < nbPackets+1; ++i)
    {   
        std::vector<int> seq;
        while(j<packetSize && (i*packetSize +j)<sequenceSize)
        {
            seq.push_back(sequence[i*packetSize +j]);
            j++;
        }
        j=0;
        sequence_chunks.push_back(seq);
    }
    
    // On encode en rANS dans un fichier
    std::cout<<"\nCodage...\n";
    FILE * fp = fopen("lapinCompression.mlm","wb");
    std::vector<int> lasts;
    typedef std::map<int,int> CounterMap;
    for(int i = 0 ; i < nbPackets+1;i++)
    {   
        std::vector<int> sequence = sequence_chunks[i];
        CounterMap alphaSequence =count(sequence);
        int last = encodageRANS(sequence,alphaSequence);
        std::map<int,  int>::iterator it;
        for (it = alphaSequence.begin(); it != alphaSequence.end(); it++)
        {

                    fputc(it->first, fp);
                    fputc(':',fp);
                    fputc(it->second,fp);


        }
        fputc('s',fp);
        fputc(sequence.size(),fp);
        fputc(':',fp);
        fputc(last,fp);

         //putc("sl:%d:%d;",sequence.size(), last,fp);

        // On récupère les paquets décodés
        outputs.push_back(decodageRANS(last,alphaSequence,sequence.size()));

    }
    // On concatène les paquets
     std::vector<int> output;
     for(int j = 0 ; j < nbPackets+1;j++)
        for(int i = 0 ; i < outputs[j].size();i++)
        {
           
            output.push_back(outputs[j][i]);
            
        }
        
    // On déquantifie la séquence
    for(int i=0; i < output.size() ; i +=3)
    {
        for(int j=0; j < 3 ; j ++)
        {

            indexed_vertices[i/3][j] = dequantification(qp,output[i+j], rangeMax, mini[j]);
            
        }
    }
    
    std::cout<<"\n Fini !\n";
    
    

    


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

    
        
        
        

    // Load it into a VBO



    GLuint uvbuffer;
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size()* sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

    // Get a handle for our "LightPosition" uniform
    glUseProgram(programID);
    GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

    //GLuint Texture = loadBMP_custom((char *)"maison.bmp");
    GLuint Texture = loadBMP_custom((char *)"hmap_rocky.bmp");
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

        rotationMatrix  = glm::rotate(glm::mat4(1.0f), (float)-M_PI_2, glm::vec3(1.,0.,0.0));
        rotationMatrix  *= glm::rotate(glm::mat4(1.0f), (float)M_PI/4, glm::vec3(0.,1.,0.0));
        rotationMatrix  *= glm::rotate(glm::mat4(1.0f), (float)M_PI_2, glm::vec3(0.,0.,1.0));

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
        //rotationMatrix  = glm::rotate(glm::mat4(1.0f), (float)-M_PI_2, glm::vec3(0.,0.,1.0));
        //rotationMatrix  = glm::rotate(glm::mat4(1.0f), -flip, glm::vec3(1.,0.,0.0));

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
