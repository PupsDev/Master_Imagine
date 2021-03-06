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
#include <cmath>

#include <algorithm>
#include <GL/glut.h>
#include <float.h>
#include "src/Vec3.h"
#include "src/Camera.h"

enum DisplayMode{ WIRE=0, SOLID=1, LIGHTED_WIRE=2, LIGHTED=3 };

using namespace std;
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

    }

    //Compute vertices normals as the average of its incident faces normals
    void computeVerticesNormals(  ){



    }

    void computeNormals(){
        computeTrianglesNormals();
        computeVerticesNormals();
    }
};

class Maillage
{
    private:
        Mesh mesh;
        Mesh cube;
        vector<vector<vector<Vec3>>> grid;

        vector<vector<vector<Vec3>>> repPos;
        vector<vector<vector<Vec3>>> repNorm;
        vector<vector<vector<int>>> nbSommet;

    public:
        Maillage(Mesh const& mesh)
        {
            this->mesh = mesh;
        }
        void calculate_cube()
        {
            float x,y,z; //min
            float x1,y1,z1; //max
            x=y=z=100.0;
            x1=y1=z1=-100.0;

            for(int i = 0 ; i < (int)this->mesh.vertices.size();i++)
            {

                if(x>this->mesh.vertices[i][0])
                    x = this->mesh.vertices[i][0];

                if(y>this->mesh.vertices[i][1])
                    y = this->mesh.vertices[i][1];

                if(z>this->mesh.vertices[i][2])
                    z = this->mesh.vertices[i][2];


                if(x1<this->mesh.vertices[i][0])
                    x1 = this->mesh.vertices[i][0];
                
                if(y1<this->mesh.vertices[i][1])
                    y1 = this->mesh.vertices[i][1];

                if(z1<this->mesh.vertices[i][2])
                    z1 = this->mesh.vertices[i][2];
            }
            float dmax = fmax(abs(x-x1), fmax(abs(y-y1),abs(z-z1)));
            float diffx = dmax - abs(x-x1);
            float diffy = dmax - abs(y-y1);
            float diffz = dmax - abs(z-z1);

            x-=diffx/2;
            x1+=diffx/2;
            y-=diffy/2;
            y1+=diffy/2;
            z-=diffz/2;
            z1+=diffz/2;
            this->cube.vertices.resize(8);

            this->cube.vertices[0] = Vec3(x,y,z);
            this->cube.vertices[1] = Vec3(x1,y,z);
            this->cube.vertices[2] = Vec3(x1,y,z1);
            this->cube.vertices[3] = Vec3(x,y,z1);

            this->cube.vertices[4] = Vec3(x,y1,z);
            this->cube.vertices[5] = Vec3(x1,y1,z);
            this->cube.vertices[6] = Vec3(x1,y1,z1);
            this->cube.vertices[7] = Vec3(x,y1,z1);


        }
        void calculate_grid(unsigned int resolution)
        {

            vector<vector<vector<Vec3>>> grid;
            grid.resize(resolution);

            float factor = 1./resolution;
            Vec3 d = this->cube.vertices[1] -this->cube.vertices[0];
            float dist = d.length()*factor;
            Vec3 origin = this->cube.vertices[0];
            for(int i = 0 ; i <(int)resolution;i++)
            {   
                grid[i].resize(resolution);
                for(int j = 0 ; j < (int)resolution ; j++)
                {
                    grid[i][j].resize(resolution);
                    for(int k = 0 ; k < (int)resolution ; k++)
                    {
                        Vec3 p = Vec3(i*dist+origin[0],j*dist+origin[1],k*dist+origin[2]);
                        grid[i][j][k]=p;
                    }

                }
            }

            this->grid = grid;
        }
        Vec3 cubeIndice(Vec3 point, unsigned int resolution)
        {
            Vec3 origin = this->cube.vertices[0];
            float factor = 1./resolution;
            Vec3 d = this->cube.vertices[1] -this->cube.vertices[0];
            float dist = d.length()*factor;

            int i = (point[0]-origin[0])/dist;
            int j = (point[1]-origin[1])/dist;
            int k = (point[2]-origin[2])/dist;
            if(i>((int)resolution-1) || j >((int)resolution-1) || k>((int)resolution-1))
                    j=(resolution-1);
            return Vec3( (float)i,(float)j,(float)k);
        }
        void calculate_repr(unsigned int resolution)
        {
            vector<vector<vector<Vec3>>> repPos;
            vector<vector<vector<Vec3>>> repNorm;
            vector<vector<vector<int>>> nbSommet;

            repPos.resize(resolution);
            repNorm.resize(resolution);
            nbSommet.resize(resolution);
             for(int i = 0 ; i <(int)resolution;i++)
            {   
                repPos[i].resize(resolution);
                repNorm[i].resize(resolution);
                nbSommet[i].resize(resolution);
                for(int j = 0 ; j < (int)resolution ; j++)
                {
                    repPos[i][j].resize(resolution);
                    repNorm[i][j].resize(resolution);
                    nbSommet[i][j].resize(resolution);
                    for(int k = 0 ; k < (int)resolution ; k++)
                    {
                        repPos[i][j][k]=Vec3(0.,0.,0.);
                        repNorm[i][j][k]=Vec3(0.,0.,0.);
                        nbSommet[i][j][k]=0;
                    }

                }
            }
            
            for(int i = 0 ; i< (int)this->mesh.vertices.size();i++)
            {
               int ii=0,jj=0,kk=0;
                Vec3 point = this->cubeIndice(this->mesh.vertices[i], resolution);
                ii = (int)point[0];
                jj = (int)point[1];
                kk = (int)point[2];
                if(ii>((int)resolution-1) || jj >((int)resolution-1) || kk>((int)resolution-1))
                    jj=(resolution-1);
    

                repPos[ii][jj][kk]+=this->mesh.vertices[i];
                repNorm[ii][jj][kk]+=this->mesh.normals[i];

                nbSommet[ii][jj][kk]++;
               
            }
            int count = 0;
             for(int i = 0 ; i <(int)resolution;i++)
            {   

                for(int j = 0 ; j < (int)resolution ; j++)
                {

                    for(int k = 0 ; k < (int)resolution ; k++)
                    {
                        if(nbSommet[i][j][k]!=0)
                        {   
                           count++;

                            repPos[i][j][k]=(1./nbSommet[i][j][k])*repPos[i][j][k];
                            repNorm[i][j][k].normalize();
                        }
                        else
                            repPos[i][j][k]=Vec3(0.,0.,0.);

                    }

                }
            }
           
            this->repPos=repPos;
            this->repNorm=repNorm;
            this->nbSommet=nbSommet;
        }
         Mesh get_mesh()
        {
            return this->mesh;
        }
        Mesh get_cube()
        {
            return this->cube;
        }
         vector<vector<vector<Vec3>>> get_grid()
        {
            return this->grid;
        }
          vector<vector<vector<Vec3>>> get_rep()
        {
            return this->repPos;
        }
        bool diff(Vec3 i0,Vec3 i1, Vec3 i2)
        {
            bool r = true;
            for(int i = 0 ; i < 3 ; i++)
            {
                if(i0[i]!=i1[i]&&i1[i]!=i2[i]&&i2[i]!=i0[i])
                    r = false;
            }
            return r;
        }
        void printVec(Vec3 point)
        {
            cout<<"[0]:"<<point[0]<<" [1]:"<<point[1]<<" [2]:"<<point[2]<<endl;
        }
        void simplify (unsigned  int  resolution)
        {
           
            Mesh copy;
            copy = this->mesh;

            for( int i =0; i < (int)this->mesh.triangles.size();i++)
            {
                Triangle t = this->mesh.triangles[i];
                Vec3 i0 = this->cubeIndice(this->mesh.vertices[t[0]],resolution);
                Vec3 i1 = this->cubeIndice(this->mesh.vertices[t[1]],resolution);
                Vec3 i2 = this->cubeIndice(this->mesh.vertices[t[2]],resolution);



                if(diff(i0,i1,i2))
                {

                    copy.vertices[t[0]] = this->repPos[i0[0]][i0[1]][i0[2]];
                    copy.vertices[t[1]] = this->repPos[i1[0]][i1[1]][i1[2]];
                    copy.vertices[t[2]] = this->repPos[i2[0]][i2[1]][i2[2]];
                    

                    copy.normals[t[0]] = this->repNorm[i0[0]][i0[1]][i0[2]];
                    copy.normals[t[1]] = this->repNorm[i1[0]][i1[1]][i1[2]];
                    copy.normals[t[2]] = this->repNorm[i2[0]][i2[1]][i2[2]];
                }

            }
            this->mesh = copy;


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
//Fonction ?? completer
void collect_one_ring (std::vector<Vec3> const & i_vertices,
                       std::vector< Triangle > const & i_triangles,
                       std::vector<std::vector<unsigned int> > & o_one_ring) {//one-ring of each vertex, i.e. a list of vertices with which it shares an edge
    //Initialiser le vecetur de o_one_ring de la taille du vecteur vertices
    //Parcourir les triangles et ajouter les voisins dans le 1-voisinage
    //Attention verifier que l'indice n'est pas deja present
    //Tous les points oppos??s dans le triangle sont reli??s

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

//Fonction ?? completer
void compute_vertex_valences (const std::vector<Vec3> & i_vertices,
                              const std::vector< Triangle > & i_triangles,
                              std::vector<unsigned int> & o_valences ) {
    //Utiliser la fonction collect_one_ring pour r??cuperer le 1-voisinage*

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
Mesh smesh;
Mesh cubeMesh;
vector<vector<vector<Vec3>>> gridMesh;
vector<vector<vector<Vec3>>> repMesh;

std::vector< float > mesh_valence_field; //normalized valence of each vertex

Basis basis;

bool display_normals;
bool display_smooth_normals;
bool display_mesh;
bool display_smesh;
bool display_basis;
DisplayMode displayMode;
int weight_type;

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
    display_smesh = false;
    display_smooth_normals = false;
    displayMode = LIGHTED;
    display_basis = false;
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
    float r;       // ??? [0, 1]
    float g;       // ??? [0, 1]
    float b;       // ??? [0, 1]
} RGB;



RGB scalarToRGB( float scalar_value ) //Scalar_value ??? [0, 1]
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
    glBegin(GL_TRIANGLES);
    for(unsigned int tIt = 0 ; tIt < i_mesh.triangles.size(); ++tIt) {

        for(unsigned int i = 0 ; i < 3 ; i++) {
            const Vec3 & p = i_mesh.vertices[i_mesh.triangles[tIt][i]]; //Vertex position
            const Vec3 & n = i_mesh.normals[i_mesh.triangles[tIt][i]]; //Vertex normal


            glNormal3f( n[0] , n[1] , n[2] );
            glVertex3f( p[0] , p[1] , p[2] );
        }
    }
    glEnd();

}

void drawTriangleMesh( Mesh const & i_mesh , bool draw_field = false  ) {
    glBegin(GL_TRIANGLES);
    for(unsigned int tIt = 0 ; tIt < i_mesh.triangles.size(); ++tIt) {
        const Vec3 & n = i_mesh.triangle_normals[ tIt ]; //Triangle normal
        for(unsigned int i = 0 ; i < 3 ; i++) {
            const Vec3 & p = i_mesh.vertices[i_mesh.triangles[tIt][i]]; //Vertex position


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
void drawCube(Mesh const& i_mesh)
{
    glColor3f(1.,0.,0.);

    glLineWidth(4.);
    for(int i=0;i<3;i++)
        drawVector(i_mesh.vertices[i],i_mesh.vertices[i+1]);
    drawVector(i_mesh.vertices[3],i_mesh.vertices[0]);
    for(int i=4;i<7;i++)
        drawVector(i_mesh.vertices[i],i_mesh.vertices[i+1]);
    drawVector(i_mesh.vertices[7],i_mesh.vertices[4]);
    for(int i=0;i<4;i++)
        drawVector(i_mesh.vertices[i],i_mesh.vertices[i+4]);


}
void drawGrid(vector<vector<vector<Vec3>>> grid)
{
    glColor3f(1.,0.,0.);
    glPointSize(2.);
    glBegin(GL_POINTS);
    for(int i = 0 ; i < (int)grid.size();i++)
        for(int j = 0 ; j < (int)grid[i].size();j++)
            for(int k = 0 ; k < (int)grid[i][j].size();k++)
                glVertex3f( grid[i][j][k][0] , grid[i][j][k][1] , grid[i][j][k][2] );
    glEnd();


}
void drawRep(vector<vector<vector<Vec3>>> grid)
{
    glColor3f(0.,1.,0.);
    glPointSize(2.);
    glBegin(GL_POINTS);
    for(int i = 0 ; i < (int)grid.size();i++)
        for(int j = 0 ; j < (int)grid[i].size();j++)
            for(int k = 0 ; k < (int)grid[i][j].size();k++)
                glVertex3f( grid[i][j][k][0] , grid[i][j][k][1] , grid[i][j][k][2] );
    glEnd();


}
//Draw fonction
void draw () {



    if(displayMode == LIGHTED || displayMode == LIGHTED_WIRE){

        glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_LIGHTING);

    }  else if(displayMode == WIRE){

        glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
        glDisable (GL_LIGHTING);

    }  else if(displayMode == SOLID ){
        glDisable (GL_LIGHTING);
        glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

    }
    if(display_mesh)
    {
     glColor3f(0.8,1,0.8);
    drawMesh(mesh, true);       
    }
    if(display_smesh)
    {
     glColor3f(0.8,1,0.8);
    drawMesh(smesh, true);       
    }


    if(displayMode == SOLID || displayMode == LIGHTED_WIRE){
        glEnable (GL_POLYGON_OFFSET_LINE);
        glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth (1.0f);
        glPolygonOffset (-2.0, 1.0);

        glColor3f(0.,0.,0.);
        drawMesh(mesh, false);

        glDisable (GL_POLYGON_OFFSET_LINE);
        glEnable (GL_LIGHTING);
    }
    
    


    glDisable(GL_LIGHTING);
    if(display_normals){
        glColor3f(1.,0.,0.);
        drawNormals(mesh);
    }

    if( display_basis ){
        drawReferenceFrame(basis);
    }
    drawCube(cubeMesh);
    //drawRep(repMesh);
    drawGrid(gridMesh);
    glEnable(GL_LIGHTING);


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
    draw ();
    glFlush ();
    glutSwapBuffers ();
}

void idle () {
    glutPostRedisplay ();
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

    case 'n': //Press n key to display normals
        display_normals = !display_normals;
        break;

    case '1': //Toggle loaded mesh display
        display_mesh = !display_mesh;
        break;
    case 's': //Switches decimate and not decimate
        display_mesh = !display_mesh;
        display_smesh = !display_smesh;
        break;
    case 'v': //Switches between face normals and vertices normals
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

    init ();
    glutIdleFunc (idle);
    glutDisplayFunc (display);
    glutKeyboardFunc (key);
    glutReshapeFunc (reshape);
    glutMotionFunc (motion);
    glutMouseFunc (mouse);
    key ('?', 0, 0);

    //Mesh loaded with precomputed normals
    openOFF("data/elephant_n.off", mesh.vertices, mesh.normals, mesh.triangles, mesh.triangle_normals);
    Maillage* elephant = new Maillage(mesh);
    elephant->calculate_cube();
    unsigned int resolution = 16;
    elephant->calculate_grid(resolution);
    elephant->calculate_repr(resolution);
    cubeMesh = elephant->get_cube();
    gridMesh = elephant->get_grid();
    repMesh = elephant->get_rep();
    elephant->simplify(resolution);
    smesh = elephant->get_mesh();
    //Completer les fonction de calcul de normals
    mesh.computeNormals();
    smesh.computeNormals();

    basis = Basis();


     
    glutMainLoop ();
    return EXIT_SUCCESS;
}

