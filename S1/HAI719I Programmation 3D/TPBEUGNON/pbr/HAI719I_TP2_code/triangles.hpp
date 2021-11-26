GLuint programID;

float scale;
Vec3 translate;

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

    GLuint vertexbuffer, colorbuffer;

    void initBuffers(){
        static const GLfloat g_vertex_buffer_data[] = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            1.0f,  1.0f, 0.0f,
        };

        static const GLfloat g_color_buffer_data[] = {
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
        };


        glGenBuffers(1,&vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER,sizeof(g_vertex_buffer_data),g_vertex_buffer_data,GL_STATIC_DRAW);


        
       
        glGenBuffers(1,&colorbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glBufferData(GL_ARRAY_BUFFER,sizeof(g_color_buffer_data),g_color_buffer_data,GL_STATIC_DRAW);


    }

    void clearBuffers(){
        //Liberer la memoire, utiliser glDeleteBuffers
        glDeleteBuffers(1, &vertexbuffer);
        glDeleteBuffers(1, &colorbuffer);


    }

    void draw (){     
           
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
        

        //Ajouter un attribut dans un color buffer à envoyé au GPU
        //Utiliser glVertexAttribPointer
        // 2nd attribute buffer : color

        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, 0);



        // Draw the triangle !
        glDrawArrays( GL_TRIANGLES,0,3);

        // En commentaire pour afficher le triangle vao 
        //glDisableVertexAttribArray(0);
        //glDisableVertexAttribArray(1);

        
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

    void initTriangleMesh(){
        vertices.clear();
        std::vector<Vec3> g_vertex_buffer_data {
                    Vec3(-1.0f, 1.0f, 0.0f),Vec3(1.0f, 0.0f, 0.0f),
                    Vec3(-1.0f, -1.0f, 0.0f),Vec3(0.0f, 1.0f, 0.0f),
                    Vec3(1.0f,  1.0f, 0.0f),Vec3(0.0f, 0.0f, 1.0f),
        };

        vertices = g_vertex_buffer_data;
        normals = g_vertex_buffer_data;

        triangles.push_back( Triangle(0,1,2) );
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

    void clearBuffers(){
        //Liberer la memoire, utiliser glDeleteBuffers
        glDeleteBuffers( 1, &vertexbuffer);

    }

    void draw (){

        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
         // Stride de 2* size vec3 et offset de 0 pour la position et 1 pour la couleur
        glVertexAttribPointer(0, 3, GL_FLOAT, false,2*sizeof(Vec3), (GLvoid*)(0*sizeof(Vec3)));

        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(1, 3, GL_FLOAT, false,2*sizeof(Vec3), (GLvoid*)(sizeof(Vec3)) );

        glBindVertexArray(vaoID);
        glDrawArrays(GL_TRIANGLES,0,3);
        glBindVertexArray(0);

    }
};


TriangleVArray first_triangle;
Mesh triangle_mesh;
MeshVAO triangle_mesh2;

Mesh mesh;