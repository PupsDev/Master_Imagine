#include "util.h"
int selected = 0 ;
std::vector<Vec3>  points;
bool animation = false;

std::vector<std::vector<Vec3>>  animationVec;
class Bone
{
    public:
    Vec3 pos;
    Basis base;
    float length;
    bool selected;
    Bone * next;
    Bone * prev;
    Bone()
    {
        this->pos = Vec3(0.,0.,0.);
        this->base = Basis();
        this->prev =NULL;
        this->next =NULL;
        this->selected = false;
    }
};

float getTotalLength(Bone * parent)
{
    float length = 0.;
    while(parent !=NULL)
    {

        if(parent->next!=NULL)
        {
            Vec3 vecBone = parent->next->pos - parent->pos;
            length += vecBone.length();
        }
        parent= parent->next;
    }
    return length;
}
void insert(Bone * parent, Bone * child)
{

    parent->next = child;
    child->prev = parent;
    
}
Bone * getLastChild(Bone * parent)
{
    while(parent->next !=NULL)
    {
        parent= parent->next;
    }
    return parent;
    
}
Bone * addBone(Bone * parent, Vec3 pos, Basis base)
{
    Bone * newBone = new Bone;
    if(parent)
        parent->next = newBone;
    newBone->prev = parent;
    Vec3 B = parent->pos - pos;
    newBone->prev->length = B.length();
    newBone->next = NULL;
    newBone->pos = pos;
    newBone->base =Basis();
    newBone->base.setOrigin(pos);
    newBone->selected = false;
    return newBone;
}
Vec3 rotate90i(Vec3 v)
{
    return Vec3( 0 , 1*v[0],0. )+ Vec3( -1 *v[1], 0,0. );
}
Vec3 rotateAngle(Vec3 v, float angle)
{
    return  Vec3( cos(angle)*v[0] ,sin(angle)*v[0],0. )+ 
            Vec3( -sin(angle) *v[1],cos(angle)*v[1],0. );
}

float easeIn(float t)
{
    return t*t*t;
}
Vec3  vecLerp(Vec3 oldPos , Vec3 newPos, float t)
{
    return (1.-easeIn(t))*oldPos +easeIn(t) * newPos;
}
void boneLerp(Bone * oldBone, Bone* newBone, float t)
{

    while(oldBone !=NULL && newBone!=NULL)
    {
        oldBone->pos = vecLerp(oldBone->pos,newBone->pos, t);
        oldBone= oldBone->next;
        newBone= newBone->next;
    }

}
void computeNewBase(Bone * parent, Bone * child)
{

    Vec3 v2 = child->pos;
    Vec3 v1 = parent->pos;
    Vec3 a = v2-v1;
    a.normalize();
    Vec3 b = Vec3(a[0],-a[1],0.);

    b = rotate90i(a);
    Basis basis2;
    basis2 = Basis();
    basis2.setI(a);
    basis2.setJ(b);
    basis2.setOrigin(v2);

    child->base = basis2;

}

void computeRotation(Bone * parent, float angle)
{   
    Bone *child = parent->next;
    while(child !=NULL)
    {

        child->pos = rotateAngle(child->pos-parent->pos,angle);
        child->pos+=parent->pos;
        computeNewBase(parent, child);
        child= child->next;
    }

}


void printBone(Bone *parent)
{
    while(parent !=NULL)
    {
        std::cout<<parent->pos<<std::endl;
        parent = parent->next;
    }
}

void copyBone(Bone * newB, Bone* oldB)
{
    Basis basis = Basis();
    while(oldB!=NULL)
    {
        addBone(newB,oldB->pos, basis);
        newB = newB->next;
        oldB = oldB->next;
    }

}
void makeBone(Bone * newB, std::vector<Vec3> points)
{
    Basis basis = Basis();
    int i =0;
    while(i<points.size())
    {
        addBone(newB,points[i], basis);
        newB = newB->next;
        i++;
    }

}
void updatePos(Bone* parent, int i , Vec3 pos)
    {
        int indice =0;
        while(parent !=NULL)
        {
            if(indice ==i)
            {
                parent->pos=parent->pos+pos;
                
                parent->base.setOrigin(parent->pos);

            }
            indice++;
            if(parent->next!=NULL)
                computeNewBase( parent,  parent->next);
            parent= parent->next;
        }
    
    }
    void updateRotation(Bone* parent, int i , float angle)
    {
        int indice =0;
        while(parent !=NULL)
        {
            if(indice ==i)
            {
                computeRotation( parent, angle);
            }
            indice++;
             parent= parent->next;
        }
    
    }

void backward(Bone * parent,Vec3 mousePos)
{
    parent->pos = mousePos;
    while(parent->prev !=NULL)
    {
        Vec3 dir = parent->prev->pos - parent->pos;
        dir.normalize();

        // Calculate P-1 pos
        float length = parent->prev->length;
        dir*= length;
        parent->prev->pos = parent->pos + dir;

        parent = parent->prev;
    }
}
void forward(Bone * parent,Vec3 mousePos)
{
    parent->pos = Vec3(0.,0.,0.);
    while(parent->next !=NULL)
    {
        Vec3 dir = parent->next->pos - parent->pos;
        dir.normalize();

        // Calculate P+1 pos
        float length = parent->length;
        dir*= length;
        parent->next->pos = parent->pos + dir;

        parent = parent->next;
    }
    
}
void fabrikAlgo(Bone * parent, Vec3 mousePos)
{

    if(mousePos.length()>getTotalLength(parent))
    {
        while(parent->next !=NULL)
        {
            Vec3 dir = mousePos-parent->pos;
            dir.normalize();
            dir*=parent->length;

            parent->next->pos = dir + parent->pos;
            parent= parent->next;
        }

    }
    else
    {
        Vec3 dVec = mousePos - getLastChild(parent)->pos;
        float d = dVec.length();
        int iteration =0;
        while(d>0.01&&iteration < 10)
        {
            backward(getLastChild(parent), mousePos);
            forward(parent,mousePos);
            dVec = mousePos - getLastChild(parent)->pos;
            d = dVec.length();
            iteration++;

        }

    }
}

std::vector <Bone*> boneFrame;
Bone* parent = new Bone;
Bone* nextParent = new Bone;