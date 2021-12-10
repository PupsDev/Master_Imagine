// -------------------------------------------
// gMini : a minimal OpenGL/GLUT application
// for 3D graphics.
// Copyright (C) 2006-2008 Tamy Boubekeur
// All rights reserved.
// -------------------------------------------

// -------------------------------------------
// Disclaimer: this code is dirty in the
// meaning that there is no attention paid to
// proper class attribute access, memory
// management or optimisation of any kind. It
// is designed for quick-and-dirty testing
// purpose.
// -------------------------------------------

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <stack>
#include <algorithm>
#include <GL/glut.h>
#include <float.h>
#include "src/Vec3.h"
#include "src/Camera.h"

enum DisplayMode{ WIRE=0, SOLID=1, LIGHTED_WIRE=2, LIGHTED=3 };
std::vector< Vec3 > joints;
std::vector< Vec3 > convex;


//Transformation made of a rotation and translation
struct Transformation {
    Mat3 rotation;
    Vec3 translation;
};

/*
Vec3 i=Vec3(1.,0.,0.);
Vec3 j=Vec3(0.,1.,0.);
Vec3 j=Vec3(0.,0.,1.);
*/
/*
Base base;
base.origin = Vec3(0.,0.,0.);
base.i = i; 
base.j = j;
*/

//Basis ( origin, i, j ,k )
struct Basis {
    inline Basis ( Vec3 const & i_origin,  Vec3 const & i_i, Vec3 const & i_j, Vec3 const & i_k) {
        origin = i_origin; i = i_i ; j = i_j ; k = i_k;
    }

    inline Basis ( ) {
        origin = Vec3(0., 0., 0.);
        i = Vec3(1., 0., 0.) ; j = Vec3(0., 1., 0.) ; k = Vec3(0., 0., 1.);
    }
    Vec3 operator [] (unsigned int ib) {
        if(ib==0) return i;
        if(ib==1) return j;
        return k;}

    Vec3 origin;
    Vec3 i;
    Vec3 j;
    Vec3 k;
};

bool inVector(std::vector<unsigned int> vec, unsigned int item)
{
    return std::find(vec.begin(), vec.end(), item) != vec.end();
}
void insertInRing(std::vector<unsigned int> &vec, unsigned int item)
{
    
    if( ! inVector(vec, item))
    {
        vec.push_back(item);

    }
    

}



Basis basis;

bool display_normals;
bool display_smooth_normals;
bool display_mesh;
bool display_basis;
DisplayMode displayMode;
int weight_type;
int algo = 0;

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
static int lastX=0, lastY=2000, lastZoom=2000;
static bool fullScreen = false;

// ------------------------------------
// Application initialization
// ------------------------------------
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

void init () {
    camera.resize (SCREENWIDTH, SCREENHEIGHT);
    initLight ();
    glCullFace (GL_BACK);
    glDisable (GL_CULL_FACE);
    glDepthFunc (GL_LESS);
    glEnable (GL_DEPTH_TEST);
    glClearColor (0.2f, 0.2f, 0.3f, 1.0f);
    glEnable(GL_COLOR_MATERIAL);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    display_normals = false;
    display_mesh = true;
    display_smooth_normals = true;
    displayMode = SOLID;
    display_basis = true;
}



//Draw fonction
void drawBasis(Basis base)
{
    glBegin(GL_LINE_STRIP);
    glColor3f(1.,0.,0.);
    glVertex3f(base.origin[0],base.origin[1],base.origin[2]);
    glVertex3f(base.i[0],base.i[1],base.i[2]);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glColor3f(0.,1.,0.);
    glVertex3f(base.origin[0],base.origin[1],base.origin[2]);
    glVertex3f(base.j[0],base.j[1],base.j[2]);
    glEnd();


}
void draw (std::vector<Vec3> & o_vertices, Vec3 color, int algo) {
/*
    if(algo==0)
        glBegin(GL_LINES);
    else
        glBegin(GL_LINE_STRIP);
    float c = (float)1./(float)o_vertices.size();
     
    for(size_t i = 0 ; i < o_vertices.size() ; i++)
    {
        Vec3 color = Vec3(c*i,0.2,1.-(c*i));
        if(i==0)
            color =Vec3(0.0,1.0,0.0);

        glColor3f(color[0],color[1],color[2]);
        glVertex3f(o_vertices[i][0], o_vertices[i][1], o_vertices[i][2]);
                

    }
    glEnd();
    */
    glPointSize(10.);
    glBegin(GL_POINTS);
    glColor3f(color[0],color[1],color[2]);
    for(size_t i = 0 ; i < o_vertices.size() ; i++)
    {
       
        glVertex3f(o_vertices[i][0], o_vertices[i][1], o_vertices[i][2]);
    }

    glEnd();

    glBegin(GL_LINE_STRIP);
    glColor3f(color[1],color[0],color[2]);
    for(size_t i = 0 ; i < o_vertices.size() ; i++)
    {
       
        glVertex3f(o_vertices[i][0], o_vertices[i][1], o_vertices[i][2]);
    }

    glEnd();


}
void drawCloud (std::vector<Vec3> & o_vertices, Vec3 color) {


    glColor3f(color[0],color[1],color[2]);
    glPointSize(5.);
    glBegin(GL_POINTS);
    
    for(size_t i = 0 ; i < o_vertices.size() ; i++)
    {
        glVertex3f(o_vertices[i][0], o_vertices[i][1], o_vertices[i][2]);

    }

    glEnd();


}

void display () {
    glLoadIdentity ();
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera.apply ();
    Vec3 Red(1.0,0.0,0.0);
    Vec3 Green(0.0,1.0,0.0);
    Vec3 Blue(0.0,0.0,1.0);


    draw (joints, Red,0);
    drawBasis(basis);
    
    glFlush ();
    glutSwapBuffers ();
}

void idle () {
    glutPostRedisplay ();
}
std::vector <Vec3> initRandomPoints(size_t n, unsigned int  w, unsigned int  h){
    std::vector <Vec3> points;
    for (size_t i = 0; i < n; i++){
        points.push_back(Vec3((float)(12 + ((w - 40) * rand())%w-800.) , (float)(20 + ((h - 40) * rand())%h-500.), 1.));
    }
    return points;
} 


// ------------------------------------
// User inputs
// ------------------------------------
//Keyboard event
void key (unsigned char keyPressed, int x, int y) {
    switch (keyPressed) {
    case 'f':
        if (fullScreen == true) {
            glutReshapeWindow (SCREENWIDTH, SCREENHEIGHT);
            fullScreen = false;
        } else {
            glutFullScreen ();
            fullScreen = true;
        }
        break;

    case 'b': //Toggle basis display
        display_basis = !display_basis;
        break;

    default:
        break;
    }
    idle ();
}

//Mouse events
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

//Mouse motion, update camera
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
        std::cout<<y<<std::endl;
        lastZoom = y;
    }
}



void reshape(int w, int h) {
    camera.resize (w, h);
}

// ------------------------------------
// Start of graphical application
// ------------------------------------
int main (int argc, char ** argv) {
    if (argc > 2) {
        exit (EXIT_FAILURE);
    }
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize (SCREENWIDTH, SCREENHEIGHT);

    window = glutCreateWindow ("TP HAI702I");
    camera.zoom (float (5));
    init ();
    glutIdleFunc (idle);
    glutDisplayFunc (display);
    glutKeyboardFunc (key);
    glutReshapeFunc (reshape);
    glutMotionFunc (motion);
    glutMouseFunc (mouse);
    key ('?', 0, 0);

    int n=3;
    //joints.resize(n);
    float s = 0.;
    float k =1.;
    /*
    joints.push_back(Vec3(0.,0.,0.));
    joints.push_back(Vec3(1.,0.,0.));
    joints.push_back(Vec3(2.,0.,0.));
    joints.push_back(Vec3(3.,0.,0.));
    joints.push_back(Vec3(4.,0.,0.));
    */


    for(int i =0; i<n;i++)
    {
        joints.push_back(Vec3(s+(float)i,(float)i,0.));

    }
  

    basis = Basis();

    glutMainLoop ();
    return EXIT_SUCCESS;
}

