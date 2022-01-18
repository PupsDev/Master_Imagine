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
    // membres indices des sommets du triangle:
    unsigned int v[3];
};


struct Mesh {
    std::vector< Vec3 > vertices; //array of mesh vertices positions
    std::vector< Vec3 > normals; //array of vertices normals useful for the display
    std::vector< Triangle > triangles; //array of mesh triangles
    std::vector< Vec3 > triangle_normals; //triangle normals to display face normals

    //Compute face normals for the display
    void computeTrianglesNormals(){

        //A faire : implémenter le calcul des normales par face
        //Attention commencer la fonction par triangle_normals.clear();
        //Iterer sur les triangles

        //La normal du triangle i est le resultat du produit vectoriel de deux ses arêtes e_10 et e_20 normalisé (e_10^e_20)
        //L'arete e_10 est représentée par le vecteur partant du sommet 0 (triangles[i][0]) au sommet 1 (triangles[i][1])
        //L'arete e_20 est représentée par le vecteur partant du sommet 0 (triangles[i][0]) au sommet 2 (triangles[i][2])

        //Normaliser et ajouter dans triangle_normales
    }

    //Compute vertices normals as the average of its incident faces normals
    void computeVerticesNormals(  ){
        //Utiliser weight_type : 0 uniforme, 1 aire des triangles, 2 angle du triangle

        //A faire : implémenter le calcul des normales par sommet comme la moyenne des normales des triangles incidents
        //Attention commencer la fonction par normals.clear();
        //Initializer le vecteur normals taille vertices.size() avec Vec3(0., 0., 0.)
        //Iterer sur les triangles

        //Pour chaque triangle i
        //Ajouter la normal au triangle à celle de chacun des sommets en utilisant des poids
        //0 uniforme, 1 aire du triangle, 2 angle du triangle

        //Iterer sur les normales et les normaliser


    }

    void computeNormals(){
        computeTrianglesNormals();
        computeVerticesNormals();
    }
};

//Transformation made of a rotation and translation
struct Transformation {
    Mat3 rotation;
    Vec3 translation;
};

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
//Fonction à completer
void collect_one_ring (std::vector<Vec3> const & i_vertices,
                       std::vector< Triangle > const & i_triangles,
                       std::vector<std::vector<unsigned int> > & o_one_ring) {//one-ring of each vertex, i.e. a list of vertices with which it shares an edge
    //Initialiser le vecetur de o_one_ring de la taille du vecteur vertices
    //Parcourir les triangles et ajouter les voisins dans le 1-voisinage
    //Attention verifier que l'indice n'est pas deja present
    //Tous les points opposés dans le triangle sont reliés

    o_one_ring.clear();
    o_one_ring.resize(i_vertices.size());

    for(size_t i = 0 ; i < i_triangles.size() ; i++)
    {      
           for(size_t j = 0 ; j< 3 ; j++)
           {
                 /*
                v0  v1    v2
                j   j+1   j+2 -> % 3

                0   1     2
                1   2     1
                2   0     1

                */
    
                size_t v1 = (j+1)%3;
                size_t v2 = (j+2)%3;

                insertInRing(o_one_ring[i_triangles[i][j]] , i_triangles[i][v1]);
                insertInRing(o_one_ring[i_triangles[i][j]] , i_triangles[i][v2]);    

           }
    }

}

//Fonction à completer
void compute_vertex_valences (const std::vector<Vec3> & i_vertices,
                              const std::vector< Triangle > & i_triangles,
                              std::vector<unsigned int> & o_valences ) {
    //Utiliser la fonction collect_one_ring pour récuperer le 1-voisinage*

    std::vector<std::vector<unsigned int> > o_one_ring;
    collect_one_ring(i_vertices, i_triangles, o_one_ring);

    o_valences.clear();


    for(size_t i = 0 ; i < i_vertices.size() ; i ++)
    {

        o_valences.push_back( o_one_ring[i].size());
    }

}


//Input mesh loaded at the launch of the application
Mesh mesh;
std::vector< float > mesh_valence_field; //normalized valence of each vertex
std::vector< Vec3 > courbe;
std::vector< Vec3 > convex;



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
// File I/O
// ------------------------------------
bool saveOFF( const std::string & filename ,
              std::vector< Vec3 > const & i_vertices ,
              std::vector< Vec3 > const & i_normals ,
              std::vector< Triangle > const & i_triangles,
              std::vector< Vec3 > const & i_triangle_normals ,
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
        myfile << 3 << " " << i_triangles[f][0] << " " << i_triangles[f][1] << " " << i_triangles[f][2]<< " ";
        if (save_normals) myfile << i_triangle_normals[f][0] << " " << i_triangle_normals[f][1] << " " << i_triangle_normals[f][2];
        myfile << std::endl;
    }
    myfile.close();
    return true;
}

void openOFF( std::string const & filename,
              std::vector<Vec3> & o_vertices,
              std::vector<Vec3> & o_normals,
              std::vector< Triangle > & o_triangles,
              std::vector< Vec3 > & o_triangle_normals,
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
    o_triangle_normals.clear();
    for( int f = 0 ; f < n_faces ; ++f )
    {
        int n_vertices_on_face;
        myfile >> n_vertices_on_face;

        if( n_vertices_on_face == 3 )
        {
            unsigned int _v1 , _v2 , _v3;
            myfile >> _v1 >> _v2 >> _v3;

            o_triangles.push_back(Triangle( _v1, _v2, _v3 ));

            if( load_normals ) {
                float x , y , z ;
                myfile >> x >> y >> z;
                o_triangle_normals.push_back( Vec3( x , y , z ) );
            }
        }
        else if( n_vertices_on_face == 4 )
        {
            unsigned int _v1 , _v2 , _v3 , _v4;
            myfile >> _v1 >> _v2 >> _v3 >> _v4;

            o_triangles.push_back(Triangle(_v1, _v2, _v3 ));
            o_triangles.push_back(Triangle(_v1, _v3, _v4));
            if( load_normals ) {
                float x , y , z ;
                myfile >> x >> y >> z;
                o_triangle_normals.push_back( Vec3( x , y , z ) );
            }

        }
        else
        {
            std::cout << "We handle ONLY *.off files with 3 or 4 vertices per face" << std::endl;
            myfile.close();
            exit(1);
        }
    }

}

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


// ------------------------------------
// Rendering.
// ------------------------------------

void drawVector( Vec3 const & i_from, Vec3 const & i_to ) {

    glBegin(GL_LINES);
    glVertex3f( i_from[0] , i_from[1] , i_from[2] );
    glVertex3f( i_to[0] , i_to[1] , i_to[2] );
    glEnd();
}

void drawAxis( Vec3 const & i_origin, Vec3 const & i_direction ) {

    glLineWidth(4); // for example...
    drawVector(i_origin, i_origin + i_direction);
}

void drawReferenceFrame( Vec3 const & origin, Vec3 const & i, Vec3 const & j, Vec3 const & k ) {

    glDisable(GL_LIGHTING);
    glColor3f( 0.8, 0.2, 0.2 );
    drawAxis( origin, i );
    glColor3f( 0.2, 0.8, 0.2 );
    drawAxis( origin, j );
    glColor3f( 0.2, 0.2, 0.8 );
    drawAxis( origin, k );
    glEnable(GL_LIGHTING);

}

void drawReferenceFrame( Basis & i_basis ) {
    drawReferenceFrame( i_basis.origin, i_basis.i, i_basis.j, i_basis.k );
}

typedef struct {
    float r;       // ∈ [0, 1]
    float g;       // ∈ [0, 1]
    float b;       // ∈ [0, 1]
} RGB;



RGB scalarToRGB( float scalar_value ) //Scalar_value ∈ [0, 1]
{
    RGB rgb;
    float H = scalar_value*360., S = 1., V = 0.85,
            P, Q, T,
            fract;

    (H == 360.)?(H = 0.):(H /= 60.);
    fract = H - floor(H);

    P = V*(1. - S);
    Q = V*(1. - S*fract);
    T = V*(1. - S*(1. - fract));

    if      (0. <= H && H < 1.)
        rgb = (RGB){.r = V, .g = T, .b = P};
    else if (1. <= H && H < 2.)
        rgb = (RGB){.r = Q, .g = V, .b = P};
    else if (2. <= H && H < 3.)
        rgb = (RGB){.r = P, .g = V, .b = T};
    else if (3. <= H && H < 4.)
        rgb = (RGB){.r = P, .g = Q, .b = V};
    else if (4. <= H && H < 5.)
        rgb = (RGB){.r = T, .g = P, .b = V};
    else if (5. <= H && H < 6.)
        rgb = (RGB){.r = V, .g = P, .b = Q};
    else
        rgb = (RGB){.r = 0., .g = 0., .b = 0.};

    return rgb;
}

void drawSmoothTriangleMesh( Mesh const & i_mesh , bool draw_field = false ) {
    glBegin(GL_LINES);
    for(unsigned int tIt = 0 ; tIt < i_mesh.triangles.size(); ++tIt) {

        for(unsigned int i = 0 ; i < 3 ; i++) {
            const Vec3 & p = i_mesh.vertices[i_mesh.triangles[tIt][i]]; //Vertex position
            const Vec3 & n = i_mesh.normals[i_mesh.triangles[tIt][i]]; //Vertex normal

            if( draw_field && mesh_valence_field.size() > 0 ){
                RGB color = scalarToRGB( mesh_valence_field[i_mesh.triangles[tIt][i]] );
                glColor3f( color.r, color.g, color.b );
            }
            glNormal3f( n[0] , n[1] , n[2] );
            glVertex3f( p[0] , p[1] , p[2] );
        }
    }
    glEnd();

}

void drawTriangleMesh( Mesh const & i_mesh , bool draw_field = false  ) {
    glBegin(GL_LINES);
    for(unsigned int tIt = 0 ; tIt < i_mesh.triangles.size(); ++tIt) {
        const Vec3 & n = i_mesh.triangle_normals[ tIt ]; //Triangle normal
        for(unsigned int i = 0 ; i < 3 ; i++) {
            const Vec3 & p = i_mesh.vertices[i_mesh.triangles[tIt][i]]; //Vertex position

            if( draw_field ){ // TODOOM
                RGB color = scalarToRGB( mesh_valence_field[i_mesh.triangles[tIt][i]] );
                glColor3f( color.r, color.g, color.b );
            }
            glNormal3f( n[0] , n[1] , n[2] );
            glVertex3f( p[0] , p[1] , p[2] );
        }
    }
    glEnd();

}

void drawMesh( Mesh const & i_mesh , bool draw_field = false ){
    if(display_smooth_normals)
        drawSmoothTriangleMesh(i_mesh, draw_field) ; //Smooth display with vertices normals
    else
        drawTriangleMesh(i_mesh, draw_field) ; //Display with face normals
}

void drawVectorField( std::vector<Vec3> const & i_positions, std::vector<Vec3> const & i_directions ) {
    glLineWidth(1.);
    for(unsigned int pIt = 0 ; pIt < i_directions.size() ; ++pIt) {
        Vec3 to = i_positions[pIt] + 0.02*i_directions[pIt];
        drawVector(i_positions[pIt], to);
    }
}

void drawNormals(Mesh const& i_mesh){

    if(display_smooth_normals){
        drawVectorField( i_mesh.vertices, i_mesh.normals );
    } else {
        std::vector<Vec3> triangle_baricenters;
        for ( const Triangle& triangle : i_mesh.triangles ){
            Vec3 triangle_baricenter (0.,0.,0.);
            for( unsigned int i = 0 ; i < 3 ; i++ )
                triangle_baricenter += i_mesh.vertices[triangle[i]];
            triangle_baricenter /= 3.;
            triangle_baricenters.push_back(triangle_baricenter);
        }

        drawVectorField( triangle_baricenters, i_mesh.triangle_normals );
    }
}

//Draw fonction



float lin(float x)
{
    return x*x;
}
void draw (std::vector<Vec3> & o_vertices, Vec3 color, int algo) {

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

void changeDisplayMode(){
    if(displayMode == LIGHTED)
        displayMode = LIGHTED_WIRE;
    else if(displayMode == LIGHTED_WIRE)
        displayMode = SOLID;
    else if(displayMode == SOLID)
        displayMode = WIRE;
    else
        displayMode = LIGHTED;
}

void display () {
    glLoadIdentity ();
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera.apply ();
    Vec3 Red(1.0,0.0,0.0);
    Vec3 Green(0.0,1.0,0.0);
    Vec3 Blue(0.0,0.0,1.0);

    
    drawCloud (courbe, Red);
    draw (convex, Green,algo);
    


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

std::vector <Vec3> initFixedPoints(){
    std::vector <Vec3> points;
    std::vector<int>  x = {120, 30, 220, 330, 60}, y = {340, 450, 100, 450, 200};
    for (size_t i = 0; i < 5; i++){
            points.push_back(Vec3((float)x[i], (float)y[i], 1.));
    }
    return points;
}

size_t findMinIdx(std::vector <Vec3> points){
    size_t imin = 0;
    for(size_t i = 1; i < points.size(); i++){
        if (points[i][0] < points[imin][0])
            imin = i;
    }
    return imin;
}

float determinant(Vec3 v1, Vec3 v2){
    return v1[0] * v2[1] - v1[1] * v2[0];
}

int detSign(Vec3 v1, Vec3 v2){
    float d = determinant(v1,v2);
    if (d > 0) return 1;
    if (d == 0) return 0;
    return -1;
}
 

Vec3 vect(Vec3 a, Vec3 b){
    return Vec3( b[0]-a[0], b[1]-a[1],1.);
}
bool diffVect(Vec3 a, Vec3 b)
{
    bool result = true;
    for(size_t i = 0 ; i < 3 ; i++)
    {
        if(a[i]!=b[i])
            result=false;
    }
    return result;
}
size_t findFirst(std::vector <Vec3> points, size_t i, size_t j){
    size_t x= 0;
    for(size_t k=0; k<points.size();k++)
    {
        if( diffVect(points[k],points[i]) && diffVect(points[k],points[j]))
            return k;
    }
    return x;

}
int tour(Vec3 va,Vec3 vb,Vec3 vc)
{
    Vec3 ab = vect(va,vb);
    Vec3 ac = vect(va,vc);
    ab.normalize();
    ac.normalize();
    int d = detSign(ab,ac);
    return d; 

}
std::vector <Vec3> algoDemiPlan(std::vector <Vec3> points)
{
    std::vector <Vec3> result;
    int current;
    int previous;
    size_t n = points.size();
    size_t i,j,k;
    size_t first;
    bool mc;
    std::cout<<"Algo Demi Plan"<<std::endl;
    for(i=0 ; i < n ; i ++)
    {
        for(j=i+1; j < n ; j++)
        {
            mc=true;
            k=0;  
            do
            {
                if(k==0)
                {
                    first = findFirst(points,i,j);
                    previous = tour(points[i],points[j],points[first]);

                }
                if(k!=i && k!=j)
                {
                    current = tour(points[i],points[j],points[k]);
                    if(current==0)
                    {
                        std::cout<<"Alignement"<<std::endl;
                    }
                    else if(current != previous)
                    {
                        mc = false;
                    }
                } 
            k++;
            }while(k<n && mc);
            
            if(k==n && previous==current)
            {
                if(current>0)
                {
                    result.push_back(points[i]);
                    result.push_back(points[j]);
                }
                if(current<0)
                {
                    result.push_back(points[j]);
                    result.push_back(points[i]);
                                       
                }
                if(current==0)std::cout<<"Alignement"<<std::endl;;
            }
        }
    }


    return result;
}
int minY(std::vector <Vec3> points)
{
    int result = 0;
    float my = points[0][1]; // y -> 1

    for(size_t i =0; i <points.size();i++)
    {

        if(points[i][1]<my)
        {
            my = points[i][1];
            result = i;

        }

    }
    return result;
}

int findNext(std::vector <Vec3> points, size_t previous)
{
    int mostRight = (previous+1)%points.size();

    for(size_t i = 0 ; i < points.size(); i++)
    {
            if(tour(points[previous],points[i],points[mostRight])==-1) // TD
            {
                mostRight = i;
                
            }
        
    }
    return mostRight;

}

std::vector <Vec3> algoJarvis(std::vector <Vec3> points)
{
    std::vector <Vec3> result;
    int min = minY(points);
    result.push_back(points[min]);
    int current;
    int previous = min;
    std::cout<<"Algo Jarvis"<<std::endl;
    do
    {

        current = findNext(points,previous);
        result.push_back(points[current]);
        previous = current;

    }while(current != min); 

    return result;
}

int compare(Vec3 p1 , Vec3 p2,Vec3 p0)
{
    Vec3 pp0 =Vec3(1.,0.,0.);
    Vec3 pp1 =vect(p0,p1);
    Vec3 pp2 =vect(p0,p2);
    pp1.normalize();
    pp2.normalize();

    float angle = acos((float)Vec3::dot(pp0,pp1)/(float)sqrt(pp0.length() * pp1.length()));
    float angle2 = acos((float)Vec3::dot(pp0,pp2)/(float)sqrt(pp0.length() * pp2.length()));
    if(angle == angle2)
    {
        return p1[0]<p2[0];
    }
    else return angle < angle2;

}

int Partition(std::vector<Vec3> &v, int start, int end, Vec3 pmin){
	
	int pivot = end;
	int j = start;
	for(int i=start;i<end;++i){
            if(compare(v[i],v[pivot],pmin)){
                std::swap(v[i],v[j]);
                ++j;
            }
	}
	std::swap(v[j],v[pivot]);
	return j;
	
}

void Quicksort(std::vector<Vec3> &v, int start, int end ,Vec3 pmin){


	if(start<end){
		int p = Partition(v,start,end, pmin);
		Quicksort(v,start,p-1,pmin);
		Quicksort(v,p+1,end,pmin);
	}
	
}
std::vector <Vec3> algoGraham(std::vector <Vec3> points)
{
    std::vector <Vec3> result;
    int min = minY(points);
    Vec3 pmin = points[min];

    points.erase(points.begin()+min);
    std::vector <Vec3> candidates = points;
    Quicksort(candidates,  0, candidates.size() -1, pmin);
    std::cout<<"Algo Graham"<<std::endl;


    result.push_back(pmin);
    result.push_back(candidates[0]);
    
    Vec3 previous =pmin;
    Vec3 current=candidates[0];

    for(int i=1; i< candidates.size();i++)
    {       
            while(result.size()>1 && tour(result[result.size()-2],result[result.size()-1],candidates[i])<=0)
            {
                result.pop_back();
                
            }
            result.push_back(candidates[i]);
    }
    result.push_back(pmin);


    return result;
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


    case 'w': //Change le mode d'affichage
        changeDisplayMode();
        break;


    case 'b': //Toggle basis display
        display_basis = !display_basis;
        break;

    case 'd': //Press n key to display normals

        courbe.resize(45);
        courbe = initRandomPoints(45,SCREENWIDTH,SCREENHEIGHT );
        convex=algoDemiPlan(courbe);
        algo=0;
        
        break;

    case 'j': //Press n key to display normals

        courbe.resize(45);
        courbe = initRandomPoints(45,SCREENWIDTH,SCREENHEIGHT );
        convex=algoJarvis(courbe);
        algo=1;

        break;
    
    case 'g': //Press n key to display normals

        courbe.resize(10);
        courbe = initRandomPoints(10,SCREENWIDTH,SCREENHEIGHT );
        convex=algoGraham(courbe);
        algo=2;

        break;

    case 'n': //Press n key to display normals

        courbe.resize(45);
        courbe = initRandomPoints(45,SCREENWIDTH,SCREENHEIGHT );
        convex=algoJarvis(courbe);
        break;
    case '1': //Toggle loaded mesh display
        display_mesh = !display_mesh;
        break;

    case 's': //Switches between face normals and vertices normals
        display_smooth_normals = !display_smooth_normals;
        break;

    case '+': //Changes weight type: 0 uniforme, 1 aire des triangles, 2 angle du triangle
        weight_type ++;
        if(weight_type == 3) weight_type = 0;
        mesh.computeVerticesNormals(); //recalcul des normales avec le type de poids choisi
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


Vec3 doublePoint(Vec3 P1, Vec3 P2, float u)
{
    Vec3 P3;
    P3 = (1 - u) * P1 + u * P2;
    return P3;
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
    camera.zoom (float (1500));
    init ();
    glutIdleFunc (idle);
    glutDisplayFunc (display);
    glutKeyboardFunc (key);
    glutReshapeFunc (reshape);
    glutMotionFunc (motion);
    glutMouseFunc (mouse);
    key ('?', 0, 0);

    //Mesh loaded with precomputed normals
    //openOFF("data/elephant_n.off", mesh.vertices, mesh.normals, mesh.triangles, mesh.triangle_normals);

    //Completer les fonction de calcul de normals
    mesh.computeNormals();
    size_t numberPoints =4;
    courbe.resize(numberPoints);
    /*
    for(size_t i=0; i< numberPoints ; i++)
    {   
        Vec3 pts = Vec3((float)(rand() % 7),(float)(rand() % 5),1.);
        courbe.push_back(pts);

    }*/
    //courbe = initFixedPoints();
    //courbe = initFixedPoints();

    courbe = initRandomPoints(numberPoints,SCREENWIDTH,SCREENHEIGHT);
    

    algo=2;
    if(algo==1)
        convex=algoJarvis(courbe);
    if(algo==0)
        convex = algoDemiPlan(courbe);
    if(algo==2)
        convex= algoGraham( courbe);




    basis = Basis();

    glutMainLoop ();
    return EXIT_SUCCESS;
}

