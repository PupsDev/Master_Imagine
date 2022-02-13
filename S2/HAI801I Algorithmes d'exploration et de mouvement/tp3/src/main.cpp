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
void printBitBoardAsBoard(const char * bitboard, int size)
{
    cout<<"bitboard:\n";
    for(int i = 0 ; i < size;i++)
    {
        for(int j = 0 ; j < size;j++)
        {
             cout<<bitboard[i*size+j]+'0'<<" ";
        }
        cout<<"\n";
    }
}
void printBitBoardAsBoard(std::string bitboard,int size)
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
         static int count;
        int turnState;
        int value;

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
        int check(int bitboard)
        {
            int c1,c2,c3;
            int l1,l2,l3;
            int d1,d2;
            c1= 1+(1<<3) +(1<<(2*3));
            c2= c1<<1;
            c3= c2<<1;

            l1= 1+2+4;
            l2= l1<<3;
            l3= l2<<3;

            d1= 1 + (1<<(3+1)) + (1<<2*(3+1));
            d2= (1<<2) + (1<<4) + (1<<6);
            /*
            string Sc1 = intToChar( c1,data->size); 
            string Sc2 = intToChar( c2,data->size);  
            string Sc3 = intToChar( c3,data->size);  
            string Sl1 = intToChar( l1,data->size); 
            string Sl2 = intToChar( l2,data->size);  
            string Sl3 = intToChar( l3,data->size);  
            string Sd1 = intToChar( d1,data->size);  
            string Sd2 = intToChar( d2,data->size);  

            cout<<"c1:"<<Sc1<<endl;
            cout<<"c2:"<<Sc2<<endl;
            cout<<"c3:"<<Sc3<<endl;
            cout<<"l1:"<<Sl1<<endl;
            cout<<"l2:"<<Sl2<<endl;
            cout<<"l3:"<<Sl3<<endl;
            cout<<"d1:"<<Sd1<<endl;
            cout<<"d2:"<<Sd2<<endl;
            
            cout<<"c1:"<<c1<<endl;
            cout<<"c2:"<<c2<<endl;
            cout<<"c3:"<<c3<<endl;
            cout<<"l1:"<<l1<<endl;
            cout<<"l2:"<<l2<<endl;
            cout<<"l3:"<<l3<<endl;
            cout<<"d1:"<<d1<<endl;
            cout<<"d2:"<<d2<<endl;
            */
            
            return ((bitboard&c1)==c1) ||
                    ((bitboard&c2)==c2) ||
                    ((bitboard&c3)==c3) ||
                    ((bitboard&l1)==l1) ||
                    ((bitboard&l2)==l2) ||
                    ((bitboard&l3)==l3) ||
                    ((bitboard&d1)==d1) ||
                    ((bitboard&d2)==d2);


        }
    void play(int i)
    {   

        //cout<<"\nPlay : "<<i<<"\n";
       /* int O = this->bitboard[0];
        int X = this->bitboard[1];
        string boardStringO = intToChar( this->bitboard[0],size);  
        string boardStringX = intToChar( this->bitboard[1],size); 
        cout<<boardStringO<<"\n";
        cout<<boardStringX<<"\n";
        cout<<O<<" "<<X<<"\n";
         string boardStringOX = intToChar(this->bitboard[0]+this->bitboard[1],size); 
        cout<<O+X<<"\n";
         cout<<boardStringOX<<"\n";
         */

        // Ennemmy at pos i
        int x = (1<<i);
        this->bitboard[turnState]+=x;
        int O =  this->bitboard[0];
        int X =  this->bitboard[1];

        //cout<<(O+X)<<endl;
        if( (O+X) >= 511)
        {
            this->value = 0;  
            //cout<<"TIE"<<endl; 
        }
        if(check(O))
        {

            this->value = -1;
            //cout<<"LOSE"<<endl;

        }
        if(check(X))
        {   

            this->value = 1;
            //cout<<"WIN"<<endl;

        }
        turnState = !turnState;

    }
    void printBoard()
    {
        
        cout<<"\nBoard :\n";
        string boardStringO = intToChar( this->bitboard[0],size);  
        string boardStringX = intToChar( this->bitboard[1],size); 
        //cout<<boardStringO<<" \n";
        //cout<<boardStringX<<" \n";
        //printBitBoardAsBoard(boardStringO,size);
        //printBitBoardAsBoard(boardStringX,size);

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
            data->count++;
            choice->coup = people;
            choice->data->play(people);
            choice->play();
            //cout<<"VALUE: "<<choice->data->value<<"\n";
            children.push_back(choice);

        }
        void createNode2(int people)
        {
            State * state = new State(data);
            Node< State* >* choice = new Node< State* >( state ,this->indice*10+people);
            data->count++;
            choice->coup = people;
            choice->data->play(people);
            choice->play();
            //cout<<"VALUE: "<<choice->data->value<<"\n";
            children.push_back(choice);

        }

        void playHuman(int d)
        {
            d = d-1;

            int ennemyPos = data->bitboard[!data->turnState]>>d;
            int Pos = data->bitboard[data->turnState]>>d;
            data->play(d);
            data->printBoard();
            if( !(ennemyPos&1) && !(Pos&1) )
            {
                createNode(d);
            }
            
            

        
        }
        /*int play2()
        {
            State * state = new State(data);
            Node< State* >* choice = new Node< State* >( state ,this->indice*100);

            for(int i = 0 ; i < data->size2;i++)
            {
                int ennemyPos = data->bitboard[!data->turnState]>>i;
                int Pos = data->bitboard[data->turnState]>>i;

                if( !(ennemyPos&1) && !(Pos&1) )
                {

                    State * stateSon = new State(data);
                    Node< State* >* choiceSon = new Node< State* >( stateSon ,this->indice*10+i);;
                    choiceSon->data->play(i);
                    choiceSon->play2();
                    choice->children.push_back(choiceSon);
                }
                
            }
            int cop;
            return choice->minimax(100,0,cop);
            for(auto& child : choice->children)
            {
                int cop;
                cout<<"minimax"<<child-><<"\n";
            }
            
         
        }*/
        void play()
        {

            for(int i = 0 ; i < data->size2;i++)
            {
                int ennemyPos = data->bitboard[!data->turnState]>>i;
                int Pos = data->bitboard[data->turnState]>>i;

                if( !(ennemyPos&1) && !(Pos&1) )
                {
                    createNode(i);
                }
            }            
        }
        int minimax(int depth,int player)
        {
            
            //return 1;
            int res;
            if(depth ==0 || this->children.empty())
            {
      
                return this->data->value;
            }
            else if(player)
            {
                res = -INT32_MAX;
                for(auto& child: this->children)
                {
                   
                    int play = child->minimax(depth-1,!player);
                    if(play > res )
                    {
                        res = play;

                    }
                    
                }
            }
            else if(!player)
            {
                res = INT32_MAX;
                for(auto& child: this->children)
                {
                    int play = child->minimax(depth-1,player);
                    if(play < res )
                    {
                        res = play;

                    }
                }
            }
            return res;
        }
        std::vector<Node*> children;
        size_t indice;
        int value;
        int coup;
        bool discovered;
        T data;
};

int State::count =0;
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
    
    init->playHuman(2);
    init->playHuman(8);
    /*
    init->playHuman(4);
    init->playHuman(5);

    init->playHuman(8);
    init->playHuman(7);*/

    init->play();

    int nextMove=-1;
    int bestscore = -INT32_MAX;

    for(int i = 0 ; i < init->children.size();i++)
    {

        int score = init->children[i]->minimax(1,1);
        if(score>bestscore)
        {
            //cout<<"score:"<<score<<endl;
            score = bestscore;
            nextMove = i;
        }
    }

    init->children.clear();
    cout<<nextMove<<"\n";
    //init->playHuman(nextMove);



    //init->play();
    /*init->playHuman(2);

    int nextMove=-1;
    int bestscore = -INT32_MAX;
    init->play();
    for(int i = 0 ; i < init->children.size();i++)
    {
        //cout<<"joue "<<i<<"\n";
        cout<<"joue2 "<<init->children[i]->coup<<"\n";

        int score = init->children[i]->minimax(1,init->data->turnState);
        if(score>bestscore)
        {
            cout<<"score:"<<score<<endl;
            score = bestscore;
            nextMove = i;
        }
    }
    init->children.clear();
    cout<<nextMove<<"\n";

    init->playHuman(2);
    */
    /*for(int i = 0 ; i < 513;i++)
    {
        if(init->check(i))
            cout<<i<<":"<<intToChar( i,3)<<"\n";
    }*/
    //cout<<init->check(511);
    

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
    //BFS_iterative(init);
    /*for(int i = 0 ; i < (size*size);i++)
    {
        init.play(i);
        init.printBoard();
    }*/


}