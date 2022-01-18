#include "triangles.hpp"

bool display_normals;
bool display_loaded_mesh;

// -------------------------------------------
// OpenGL/GLUT application code.
// -------------------------------------------

static GLint window;
static unsigned int SCREENWIDTH = 1600;
static unsigned int SCREENHEIGHT = 900;
static Camera camera;
static bool mouseRotatePressed = false;
static bool mouseMovePressed = false;
static bool mouseZoomPressed = false;
static int lastX=0, lastY=0, lastZoom=0;
static bool fullScreen = false;

// ------------------------------------
bool saveOFF( const std::string & filename ,
              std::vector< Vec3 > & i_vertices ,
              std::vector< Vec3 > & i_normals ,
              std::vector< Triangle > & i_triangles,
              bool save_normals = true ) {
    std::ofstream myfile;
    myfile.open(filename.c_str());
    if (!myfile.is_open()) {
        std::cout << filename << " cannot be opened" << std::endl;
        return false;
    }

    myfile << "OFF" << std::endl ;

    unsigned int n_vertices = i_vertices.size() , n_triangles = i_triangles.size();
    myfile << n_vertices << " " << n_triangles << " 0" << std::endl;

    for( unsigned int v = 0 ; v < n_vertices ; ++v ) {
        myfile << i_vertices[v][0] << " " << i_vertices[v][1] << " " << i_vertices[v][2] << " ";
        if (save_normals) myfile << i_normals[v][0] << " " << i_normals[v][1] << " " << i_normals[v][2] << std::endl;
        else myfile << std::endl;
    }
    for( unsigned int f = 0 ; f < n_triangles ; ++f ) {
        myfile << 3 << " " << i_triangles[f][0] << " " << i_triangles[f][1] << " " << i_triangles[f][2];
        myfile << std::endl;
    }
    myfile.close();
    return true;
}

void openOFF( std::string const & filename,
              std::vector<Vec3> & o_vertices,
              std::vector<Vec3> & o_normals,
              std::vector< Triangle > & o_triangles,
              bool load_normals = true )
{
    std::ifstream myfile;
    myfile.open(filename.c_str());
    if (!myfile.is_open())
    {
        std::cout << filename << " cannot be opened" << std::endl;
        return;
    }

    std::string magic_s;

    myfile >> magic_s;

    if( magic_s != "OFF" )
    {
        std::cout << magic_s << " != OFF :   We handle ONLY *.off files." << std::endl;
        myfile.close();
        exit(1);
    }

    int n_vertices , n_faces , dummy_int;
    myfile >> n_vertices >> n_faces >> dummy_int;

    o_vertices.clear();
    o_normals.clear();

    for( int v = 0 ; v < n_vertices ; ++v )
    {
        float x , y , z ;

        myfile >> x >> y >> z ;
        o_vertices.push_back( Vec3( x , y , z ) );

        if( load_normals ) {
            myfile >> x >> y >> z;
            o_normals.push_back( Vec3( x , y , z ) );
        }
    }

    o_triangles.clear();
    for( int f = 0 ; f < n_faces ; ++f )
    {
        int n_vertices_on_face;
        myfile >> n_vertices_on_face;

        if( n_vertices_on_face == 3 )
        {
            unsigned int _v1 , _v2 , _v3;
            myfile >> _v1 >> _v2 >> _v3;

            o_triangles.push_back(Triangle( _v1, _v2, _v3 ));
        }
        else if( n_vertices_on_face == 4 )
        {
            unsigned int _v1 , _v2 , _v3 , _v4;
            myfile >> _v1 >> _v2 >> _v3 >> _v4;

            o_triangles.push_back(Triangle(_v1, _v2, _v3 ));
            o_triangles.push_back(Triangle(_v1, _v3, _v4));
        }
        else
        {
            std::cout << "We handle ONLY *.off files with 3 or 4 vertices per face" << std::endl;
            myfile.close();
            exit(1);
        }
    }

}

void initLight () {
    GLfloat light_position1[4] = {22.0f, 16.0f, 50.0f, 0.0f};
    GLfloat direction1[3] = {-52.0f,-16.0f,-50.0f};
    GLfloat color1[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat ambient[4] = {0.3f, 0.3f, 0.3f, 0.5f};

    glLightfv (GL_LIGHT1, GL_POSITION, light_position1);
    glLightfv (GL_LIGHT1, GL_SPOT_DIRECTION, direction1);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, color1);
    glLightfv (GL_LIGHT1, GL_SPECULAR, color1);
    glLightModelfv (GL_LIGHT_MODEL_AMBIENT, ambient);
    glEnable (GL_LIGHT1);
    glEnable (GL_LIGHTING);
}
glm::mat4 cam(Vec3 Translate, glm::vec2 const& Rotate)
{
	

    View = glm::translate(glm::mat4(1.0f), glm::vec3( Translate[0], Translate[1], Translate[2]));
	View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
	View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
	return   View * Model;
}

void init () {
    camera.resize (SCREENWIDTH, SCREENHEIGHT);
    initLight ();
    glCullFace (GL_BACK);
    glEnable (GL_CULL_FACE);
    glDepthFunc (GL_LESS);
    glEnable (GL_DEPTH_TEST);
    glClearColor (0.2f, 0.2f, 0.3f, 1.0f);
    glEnable(GL_COLOR_MATERIAL);

    display_normals = false;
    display_loaded_mesh = true;



    Scale  =2.;
    Translate = Vec3(0.,0.,-.8);
    View = glm::translate(glm::mat4(1.0f), glm::vec3( Translate[0], Translate[1], Translate[2]));
    rotor = glm::vec2(0.,0.);
    MV = cam(Translate,rotor);
    P = glm::perspective(45.0f, (float)16./9.f, 0.1f, 100.0f);

    

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return;
    }

}



// ------------------------------------
// rendering.
// ------------------------------------


void drawVector( Vec3 const & i_from, Vec3 const & i_to ) {

    glBegin(GL_LINES);
    glVertex3f( i_from[0] , i_from[1] , i_from[2] );
    glVertex3f( i_to[0] , i_to[1] , i_to[2] );
    glEnd();
}

void draw () {


    glClear( GL_COLOR_BUFFER_BIT );

    glUseProgram(programID);
    lightPos= Vec3(0.5,0.,-0.8);//-0.5);

    Vec3 albedo(1.,0.,0.);
    Vec3 color(0.,1.,2.);
	float roughness=0.5;
	float metalness = 0.5;

    
    glUniform1i(glGetUniformLocation(programID, "texColor"), 0);
    glUniform1i(glGetUniformLocation(programID, "texMetal"), 1);
    glUniform1i(glGetUniformLocation(programID, "texNormal"), 2);
    glUniform1i(glGetUniformLocation(programID, "texRough"), 3);

   
    glUniform1f(glGetUniformLocation(programID, "roughness"),roughness);
    glUniform1f(glGetUniformLocation(programID, "metalness"),metalness);

    glUniform3f(glGetUniformLocation(programID, "albedo"),albedo[0], albedo[1], albedo[2]);
    glUniform3f(glGetUniformLocation(programID, "lightPos"),lightPos[0], lightPos[1], lightPos[2]);

    //View = glm::translate(glm::mat4(1.0f), glm::vec3(Translate,  -Translate,0.0f));
    //View = glm::translate(glm::mat4(1.0f), glm::vec3( Translate[0], Translate[1], Translate[2]));
    //glUniformMatrix4fv(idTranslate,1, GL_FALSE, glm::value_ptr(View));

    MV = cam(Translate,rotor);
    glUniform1f(glGetUniformLocation(programID, "scale"),Scale);
    glUniformMatrix4fv(glGetUniformLocation(programID, "translate"), 1, GL_FALSE, glm::value_ptr(MV));

    
    glUniformMatrix4fv(glGetUniformLocation(programID, "projection"), 1, GL_FALSE, glm::value_ptr(P));


    triangle_mesh.draw();
    //drawTriangleMesh( mesh ,  false  );



    glDisable(GL_LIGHTING);

    glEnable(GL_LIGHTING);
}


void display () {
    glLoadIdentity ();
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera.apply ();
    draw ();
    glFlush ();
    glutSwapBuffers ();
}

void idle () {
    glutPostRedisplay ();
}

void key (unsigned char keyPressed, int x, int y) {
    switch (keyPressed) {

    case 'w':
        GLint polygonMode[2];
        glGetIntegerv(GL_POLYGON_MODE, polygonMode);
        if(polygonMode[0] != GL_FILL)
            glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
        else
            glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
        break;


    case 'n': //Press n key to display normals
        display_normals = !display_normals;
        break;

    case 'd': //Press d key to translate on x positive
        //Completer : mettre à jour le x du Vec3 translate
    Translate[0]+=0.05;
        break;

    case 'q': //Press q key to translate on x negative
        //Completer : mettre à jour le y du Vec3 translate
    Translate[0]-=0.05;
        break;

    case 'z': //Press z key to translate on y positive
        //Completer : mettre à jour le y du Vec3 translate
    Translate[1]+=0.05;
        break;

    case 's': //Press s key to translate on y negative
        //Completer : mettre à jour le y du Vec3 translate
    Translate[1]-=0.05;
        break;

    case 'p': //Press z key to translate on y positive
        //Completer : mettre à jour le y du Vec3 translate
    Translate[2]+=0.05;
        break;

    case 'o': //Press s key to translate on y negative
        //Completer : mettre à jour le y du Vec3 translate
    Translate[2]-=0.05;
        break;

    case 'r': //Press z key to translate on y positive
        //Completer : mettre à jour le y du Vec3 translate
    rotor[0]+=0.05;
        break;

    case 't': //Press s key to translate on y negative
        //Completer : mettre à jour le y du Vec3 translate
    rotor[0]-=0.05;
        break;
    case 'f': //Press z key to translate on y positive
        //Completer : mettre à jour le y du Vec3 translate
    rotor[1]+=0.05;
        break;

    case 'g': //Press s key to translate on y negative
        //Completer : mettre à jour le y du Vec3 translate
    rotor[1]-=0.05;
        break;

    case '1': //Toggle loaded mesh display
        display_loaded_mesh = !display_loaded_mesh;
        break;

    default:
        break;
    }
    idle ();
}

void mouse (int button, int state, int x, int y) {
    if (state == GLUT_UP) {
        mouseMovePressed = false;
        mouseRotatePressed = false;
        mouseZoomPressed = false;
    } else {
        if (button == GLUT_LEFT_BUTTON) {
            camera.beginRotate (x, y);
            mouseMovePressed = false;
            mouseRotatePressed = true;
            mouseZoomPressed = false;
        } else if (button == GLUT_RIGHT_BUTTON) {
            lastX = x;
            lastY = y;
            mouseMovePressed = true;
            mouseRotatePressed = false;
            mouseZoomPressed = false;
        } else if (button == GLUT_MIDDLE_BUTTON) {
            if (mouseZoomPressed == false) {
                lastZoom = y;
                mouseMovePressed = false;
                mouseRotatePressed = false;
                mouseZoomPressed = true;
            }
        }
    }
    idle ();
}

void motion (int x, int y) {
    if (mouseRotatePressed == true) {
        camera.rotate (x, y);
    }
    else if (mouseMovePressed == true) {
        camera.move ((x-lastX)/static_cast<float>(SCREENWIDTH), (lastY-y)/static_cast<float>(SCREENHEIGHT), 0.0);
        lastX = x;
        lastY = y;
    }
    else if (mouseZoomPressed == true) {
        camera.zoom (float (y-lastZoom)/SCREENHEIGHT);
        lastZoom = y;
    }
}


void reshape(int w, int h) {
    camera.resize (w, h);
}

