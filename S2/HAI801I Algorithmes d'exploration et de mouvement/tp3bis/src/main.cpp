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
typedef struct board{
    int side[2];
}BOARD;
template<typename T>
class Node
{
    public:
        Node(T turn)
        {
            this->turn=turn;
            this->board.side[0] = 0;
            this->board.side[1] = 0;
            this->value=0;
        }
        Node(Node* parent)
        {
            this->turn=parent->turn;
            this->board.side[0] = parent->board.side[0];
            this->board.side[1] = parent->board.side[1];
            this->value=0;
        }
        void printBoard(BOARD board)
        {
            
            cout<<"\nBoard :\n";
            string boardStringO = intToChar( board.side[0],SIZE);  
            string boardStringX = intToChar( board.side[1],SIZE); 

            char c = this->turn==0 ? 'O' : 'X';
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
    
        /*void resetDiscovered()
        {
            this->discovered=false;
            for(auto child:children)
            {
                if(child->discovered)
                    child->resetDiscovered();
            }

        }*/

        void tryAllMove()
        {
            for(int i = 0 ; i < SIZE*SIZE;i++)
            {
                play(i+1);

            }
        }
        int checkMove(int i,BOARD board)
        {
            int ennemyPos = board.side[0]>>i;
            int Pos =       board.side[1]>>i;


            return ( !(ennemyPos&1) && !(Pos&1) );
            
        }
        void undo(BOARD &board2,int turn,int move)
        {
            int i = move-1;
            int x=(1<<i);
            board2.side[turn]-=x;
        }
        void play(BOARD &board2,int turn,int move)
        {
           
            int i = move-1;
            int x = (1<<i);
            if(checkMove(i+1,board2)&& (board.side[0]+board.side[1])<512 )
            {
                board2.side[turn]+=x;

                

            }   
            if(checkWin(board2.side[turn]))
            {

                if(turn==1)value= -1;
                if(turn==0)value= 1;
            }        
            //string boardStringO = intToChar( board.side[0],SIZE);  
            //string boardStringX = intToChar( board.side[1],SIZE); 
            //cout<<boardStringO<<"\n";
            //cout<<boardStringX<<"\n";
             
            //if(value)cout<<"WIN";
            //turn = 1-turn;
        }
        int minimax(BOARD &board2,int depth,int player)
        {
            int res;

            if(checkWin(board2.side[0]) || checkWin(board2.side[1]) )
            {

                if(player==1)return -1;
                if(player==0)return 1;
            }
            else if((board2.side[0]+board2.side[1])==511)
                return 0;
            else 
            {
                if(depth>0)
                {
                    if(player==1)
                    {
                        res = -INT32_MAX;
                        for(int i = 0 ; i < (SIZE*SIZE);i++)
                        {
                            if(checkMove(i+1,board2))
                            {
                        
                                play(board2,1,i+1);
                         
                                res = max(res,minimax(board2,depth-1,0));                                
                                undo(board2,1,i+1);
                            }   
                        }
                        return res;

                    }
                    else if(player==0)
                    {
                        res = INT32_MAX;
                        for(int i = 0 ; i < (SIZE*SIZE);i++)
                        {
                            
                            if(checkMove(i+1,board2))
                            {   
                                
                                play(board2,0,i+1);
                                res = min(res,minimax(board2,depth-1,1));
                                
                                undo(board2,0,i+1);
             
                
   
                            }    
                        }
                        return res;        
                    }
                }
                else
                {
                    return 0;
                }
            }
            return 0;
           
        }
        
        int findMove()
        {
            int bestScore=-1000;
            int move=-1;
            int depth = 10;
            BOARD board2;
            board2.side[0]=board.side[0];
            board2.side[1]=board.side[1];

            //turn = !turn;
            printBoard(board2);
        
            for(int i = 0 ; i < (SIZE*SIZE);i++)
            {
                if(checkMove(i+1,board2))
                {
                     
 
                    play(board2,turn,i+1);
                     
                    int res = minimax(board2,depth-1,!turn ); 
                    //cout<<"side 0; "<<board.side[0]<<"side 1; "<<board.side[1]<<"\n";
                    string boardStringO = intToChar( board2.side[0],SIZE);  
                    string boardStringX = intToChar( board2.side[1],SIZE); 
                    //cout<<boardStringO<<"\n";
                    //cout<<boardStringX<<"\n";

                    undo(board2,turn,i+1);

                    boardStringO = intToChar( board2.side[0],SIZE);
                    boardStringX = intToChar( board2.side[1],SIZE);
                    //cout<<boardStringO<<"\n";
                    //cout<<boardStringX<<"\n\n";
                    //printBoard(board);
                    
                    if(res>bestScore)
                    {
                        bestScore = res;
                        move = i;
                    }                            

                }   
            }
            
            cout<<"Move : "<<move<<endl;
            return move;

        }
        std::vector<Node*> children;
        int value;
        BOARD board;
        T turn;
};

int main() {

    Node<int>*root = new Node<int>( 0);
    //int turn = 0;
    int move = -1;

    root->play(root->board,root->turn,1);

    root->printBoard(root->board);
/*
    move = root->findMove();
    root->play(root->board,!root->turn,move+1);
    root->printBoard(root->board);
    */

/*
    move = root->findMove();
    root->play(root->board,root->turn,move+1);
    root->printBoard(root->board);*/



    while((root->value!=1 || root->value!=-1 ||root->value!=0) )//|| (root->board.side[0]+root->board.side[1])<512 )
    {
        move = root->findMove();
        root->turn= !root->turn;
        root->play(root->board, root->turn,move+1);
        root->printBoard(root->board);
         cout<<"STATE:"<<root->value<<"\n";

    }
    cout<<"STATE:"<<root->value;

    
    /*
    root->play(root->board,!root->turn,1);

    root->play(root->board,root->turn,2);
    root->play(root->board,!root->turn,3);

    root->play(root->board,root->turn,4);
    root->play(root->board,!root->turn,5);

    root->play(root->board,!root->turn,6);

    root->printBoard(root->board);

    move = root->findMove();
    */
    /*
    root->play(root->board,root->turn,move);
    root->printBoard(root->board);

    root->play(root->board,!root->turn,4);
    root->printBoard(root->board);

    //root->turn = !root->turn;
    move = root->findMove();

    root->play(root->board,root->turn,move);
    root->printBoard(root->board);
/*
    //root->turn = !root->turn;
    move = root->findMove();
    root->play(root->board,root->turn,move);
    root->printBoard(root->board);

    //root->turn = !root->turn;
    move = root->findMove();
    root->play(root->board,!root->turn,move);
    root->printBoard(root->board);
/*
        //root->turn = !root->turn;
    move = root->findMove();
    root->play(root->board,root->turn,move+1);
    root->printBoard(root->board);*/
    /*
root->play(4);
    root->play(2);
root->play(5);    
    root->play(3);
    */
    
    //int move=0;

    return 0;
    
}