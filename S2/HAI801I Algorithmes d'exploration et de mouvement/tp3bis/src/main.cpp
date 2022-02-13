#include <iostream>
#include <vector>
#include <queue>
#include <math.h>
#include <cstring>
using namespace std;

#define SIZE 3

int checkWin(int bitboard)
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
    
    return ((bitboard&c1)==c1) ||
            ((bitboard&c2)==c2) ||
            ((bitboard&c3)==c3) ||
            ((bitboard&l1)==l1) ||
            ((bitboard&l2)==l2) ||
            ((bitboard&l3)==l3) ||
            ((bitboard&d1)==d1) ||
            ((bitboard&d2)==d2);

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
template<typename T>
class Node
{
    public:
        Node(T turn)
        {
            this->turn=turn;
            this->board[0] = 0;
            this->board[1] = 0;
            this->value=0;
        }
        Node(Node* parent)
        {
            this->turn=parent->turn;
            this->board[0] = parent->board[0];
            this->board[1] = parent->board[1];
            this->value=0;
        }
        void printBoard()
        {
            
            cout<<"\nBoard :\n";
            string boardStringO = intToChar( this->board[0],SIZE);  
            string boardStringX = intToChar( this->board[1],SIZE); 

            char c = this->turn==1 ? 'O' : 'X';
            printf("TOUR: %c \n",c);
            for(int i = 0 ; i < SIZE;i++)
            {
                for(int j = 0 ; j < SIZE;j++)
                {

                    if( boardStringO[i*SIZE+j]=='1')
                    {
                        cout<<"[O]"<<" ";
                    }
                    else if( boardStringX[i*SIZE+j]=='1')
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
    
        void resetDiscovered()
        {
            this->discovered=false;
            for(auto child:children)
            {
                if(child->discovered)
                    child->resetDiscovered();
            }

        }

        void tryAllMove()
        {
            for(int i = 0 ; i < SIZE*SIZE;i++)
            {
                play(i+1);

            }
        }
        int checkMove(int i)
        {
            int ennemyPos = board[!turn]>>i;
            int Pos =       board[turn ]>>i;

            return ( !(ennemyPos&1) && !(Pos&1) );
            
        }
        int play(int move)
        {
           
            int i = move-1;
            int x = (1<<i);
            int res;
            if(!checkWin(board[turn]))
            {
                if(checkMove(i))
                {
                    board[turn]+=x;
                }

            }            
            turn = 1-turn;
        }
        std::vector<Node*> children;
        int value;
        int board[2];
        T turn;
};
int checkMove(int board[2], int i, int turn)
{
    int ennemyPos = board[!turn]>>i;
    int Pos =       board[turn ]>>i;

    return ( !(ennemyPos&1) && !(Pos&1) );
    
}
int play(int board[2],int turn,int move)
{
    
    int i = move-1;
    int x = (1<<i);
    int res;
    
    if(checkMove(board,i,turn))
    {
        board[turn]+=x;
    }
        
    res = checkWin(board[turn]);
    turn = 1-turn;
    return res;
}
int minimax(int board[2],int depth,int player, int &move)
{
    int res;
    int score = 0;

    int bestScore = 0;
    if(depth == 0 || (board[0]+board[1])==511)
    {
        int res;
        if(checkWin(board[player]))
        {
            res = player == 0 ? 1 : -1;
        }
        else 
            res =0;
        return res;
    }
    if(player)
    {
        res = -INT32_MAX;
        for(int i = 0 ; i < SIZE*SIZE;i++)
        {
            if(checkMove(board,i,player))
            {
                play(board,player,i+1);
                res = minimax(board,depth-1,!player,move);
                int x = (1<<i);
                board[player]-=x;

                if(res > bestScore)
                {
                    bestScore = res;
                    move=i;
                }
            }    

        }
        //cout<<"Move: "<<move<<"\n";
    }
    else
    {
        res = INT32_MAX;
        
        for(int i = 0 ; i < SIZE*SIZE;i++)
        {
            if(checkMove(board,i,player))
            {
                play(board,player,i+1);
                res = minimax(board,depth-1,player,move);
                int x = (1<<i);
                board[player]-=x;
                
                if(res < bestScore)
                {
                    bestScore = res;
                    move=i;
                }
            }    

        }
        //cout<<"Move: "<<move<<"\n";
        
    }
    return res;
}

int main() {

    Node<int>*root = new Node<int>( 0);
    //root->play(1);
    //root->printBoard();
    //root->play(1);
    //root->tryAllMove();
    //root->turn = !root->turn;
    //root->tryAllMove();
    //root->try_move();
    root->play(1);
    //root->play(5);
    root->printBoard();
    int move=0;
    cout<<minimax(root->board,10,1,move);
    cout<<"Move ! -> "<<move;
    return 0;
    
}