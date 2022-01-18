
#include "Bone.h"
#include <sys/time.h>
#include <time.h>
Basis basis = Basis();

static double last_T;
int i =0;
int currentFrame=0; 
double dx = 0;
Bone * parent2;
std::vector<Vec3> pointsAnim;

static double ftime(void) {
    struct timeval t;
    gettimeofday(&t, NULL);

    return 1.0*t.tv_sec + 1e-6*t.tv_usec;
}
void animate(double speed) {

  glutPostRedisplay();
}

void idle () {
    const double now_T = ftime();
    const double delta_T = now_T - last_T;
    last_T = now_T;

    const double speed = delta_T * 60;

    animate(speed);

    glutPostRedisplay();
    
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
    case 'n':
        if(selected<points.size()-1)
            selected++;
        break;
    case 'p':
        if(selected>0)
            selected--;
        break;
    case 'i':

        
        boneFrame.resize(boneFrame.size()+1);
        
        parent2 = new Bone;
        copyBone(parent2, parent);
        boneFrame[currentFrame+1]=parent2;
         std::cout<<"Adding keyframe"<<std::endl;
        currentFrame++;

        break;
    case 'a':
        animation = !animation;
        break;
    case '8':
        updatePos(parent, selected , Vec3(0.,0.5,0.));
         break;
    case '2':
        updatePos(parent, selected , Vec3(0.,-0.5,0.));
         break;
    case '4':
        updatePos(parent, selected , Vec3(-0.5,0.,0.));
         break;
    case '6':
        updatePos(parent, selected , Vec3(0.5,0.,0.));
         break;
    case '+':
        updateRotation(parent, selected , (3.14/6.));
        break;
    case '-':
        updateRotation(parent, selected , -(3.14/6.));
        break;
    default:
        break;
    }
    idle ();
}
void mouseToworld(int x, int y , int& x2, int &y2)
{
    x2 = (x-800)/20;
    y2 = ((900-y)-450)/20;
}
//Mouse events
void mouse (int button, int state, int x, int y) {
    
    if (state == GLUT_UP) {
       
        mouseMovePressed = false;
        mouseRotatePressed = false;
        mouseZoomPressed = false;
    } else {
        if (button == GLUT_LEFT_BUTTON) {

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
void passiveMotion(int x, int y)
{
    int x2,y2;
    mouseToworld(x, y , x2, y2);
    if(!animation)
        fabrikAlgo(parent, Vec3(x2,y2,0.)  );

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
