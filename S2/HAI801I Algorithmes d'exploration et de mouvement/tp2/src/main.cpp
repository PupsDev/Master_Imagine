#include <iostream>
#include <vector>
#include <queue>

using namespace std;

typedef std::pair<int,int> SIDE;
// first for cannibal second for missinary

class State
{
    public:
        State()
        {
            this->boatSide =0;
            this->cannibal =make_pair(3,0);
            this->missionary =make_pair(0,3);

        }
        void transport(int people)
        {
            if(boatSide) // M
            {
                boat.second=people;
            }
            else
            {
                boat.first=people;

            }
        }


        void process()
        {
            if(boatSide) // M
            {
                this->cannibal.second+=boat.second;
            }
            else
            {
                this->missionary.first+=boat.first;

            }
            this->boatSide=1-this->boatSide;

            


        }
        void print()
        {
            cout<<"       |            |          \n";
            for(int i = 0 ; i < this->cannibal.first ;i++ )
            {
                cout<<"C ";
            }
            for(int i = 0 ; i < this->cannibal.second ;i++ )
            {
                cout<<"M ";
            }
            cout<<" |            | ";
            for(int i = 0 ; i < this->missionary.first ;i++ )
            {
                cout<<"C ";
            }
            for(int i = 0 ; i < this->missionary.second ;i++ )
            {
                cout<<"M ";
            }
             cout<<"\n";
            cout<<"       |            |          ";
             cout<<"\n";
        }

    public:
        bool boatSide;
        SIDE cannibal,missionary;
        SIDE boat;

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
        void print(){std::cout<<"indice: "<<indice<<"\n"; this->data->print();}
        void resetDiscovered()
        {
            this->discovered=false;
            for(auto child:children)
            {
                if(child->discovered)
                    child->resetDiscovered();
            }

        }
        void haveChild()
        {
            data->process();
            if(data->boatSide)
            {

                if(data->missionary.second==1)
                {
                    State * state1 = new State();
                    Node< State* >* choice1 = new Node< State* >( state1 ,1);
                    state1->transport(1);
                    state1->process();
                    children.push_back(choice1);
                    choice1->haveChild();
                }
                if(data->missionary.second>1)
                {
                    State * state1 = new State();
                    Node< State* >* choice1 = new Node< State* >( state1 ,1);
                    state1->transport(1);
                    state1->process();
                    children.push_back(choice1);
                    choice1->haveChild();

                    State * state2 = new State();
                    Node< State* >* choice2 = new Node< State* >( state2 ,2);
                    state2->transport(2);
                    state2->process();
                    children.push_back(choice2);
                    choice2->haveChild();
                }

            }
            else
            {

                if(data->cannibal.first==1)
                {
                    State * state1 = new State();
                    Node< State* >* choice1 = new Node< State* >( state1 ,1);
                    state1->transport(1);
                    state1->process();
                    children.push_back(choice1);
                    choice1->haveChild();
                }
                if(data->cannibal.first>1)
                {
                    State * state1 = new State();
                    Node< State* >* choice1 = new Node< State* >( state1 ,1);
                    state1->transport(1);
                    state1->process();
                    children.push_back(choice1);
                    choice1->haveChild();

                    State * state2 = new State();
                    Node< State* >* choice2 = new Node< State* >( state2 ,2);
                    state2->transport(2);
                    state2->process();
                    children.push_back(choice2);
                    choice2->haveChild();
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
/*
    Node<char>* A = new Node<char>('A',0);

    Node<char>* B = new Node<char>('B',1);
    Node<char>* C = new Node<char>('C',2);

    Node<char>* D = new Node<char>('D',3);
    Node<char>* E = new Node<char>('E',4);
   

    A->children.push_back(B);

    B->children.push_back(C);
    
    C->children.push_back(D);

    D->children.push_back(B);
    D->children.push_back(E);
    */
/*
    std::cout << "DFS" << std::endl;
    DFS_recursive(A);
    A->resetDiscovered();
    std::cout << "BFS" << std::endl;
    BFS_iterative(A);
*/
    //State* init=new State();

    Node< State* >* init = new Node< State* >( new State(),0);

    init->haveChild();


    

        
    BFS_iterative(init);
    





}