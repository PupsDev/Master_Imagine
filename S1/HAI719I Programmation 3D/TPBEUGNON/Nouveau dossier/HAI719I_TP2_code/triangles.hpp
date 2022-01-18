GLuint programID;
 #define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
float Scale;
Vec3 Translate ;
glm::mat4 View;
glm::mat4 Rotation;
glm::mat4 MVP;
glm::vec2 rotor;

struct Triangle {
    inline Triangle () {
        v[0] = v[1] = v[2] = 0;
    }
    inline Triangle (const Triangle & t) {
        v[0] = t.v[0];   v[1] = t.v[1];   v[2] = t.v[2];
    }
    inline Triangle (unsigned int v0, unsigned int v1, unsigned int v2) {
        v[0] = v0;   v[1] = v1;   v[2] = v2;
    }
    unsigned int & operator [] (unsigned int iv) { return v[iv]; }
    unsigned int operator [] (unsigned int iv) const { return v[iv]; }
    inline virtual ~Triangle () {}
    inline Triangle & operator = (const Triangle & t) {
        v[0] = t.v[0];   v[1] = t.v[1];   v[2] = t.v[2];
        return (*this);
    }
    // membres :
    unsigned int v[3];
};


struct TriangleVArray {

    GLuint vertexbuffer, colorbuffer,texbuffer;
    unsigned int VBO, VAO, EBO;
    unsigned int texture1;

    void initBuffers(){
        /*
        static const GLfloat g_vertex_buffer_data[] = {
            -1.0f, 1.0f, 1.f, //TL
            -1.0f, -1.0f, 1.f, //BL
            1.0f,  1.0f, 0.0f, //TR
            1.0f, -1.0f, 0.0f,
        };*/
        static const GLfloat g_vertex_buffer_data[] = {
            -1.0f, 1.0f, 0.f, //TL
            -1.0f, -1.0f, 0.f, //BL
            1.0f,  1.0f, 0.0f, //TR
            1.0f, -1.0f, 0.0f,

            -1.0f-1., 1.0f, 3.f, //TL
            -1.0f-1., -1.0f, 3.f, //BL
            1.0f,  1.0f, 3.0f, //TR
            1.0f, -1.0f, 3.0f,
        };


        static const GLfloat g_color_buffer_data[] = {
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
        };

        static const GLfloat g_tex_buffer_data[] = {
            0.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,

            0.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
        };

        unsigned int indices[] = {
            0, 1, 2, // first triangle
            2, 1, 3,  // second triangle

            4, 5, 6, // first triangle
            6, 5, 7  // second triangle
        };
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


        glGenBuffers(1,&vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER,sizeof(g_vertex_buffer_data),g_vertex_buffer_data,GL_STATIC_DRAW);
       
        glGenBuffers(1,&colorbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glBufferData(GL_ARRAY_BUFFER,sizeof(g_color_buffer_data),g_color_buffer_data,GL_STATIC_DRAW);

        glGenBuffers(1,&texbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, texbuffer);
        glBufferData(GL_ARRAY_BUFFER,sizeof(g_tex_buffer_data),g_tex_buffer_data,GL_STATIC_DRAW);
        loadTexture();


    }

    void clearBuffers(){
        //Liberer la memoire, utiliser glDeleteBuffers
        glDeleteBuffers(1, &vertexbuffer);
        glDeleteBuffers(1, &colorbuffer);
        glDeleteBuffers(1, &texbuffer);


    }
    void loadTexture()
    {
        // texture 1
        // ---------
        glGenTextures(1, &texture1);
        glBindTexture(GL_TEXTURE_2D, texture1); 
         // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   // set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load image, create texture and generate mipmaps
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
        
        unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);

    }

    void draw (){     
           
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
        

        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, texbuffer);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, false, 0, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        // Draw the triangle !
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
        
        
    }
};
struct Mesh {
    std::vector< Vec3 > vertices;
    std::vector< Vec3 > normals;
    std::vector< Triangle > triangles;
     std::vector<unsigned int> indices;

    GLuint vertexbuffer, colorbuffer, elementbuffer;

    void initTriangleMesh(){
        
        std::vector<Vec3> g_vertex_buffer_data {
                    Vec3(-1.0f, 1.0f, 0.0f),
                    Vec3(-1.0f, -1.0f, 0.0f),
                    Vec3(1.0f,  1.0f, 0.0f),
        };

        std::vector<Vec3> g_color_buffer_data {
            Vec3(1.0f, 0.0f, 0.0f),
                    Vec3(0.0f, 1.0f, 0.0f),
                    Vec3(0.0f, 0.0f, 1.0f),
        };

        vertices = g_vertex_buffer_data;
        normals = g_color_buffer_data;

        triangles.push_back( Triangle(0,1,2) );
    }

    void initBuffers(){


        glGenBuffers(1,&vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER,sizeof(Vec3)*vertices.size(),&vertices[0],GL_STATIC_DRAW);

        // Creer un deuxieme buffer contenant les couleurs
        // a mettre dans le layout 1
        // Utiliser
         
        glGenBuffers(1,&colorbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glBufferData(GL_ARRAY_BUFFER,sizeof(Vec3)*normals.size(),&normals[0],GL_STATIC_DRAW);

        //Remplir indices avec la liste des indices des triangles concatenes
        Triangle lastTriangle = triangles.back();

        for(size_t i=0 ; i < 3 ; i++ )
        {
            indices.push_back(lastTriangle[i]);
        }


        glGenBuffers(1,&elementbuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(unsigned int) * indices.size(),&indices[0],GL_STATIC_DRAW);


    }

    void clearBuffers(){
        //Liberer la memoire, utiliser glDeleteBuffers
        glDeleteBuffers( 1, &vertexbuffer);
        glDeleteBuffers( 1, &colorbuffer);
        glDeleteBuffers( 1, &elementbuffer);
    }

    void draw (){

        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
        

        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);


        glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_INT,(void*)0);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

    }
};
struct MeshVAO {
      std::vector< Vec3 > vertices;
    std::vector< Vec3 > normals;
    std::vector< Triangle > triangles;
     std::vector<unsigned int> indices;

    GLuint vertexbuffer;
    GLuint vaoID;

    unsigned int texture1, texture2;

    void initTriangleMesh(){



        vertices.clear();
        /*std::vector<Vec3> g_vertex_buffer_data {
                    Vec3(-1.0f, 1.0f, 0.0f),Vec3(1.0f, 0.0f, 0.0f),
                    Vec3(-1.0f, -1.0f, 0.0f),Vec3(0.0f, 1.0f, 0.0f),
                    Vec3(1.0f,  1.0f, 0.0f),Vec3(0.0f, 0.0f, 1.0f),
        };*/
        std::vector<Vec3> g_vertex_buffer_data {

                    Vec3(-1.0f, 1.0f, 0.0f),Vec3(1.0f, 0.0f, 0.0f),Vec3(0.0f, 1.0f, 0.0f),//top left

                    Vec3(-1.0f, -1.0f, 0.0f),Vec3(0.0f, 1.0f, 0.0f),Vec3(0.0f, 0.0f, 0.0f),//bottom lef
                    Vec3(1.0f,  1.0f, 0.0f),Vec3(0.0f, 0.0f, 1.0f),Vec3(1.0f, 1.0f, 0.0f), // top right

                    

                    Vec3(-1.0f, -1.0f, 0.0f),Vec3(1.0f, 0.0f, 0.0f),Vec3(0.0f, 0.0f, 0.0f),   
                    Vec3(1.0f, -1.0f, 0.0f),Vec3(0.0f, 1.0f, 0.0f),Vec3(1.0f, 0.f, 0.0f),//bottom right    
                    Vec3(1.0f,  1.0f, 0.0f),Vec3(0.0f, 0.0f, 1.0f),Vec3(1.0f, 1.0f, 0.0f),

                   
        };
        float texCoord[] = {
        1.0f, 1.0f, 
          1.0f, 0.0f, 
          0.0f, 0.0f, 
        0.0f, 1.0f,
        };

        vertices = g_vertex_buffer_data;
        normals = g_vertex_buffer_data;

        //triangles.push_back( Triangle(0,1,2) );
        //triangles.push_back( Triangle(0,1,3) );
    }

    void initBuffers(){



        glGenBuffers(1,&vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER,sizeof(Vec3)*vertices.size(),&vertices[0],GL_STATIC_DRAW);

      
        glGenVertexArrays(1, &vaoID);
        glBindVertexArray(vaoID);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

    }
    void loadTexture()
    {
        // texture 1
        // ---------
        glGenTextures(1, &texture1);
        glBindTexture(GL_TEXTURE_2D, texture1); 
         // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   // set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load image, create texture and generate mipmaps
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
        
        unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);

    }

    void clearBuffers(){
        //Liberer la memoire, utiliser glDeleteBuffers
        glDeleteBuffers( 1, &vertexbuffer);

    }

    void draw (){

        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);


        //POs
         // Stride de 2* size vec3 et offset de 0 pour la position et 1 pour la couleur
        glVertexAttribPointer(0, 3, GL_FLOAT, false,3*sizeof(Vec3), (GLvoid*)(0*sizeof(Vec3)));

        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        //COLOR
        glVertexAttribPointer(1, 3, GL_FLOAT, false,3*sizeof(Vec3), (GLvoid*)(sizeof(Vec3)) );


        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        //Texture
        glVertexAttribPointer(2, 2, GL_FLOAT, false,3*sizeof(Vec3), (GLvoid*)(2*sizeof(Vec3)) );

        loadTexture();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glBindVertexArray(vaoID);
        glDrawArrays(GL_TRIANGLES,0,6);
        glBindVertexArray(0);

    }
};

Mesh mesh;

TriangleVArray triangle_mesh;
//MeshVAO triangle_mesh;
//MeshVAO triangle_mesh2;
