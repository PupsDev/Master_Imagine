#include <iostream>
#include <vector>
#include <queue>
#include <math.h>
#include <cstring>
using namespace std;

int charToint(char * bitboard, int n)
{
    int x=1<<(n);
    int result=1;
   
    for(int i = 0 ; i < n ; i++)
    {
        int b = bitboard[i]-'0';
        result+=x&&b ? x : 0;
        x=x>>1;
    }
    return result;

}
int charToLongInt(char * bitboard, int n)
{
    int x=1<<(n);
    int result=0;
   
    for(int i = 0 ; i <= n ; i++)
    {
        int b = bitboard[i]-'0';
        result+=x&&b ? x : 0;
        x=x>>1;
    }
    return result;

}
string intToChar(int bit,int size)
{
    
    string result = string("");
    if(bit==0)return string("0");
    int n =log2(bit);
    int x=1<<n;

    for(int i = n; i<(size*size)-1;i++)result+='0';

    while(bit>0&&x!=0)
    {
        result += (x<=bit)+'0';
        if(x<=bit)
            bit=bit-x;
        x=x>>1;
    }
    if( result.size()<(size*size))
    {
        for(int i =result.size(); i<(size*size);i++)result+='0';
    }
    
    return result;

}
void printBitBoard(char * bitboard)
{
    cout<<"bitboard:\n"<<bitboard<<"\n";
}
void printBitBoardAsBoard(char * bitboard, int size)
{
    cout<<"bitboard:\n";
    for(int i = 0 ; i < size;i++)
    {
        for(int j = 0 ; j < size;j++)
        {
             cout<<bitboard[i*size+j]<<" ";
        }
        cout<<"\n";
    }
}
long int bitBoardToInt(char * bitboard)
{
    return charToint(bitboard,strlen(bitboard+1)); 
}

class State
{
    public:
        int bitboard[2];
        int size;
        int size2;
        // 0 for O and 1 for X
        int turnState;

    State(int size)
    {
        /*
            Allocation du tableau et transformation en int
            int -> 5
            sinon choisir plus ou tableau de int
            -> max entier 1<<(size2)-1
        */
                
        this->size=size;
        this->size2=size*size;
        this->turnState = 0;
        this->bitboard[0]=0;
        this->bitboard[1]=0;
   
    }
        State(State * parent)
    {
        /*
            Allocation du tableau et transformation en int
            int -> 5
            sinon choisir plus ou tableau de int
            -> max entier 1<<(size2)-1
        */
                
        this->size=parent->size;
        this->size2=size*size;
        this->turnState = parent->turnState;
        this->bitboard[0]= parent->bitboard[0];
        this->bitboard[1]= parent->bitboard[1];
   
    }
    void play(int i)
    {   

        cout<<"\nPlay : "<<i<<"\n";
       /* int O = this->bitboard[0];
        int X = this->bitboard[1];
        string boardStringO = intToChar( this->bitboard[0],size);  
        string boardStringX = intToChar( this->bitboard[1],size); 
        cout<<O<<" "<<X<<"\n";
        cout<<boardStringO<<"\n";
        cout<<boardStringX<<"\n";
         string boardStringOX = intToChar(this->bitboard[0]+this->bitboard[1],size); 
        cout<<O+X<<"\n";
         cout<<boardStringOX<<"\n";
         */

        // Ennemmy at pos i
        int x = (1<<i);
        this->bitboard[turnState]+=x;
        

        turnState = !turnState;
    }
    void printBoard()
    {
        
        cout<<"Board :\n";
        string boardStringO = intToChar( this->bitboard[0],size);  
        string boardStringX = intToChar( this->bitboard[1],size); 
        
        char c = !turnState==0 ? 'O' : 'X';
        printf("TOUR: %c \n",c);
        for(int i = 0 ; i < this->size;i++)
        {
            for(int j = 0 ; j < this->size;j++)
            {

                if( boardStringO[i*this->size+j]=='1')
                {
                    cout<<"[O]"<<" ";
                }
                else if( boardStringX[i*this->size+j]=='1')
                {
                    cout<<"[X]"<<" ";
                }
                else
                {
                    cout<<"[ ]"<<" ";
                }

            }
            cout<<"\n";
        }
    }
};
template<typename T>
class Node
{
    public:
        Node(T data, size_t indice)
        {
            this->data=data;
            this->indice=indice;
            this->discovered = false;
        }
         void print(){ /*std::cout<<"indice: "<<indice<<"\n";*/ this->data->printBoard();}
        void resetDiscovered()
        {
            this->discovered=false;
            for(auto child:children)
            {
                if(child->discovered)
                    child->resetDiscovered();
            }

        }
        void createNode(int people)
        {
            State * state = new State(data);
            Node< State* >* choice = new Node< State* >( state ,this->indice*10+people);
            choice->data->play(people);
            choice->data->printBoard();

           

            children.push_back(choice);
            choice->play();
            


        }
        void play()
        {
            int O = data->bitboard[0];
            int X = data->bitboard[1];
            cout<<"OHOH3"<<O+X<<endl;
            if( (O+X) < 20 )
            {
                for(int i = 0 ; i < data->size2;i++)
                {
                    int ennemyPos = data->bitboard[!data->turnState]>>i;
                    int Pos = data->bitboard[data->turnState]>>i;

                    //cout<<!(Pos&1)<<"\n";
                    if( !(ennemyPos&1) && !(Pos&1) )
                    {
                        createNode(i);
                    }
                }
            }
            
        }
        std::vector<Node*> children;
        size_t indice;
        bool discovered;
        T data;
};
template<typename T>
void DFS_recursive(Node<T>* root)
{

    root->print();
    root->discovered=true;
    for(auto child : root->children)
    {
        if(!child->discovered)
        {
            DFS_recursive(child);
        }
    }


}

template<typename T>
void BFS_iterative(Node<T>* root)
{
    std::queue<Node<T>*> Q;

     Q.push(root);
     while(!Q.empty())
     {
         Node<T>* v = Q.front();
         Q.pop();
         v->print();
         v->discovered=true;
         for(auto child : v->children)
         {
            if(!child->discovered)
            {
                child->discovered=true;
                Q.push(child);
            }
         }
     }
}


int main() {

    int size = 3;
    //State init = State(size);
    
    Node< State* >* init = new Node< State* >( new State(size),0);
    init->play();
            
    /*
    State * state = new State(init->data);
    Node< State* >* choice = new Node< State* >( state ,init->indice*10+1);
    choice->data->play(1);
    init->children.push_back(choice);

    State * state2 = new State(choice->data);
    Node< State* >* choice2 = new Node< State* >( state2 ,choice->indice*100+2);
    choice2->data->play(2);
    choice->children.push_back(choice2);
    */
    BFS_iterative(init);
    /*for(int i = 0 ; i < (size*size);i++)
    {
        init.play(i);
        init.printBoard();
    }*/


}