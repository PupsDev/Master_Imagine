#include <iostream>
#include <vector>
#include <queue>

using namespace std;

typedef std::pair<int,int> SIDE;
// first for cannibal second for missinary
/*
Sans régles :
Iteration26
Traversée de 1 Missionaire
       |            |
M M M  |            | C C C
       |            |

Avec régles:

*/
class State
{
    public:
        State(bool boatSide)
        {
            this->boatSide =boatSide;
            this->cannibal =make_pair(3,0);
            this->missionary =make_pair(0,3);
            this->people=0;

        }
        State(State* parent)
        {
            this->boatSide = !parent->boatSide;
            this->cannibal =parent->cannibal;
            this->missionary =parent->missionary;
            this->people=0;

        }
        void transport(int people)
        {
            this->people = people;
            if(boatSide) // M
            {
                this->boat.first=people;
                this->cannibal.first-=people;

            }
            else
            {
                this->boat.second=people;
                this->missionary.second-=people;

            }
        }


        void process()
        {

            if(boatSide) // M
            {
                // Bateau va à droite on enleve les missionaire du côté cannibal 
                this->missionary.first+=boat.first;
                boat.first=0;
                
            }
            else
            {
                 // Bateau va à gauche on enleve les cannibal du côté missionnaiore 
                
                this->cannibal.second+=boat.second;
                boat.second=0;
                
            }
            


        }
        void print()
        {
            count++;
            cout<<"Iteration"<<count<<"\n";
            if(boatSide)
                cout<<"Traversée de "<<people<<" Cannibal \n";
            else
                 cout<<"Traversée de "<<people<<" Missionaire\n";

            cout<<"       |            |          \n";
            for(int i = 0 ; i < this->cannibal.first ;i++ )
            {
                cout<<"C ";
            }
            for(int i = 0 ; i < this->cannibal.second ;i++ )
            {
                cout<<"M ";
            }
            cout<<" |      ";
            for(int i = 0 ; i < this->boat.first ;i++ )
            {
                cout<<"C ";
            }
            for(int i = 0 ; i < this->boat.second ;i++ )
            {
                cout<<"M ";
            }

            cout<<"      | ";
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
            
            this->etatPrint = "";
            for(int i = 0 ; i < this->cannibal.first ;i++ )
            {
                etatPrint+="C ";
            }
            for(int i = 0 ; i < this->cannibal.second ;i++ )
            {
                etatPrint+="M ";
            }
            etatPrint+=" |      ";
            for(int i = 0 ; i < this->boat.first ;i++ )
            {
                etatPrint+="C ";
            }
            for(int i = 0 ; i < this->boat.second ;i++ )
            {
                etatPrint+="M ";
            }

            etatPrint+="      | ";
            for(int i = 0 ; i < this->missionary.first ;i++ )
            {
                etatPrint+="C ";
            }
            for(int i = 0 ; i < this->missionary.second ;i++ )
            {
                etatPrint+="M ";
            }
        }

    public:
        bool boatSide;
        SIDE cannibal,missionary;
        SIDE boat;
        int people;
        static int count;
        string etatPrint;

};
int State::count =0;
void printp(SIDE pair)
{
    cout<<pair.first<<" "<<pair.second<<endl;
}


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
        void print(){ /*std::cout<<"indice: "<<indice<<"\n";*/ this->data->print();}
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
            /*
            cout<<"Side:"<<data->boatSide<<endl;
            data->process();
            cout<<"Side:"<<data->boatSide<<endl;
            */

            /*
            if(data->missionary.first>data->missionary.second && data->missionary.second>0)
            {   
                printp(data->missionary);
                printp(data->cannibal);
            }
            if(data->cannibal.first>data->cannibal.second && data->cannibal.second>0)
            {
                printp(data->missionary);
                printp(data->cannibal);

            }
            */
            if(data->boatSide)
            {

                if(data->missionary.second==1)
                {
                    State * state1 = new State(data);
                    Node< State* >* choice1 = new Node< State* >( state1 ,this->indice*10+1);
                    state1->transport(1);
                    state1->process();
                    children.push_back(choice1);
                    //choice1->print();
                    choice1->haveChild();
                }
                if(data->missionary.second>1)
                {
                    State * state1 = new State(data);
                    Node< State* >* choice1 = new Node< State* >( state1 ,this->indice*10+1);
                    state1->transport(1);
                    state1->process();
                    cout<< "1 cannibal \n";
                    //choice1->print();
                    children.push_back(choice1);
                    choice1->haveChild();

                    State * state2 = new State(data);
                    Node< State* >* choice2 = new Node< State* >( state2 ,this->indice*10+2);
                    state2->transport(2);
                    state2->process();
                    cout<< "2 cannibal \n";
                    //choice2->print();
                    children.push_back(choice2);
                    choice2->haveChild();
                }

            }
            else
            {
                //if(data->cannibal.first> data->cannibal.second && data->cannibal.second>0)
                    //return;
                if(data->cannibal.first==1)
                {
                    State * state1 = new State(data);
                    Node< State* >* choice1 = new Node< State* >( state1 ,this->indice*10+1);
                    state1->transport(1);
                    state1->process();
                    children.push_back(choice1);
                    //choice1->print();
                    choice1->haveChild();
                }
                if(data->cannibal.first>1)
                {
                    State * state1 = new State(data);
                    Node< State* >* choice1 = new Node< State* >( state1 ,this->indice*10+1);
                    state1->transport(1);
                    state1->process();
                    children.push_back(choice1);
                    cout<< "1 Missionaire \n";
                     //choice1->print();
                    choice1->haveChild();

                    State * state2 = new State(data);
                    Node< State* >* choice2 = new Node< State* >( state2 ,this->indice*10+2);
                    state2->transport(2);
                    state2->process();
                    children.push_back(choice2);
                    cout<< "2 Missionaire \n";
                    //choice2->print();
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
    string test="digraph G {\n node [shape=record fontname=Arial];\n";
    std::queue<Node<T>*> Q;


     Q.push(root);
     std::vector<string> labelNodes;
     int indicep=0;
     int indice=0;
     while(!Q.empty())
     {
         Node<T>* v = Q.front();
         Q.pop();
         v->print();
         v->discovered=true;
         string label = "v"+to_string(v->indice);
        labelNodes.push_back(label+" [label=\""+v->data->etatPrint+"\"]\n");
        int indicee =1;
         for(auto child : v->children)
         {
            if(!child->discovered)
            {
                test+="v"+to_string(v->indice)+" ->"; 
                
                 test+="v"+to_string(child->indice)+"  [label=e1];\n";
                 indicee++;
                child->discovered=true;
                Q.push(child);

            }
         }


     }
     for(auto label : labelNodes)
     {
         test+=label;
     }
     test+="}\n";

     cout<<test;
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

    bool startSide = false;
    Node< State* >* init = new Node< State* >( new State(startSide),0);


    init->haveChild();
      /*init->print();
    
        startSide=!startSide;
        
        State * state1 = new State(startSide);
         std::cout<<state1->boatSide<<std::endl;
        Node< State* >* choice1 = new Node< State* >( state1 ,1);
    
        state1->transport(1);
        state1->process();
        init->children.push_back(choice1);

        choice1->print();
        

        State * state2 = new State(startSide);
        std::cout<<state2->boatSide<<std::endl;
        Node< State* >* choice2 = new Node< State* >( state2 ,1);
        
        
        state2->transport(2);
        state2->process();
        init->children.push_back(choice2);

        choice2->print();
        // 2 level

        startSide=!startSide;
        State * stateM1 = new State(startSide);
        std::cout<<stateM1->boatSide<<std::endl;
        Node< State* >* choice11 = new Node< State* >( stateM1 ,2);
 
        stateM1->transport(1);
         stateM1->process();

        choice1->children.push_back(choice11);
        choice11->print();*/


/*
        State * state2 = new State();
        Node< State* >* choice2 = new Node< State* >( state2 ,2);
        state2->transport(2);
        state2->process();
        init->children.push_back(choice2);*/



    

        
    BFS_iterative(init);
    





}