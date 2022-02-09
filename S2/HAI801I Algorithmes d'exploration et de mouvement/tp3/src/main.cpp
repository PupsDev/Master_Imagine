#include <iostream>
#include <vector>
#include <queue>
#include <math.h>
#include <cstring>
using namespace std;

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
        void print(){std::cout<<"indice: "<<indice<<" data: "<<data<<std::endl;}
        void resetDiscovered()
        {
            this->discovered=false;
            for(auto child:children)
            {
                if(child->discovered)
                    child->resetDiscovered();
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
/*unsigned  long long */int charToLongInt(char * bitboard, int n)
{
    /*unsigned  long long */int x=1<<(n);
    /*unsigned  long long */int result=0;
   
    for(int i = 0 ; i <= n ; i++)
    {
        /*unsigned  long long */int b = bitboard[i]-'0';
        result+=x&&b ? x : 0;
        //cout<<x<<endl;
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
    //cout<<"val: "<<n<<endl;
    n = n == 0 ? 1 :n; 
    for(int i = n; i<(size*size);i++)result+='0';

    //cout<<result<<"\n";
    while(bit>0&&x!=0)
    {
        result += (x<=bit)+'0';
         //cout<<x<<" "<<bit<<"\n";
        /*if(x>bit){
            result+='0';
            cout<<"ok";
        }*/
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
    /*
        000+
        000+
        000
        N*N as char *
    */
    public:
        //char* bitboard;
        /*unsigned  long long *///int bitboard;
        int bitboard[2];
        int size;
        // 0 for O and 1 for X
        bool turnState;

    State(int size)
    {
        //int size2 = size*size;

        /*
            Allocation du tableau et transformation en int
            int -> 5
            sinon choisir plus ou tableau de int
        */
        
        /*
        char * bitboardCharO = (char*)malloc(sizeof(char*)*size2);
        char * bitboardCharX = (char*)malloc(sizeof(char*)*size2);
        for(int i = 0 ; i < size2;i++)
        {
            bitboardCharO[i]='0';
            bitboardCharX[i]='1';


        }

        //printBitBoard(bitboardCharO);
        //printBitBoardAsBoard(bitboardCharO,  size2);

        this->bitboard[0] =charToLongInt(bitboardCharO,strlen(bitboardCharO+1));
        this->bitboard[1] =charToLongInt(bitboardCharX,strlen(bitboardCharX+1));
        free(bitboardCharO);
        free(bitboardCharX);
        */
        this->size=size;
        this->bitboard[0]=512;
        this->bitboard[1]=4;
   
    }
    void printBoard()
    {
        cout<<"Board :\n";
        string boardStringO = intToChar( this->bitboard[0],size);  
        string boardStringX = intToChar( this->bitboard[1],size); 
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
int main() {

   
    /*char * bitString = (char*)"110001";
    int bit =charToint(bitString,strlen(bitString+1)); 
    cout<<bit;
    cout<<"\n"+intToChar(49);*/

    State init = State(3);
    init.printBoard();


}