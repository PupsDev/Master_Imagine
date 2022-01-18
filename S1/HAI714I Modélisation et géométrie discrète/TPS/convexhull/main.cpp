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

#include "io.h"

Basis basis;

bool display_normals;
bool display_smooth_normals;
bool display_mesh;
bool display_basis;

int weight_type;
int algo = 0;

void drawBone()
{
    glColor3f(0.5,0.5,0.5);
    glPointSize(5.);
    glBegin(GL_TRIANGLE_FAN);
        
    Vec3 v1 = Vec3(0.,0.,0.);
    Vec3 v2 = Vec3(1.,0.,0.);
    Vec3 v3 = Vec3(0.5,0.5,0.);

    glVertex3f(v1[0],v1[1],v1[2]);
    glVertex3f(v2[0],v2[1],v2[2]);
    glVertex3f(v3[0],v3[1],v3[2]);

    glEnd();

} 
void display () {
    glLoadIdentity ();
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera.apply ();
    Vec3 Red(1.0,0.0,0.0);
    Vec3 Green(0.0,1.0,0.0);
    Vec3 Blue(0.0,0.0,1.0);

    
    drawBone();
    


    glFlush ();
    glutSwapBuffers ();
}

int main (int argc, char ** argv) {
    if (argc > 2) {
        exit (EXIT_FAILURE);
    }
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize (SCREENWIDTH, SCREENHEIGHT);

    window = glutCreateWindow ("TP HAI702I");
    camera.zoom (float (10));
    init ();
    glutIdleFunc (idle);
    glutDisplayFunc (display);
    glutKeyboardFunc (key);
    glutReshapeFunc (reshape);
    glutMotionFunc (motion);
    glutMouseFunc (mouse);
    key ('?', 0, 0);

    basis = Basis();

    glutMainLoop ();
    return EXIT_SUCCESS;
}

