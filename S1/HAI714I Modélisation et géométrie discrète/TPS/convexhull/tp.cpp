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
#include<time.h>
#include <unistd.h>

#include "io.h"

Basis basis2;
Basis basis3;

bool display_normals;
bool display_smooth_normals;
bool display_mesh;
bool display_basis;

int weight_type;
int algo = 0;

std::vector <Vec3> joints;
std::vector <Basis> basisJoints;
const double maxFPS = 60.0;
const double maxPeriod = 1.0 / maxFPS;
double lastTime = 0.0;

clock_t start, stop;

void drawBone(std::vector <Vec3> points)
{

    glColor3f(0.,1.0,0.0);
    glBegin(GL_LINE_STRIP);
    for(int i = 0 ; i < points.size(); i++)
    {
        glVertex3f(points[i][0],points[i][1],points[i][2]);
    }
    glEnd();

}

void drawJoints(std::vector <Vec3> points)
{
    glColor3f(0.5,0.5,0.5);
    glPointSize(5.);
    glBegin(GL_POINTS);
    for(int i = 0 ; i < points.size(); i++)
    {
        glVertex3f(points[i][0],points[i][1],points[i][2]);
    }
    glEnd();
}
void displayBasis(Basis basis)
{
   
    glBegin(GL_LINE_STRIP);
    glColor3f(1.,0.0,0.0);
    glVertex3f(basis[4][0],basis[4][1],basis[4][2]);
    glVertex3f(basis[4][0]+basis[0][0],basis[4][1]+basis[0][1],basis[4][2]+basis[0][2]);

    glColor3f(0.,0.0,1.0);
    glVertex3f(basis[4][0],basis[4][1],basis[4][2]);
    glVertex3f(basis[4][0]+basis[1][0],basis[4][1]+basis[1][1],basis[4][2]+basis[1][2]);

    glEnd();


}
void DrawCircle( float x, float y, float z,float r) {
        float i = 0.0f;
        
        glBegin(GL_TRIANGLE_FAN);
        
        glVertex3f(x, y,z); // Center
        for(i = 0.0f; i <= 360; i++)
                glVertex3f(r*cos(M_PI * i / 180.0) + x, r*sin(M_PI * i / 180.0) + y,z);
        
        glEnd();
}
void drawBones(Bone* parent)
{
    int count =-1;
    do
    {
         count++;
        if(count==selected) 
            glColor3f(1.,0.,0.);
        else
            glColor3f(0.5,0.5,0.5);

        Vec3 pos = parent->pos;
       
        glPointSize(5.);
        glBegin(GL_POINTS);
        glVertex3f(pos[0],pos[1],pos[2]); 
        glEnd();
        DrawCircle(pos[0],pos[1],pos[2],1.);
        
        if(parent->next!=NULL)
        {
            computeNewBase( parent,  parent->next);
            glColor3f(0.,1.0,0.0);
            glBegin(GL_TRIANGLE_FAN);

            Vec3 offsetY = parent->next->base[1];
            Vec3 offsetX = parent->next->base[0];

            Vec3 BottomLeft = pos - offsetY+offsetX;
            Vec3 TopLeft = pos + offsetY+offsetX;

            Vec3 TopRight = parent->next->pos+parent->next->base[1]-offsetX;
            Vec3 BottomRight = parent->next->pos- offsetY-offsetX;

            glVertex3f(BottomLeft[0],BottomLeft[1],BottomLeft[2]);
            glVertex3f(BottomRight[0],BottomRight[1],BottomRight[2]);
            glVertex3f(TopRight[0],TopRight[1],TopRight[2]);
            glVertex3f(TopLeft[0],TopLeft[1],TopLeft[2]);

            glVertex3f(BottomLeft[0],BottomLeft[1],BottomLeft[2]);

            
            glEnd();

        }
        displayBasis(parent->base);
        
        parent = parent->next;
       
        
        
    }while(parent->next != NULL);
}
void display () {

        glLoadIdentity ();
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        camera.apply ();
        Vec3 Red(1.0,0.0,0.0);
        Vec3 Green(0.0,1.0,0.0);
        Vec3 Blue(0.0,0.0,1.0);
        if(animation)
        {

            for(int indice = 0 ; indice < boneFrame.size()-1;indice++)
            {                
                for(int k = 0 ; k < 15 ; k++)
                {         
                    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   
                    boneLerp(boneFrame[indice],boneFrame[indice+1],(float)k/15.);
                    drawBones(boneFrame[indice]);
                    glutSwapBuffers ();

                }

            }
            animation = !animation;


        }
        else
            drawBones(parent);
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
    camera.zoom (float (50));
    init ();
    glutIdleFunc (idle);
    glutDisplayFunc (display);
    glutKeyboardFunc (key);
    glutReshapeFunc (reshape);
    glutMotionFunc (motion);
    glutMouseFunc (mouse);
    //glutTimerFunc(0, timer,0);
    glutPassiveMotionFunc(passiveMotion);
    key ('?', 0, 0);
    

    points.push_back(Vec3(0.,0.,0.));
    points.push_back(Vec3(7.,0.,0.));
    //points.push_back(Vec3(10.,0.,0.));
    points.push_back(Vec3(5.,-10.,0.));
    points.push_back(Vec3(-2.,-12.,0.));

    points.push_back(Vec3(-6.,-12,0.));
    points.push_back(Vec3(-10.,-12,0.));
    points.push_back(Vec3(-14.,-12,0.));
    points.push_back(Vec3(-18.,-12,0.));



    boneFrame.resize(1);

    makeBone(parent, points);

    boneFrame[0] = new Bone;
    addBone(boneFrame[0], points[1], basis);
    addBone(boneFrame[0]->next, points[2], basis);
    addBone(boneFrame[0]->next->next, points[3], basis);

   
  

    glutMainLoop ();
    return EXIT_SUCCESS;
}

