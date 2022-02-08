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
            this->cannibal =make_pair(5,0);
            this->missionary =make_pair(0,5);
            this->people=0;
            this->labelChain= string("");

        }
        State(State* parent)
        {
            this->boatSide = !parent->boatSide;
            this->cannibal =parent->cannibal;
            this->missionary =parent->missionary;
            this->people=0;
            this->labelChain+=parent->labelChain;

        }
        void transportold(int people)
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
                //boat.first=0;
                
            }
            else
            {
                 // Bateau va à gauche on enleve les cannibal du côté missionnaiore 
                
                this->cannibal.second+=boat.second;
                //boat.second=0;
                
            }
            


        }
        void print()
        {
            count++;
            
            //cout<<"Iteration"<<count<<"\n";
            /*
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
            
            */
            this->etatPrint = "";
            if(!boatSide)
                etatPrint+="[B] ";
            for(int i = 0 ; i < this->cannibal.first ;i++ )
            {
                etatPrint+="C ";
            }
            for(int i = 0 ; i < this->cannibal.second ;i++ )
            {
                etatPrint+="M ";
            }
            etatPrint+=" |       [";
            for(int i = 0 ; i < this->boat.first ;i++ )
            {
                etatPrint+="C ";
            }
            for(int i = 0 ; i < this->boat.second ;i++ )
            {
                etatPrint+="M ";
            }

            etatPrint+="]      | ";
            if(boatSide)
                etatPrint+="[B] ";
            for(int i = 0 ; i < this->missionary.first ;i++ )
            {
                etatPrint+="C ";
            }
            for(int i = 0 ; i < this->missionary.second ;i++ )
            {
                etatPrint+="M ";
            }
        }

        bool checkCannibalSide()
        {
            if(cannibal.second)
             return (cannibal.first-boat.first) <= (cannibal.second+boat.second);
            else return true;
        }
        bool checkMissionarySide()
        {
            if(missionary.second)
             return (missionary.first-boat.first) <= (missionary.second+boat.second);
            else return true;
        }
        bool finish()
        {
            return cannibal.second==5 && missionary.first==5;
        }
    public:
        bool boatSide;
        SIDE cannibal,missionary;
        SIDE boat;
        int people;
        static int count;
        string etatPrint;
        string label;
        string labelChain;

};
int State::count =0;
void printp(SIDE pair)
{
    cout<< "cannibals"<<pair.first<<" missionaries "<<pair.second<<endl;
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
        void createNode(int people)
        {
            State * state = new State(data);
            Node< State* >* choice = new Node< State* >( state ,this->indice*10+people);
            choice->data->label = data->boatSide == 0 ? "C"+to_string(people) : "M"+to_string(people);
            choice->data->labelChain +=choice->data->label +"->";
 
            state->transport(people);
            state->process();
            

            children.push_back(choice);
            choice->haveChild();

        }
        void haveChild()
        {

            if(data->boatSide) // M side
            {
                if(data->missionary.second==1)
                    createNode(1);
                if (data->missionary.second>1)
                {
                    createNode(1);
                    createNode(2);
                }

            }
            else // C side
            {
                if(data->cannibal.first==1)
                    createNode(1);
                if (data->cannibal.first>1)
                {
                    createNode(1);
                    createNode(2);
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
string BFS_iterative(Node<T>* root)
{
    string test="digraph G {\n node [shape=record fontname=Arial];\n";
    std::queue<Node<T>*> Q;


     Q.push(root);
     std::vector<string> labelNodes;

     while(!Q.empty())
     {
         Node<T>* v = Q.front();
         Q.pop();
         v->print();
         v->discovered=true;

         string label = "v"+to_string(v->indice);
        labelNodes.push_back(label+" [label=\""+v->data->etatPrint+"\"]\n");
        int indicee =1;
        if(v->children.empty())
        {
            if(v->data->finish())
                cout<<v->data->labelChain<<endl;
        }
         for(auto child : v->children)
         {
            if(!child->discovered  )//&& child->data->checkCannibalSide() && child->data->checkMissionarySide() )
            {
                test+="v"+to_string(v->indice)+" ->"; 
                
                 test+="v"+to_string(child->indice)+"  [label="+child->data->label+"];\n";
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

     //cout<<test;
     return test;
}

int main() {


    bool startSide = false;
    Node< State* >* init = new Node< State* >( new State(startSide),0);
    init->haveChild();
    string test = BFS_iterative(init);
    cout<<"Itérations: "<<init->data->count<<endl;
        FILE *fp;
    fp = fopen("graphLeft.dot", "wb+");
    fprintf(fp,test.data());
    fclose(fp);

    init = new Node< State* >( new State(!startSide),0);
    init->haveChild();
    test = BFS_iterative(init);
    cout<<"Itérations: "<<init->data->count<<endl;
    fp = fopen("graphRight.dot", "wb+");
    fprintf(fp,test.data());
    fclose(fp);
    





}