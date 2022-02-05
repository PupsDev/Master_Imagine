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
M M M  |            | 
CCC    |            |

Avec régles:

*/
class State
{
    public:
        State(bool boatSide)
        {
            this->boatSide =boatSide;
            this->cannibal =make_pair(3,3);
            this->missionary =make_pair(0,0);
            this->cannibalCount=0;
            this->missionaryCount=0;
            this->labelChain= string("");
            this->leftSeen.push_back("CCCMMM");
            this->rightSeen.push_back("");
            this->leftS = string("CCCMMM");
            this->rightS = string("");

        }
        State(State* parent)
        {
            this->boatSide = !parent->boatSide;
            this->cannibal =parent->cannibal;
            this->missionary =parent->missionary;
            this->cannibalCount=0;
            this->missionaryCount=0;
            this->labelChain+=parent->labelChain;
            this->leftSeen= parent->leftSeen;
            this->rightSeen = parent->rightSeen;

        }

        void transport(int cannibal, int missionary)
        {
            //this->people = people;
            this->cannibalCount = cannibal;
            this->missionaryCount = missionary;
            if(boatSide) // M
            {
                this->boat.first=cannibal;
                this->boat.second=missionary;

                this->cannibal.first-=cannibal;
                this->cannibal.second-=missionary;

            }
            else
            {
                this->boat.second=missionary;
                this->boat.first=cannibal;

                this->missionary.second-=missionary;                
                this->missionary.first-=cannibal;

            }
        }

        string computeSeen(bool side)
        {
            string res = "";
            if(side)
            {
                for(int i = 0 ; i < cannibal.first ;i++)
                {
                    res+="C";
                }
                for(int i = 0 ; i < cannibal.second ;i++)
                {
                    res+="M";
                }
            }
            else
            {
                for(int i = 0 ; i < missionary.first ;i++)
                {
                    res+="C";
                }
                for(int i = 0 ; i < missionary.second ;i++)
                {
                    res+="M";
                }

            }
            return res;
        }
        void process()
        {

            if(boatSide) // M
            {
                // Bateau va à droite on enleve les missionaire du côté cannibal 
                this->missionary.first+=boat.first;
                this->missionary.second+=boat.second;
                //boat.first=0;
                this->leftS=computeSeen(0);

                
            }
            else
            {
                 // Bateau va à gauche on enleve les cannibal du côté missionnaiore 
                
                this->cannibal.second+=boat.second;
                 this->cannibal.first+=boat.first;
                 this->rightS=computeSeen(1);
                //boat.second=0;
                
            }

            


        }
        void print()
        {
            count++;
            
            //cout<<"Iteration"<<count<<"\n";
            
            cout<<"Traversée de "<<this->cannibalCount<<" Cannibal \n";
            
            cout<<"Traversée de "<<this->missionaryCount<<" Missionaire\n";

            cout<<"             |            |          \n";
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
            cout<<"             |            |          \n";
             cout<<"\n";
            
            
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
            return cannibal.second==3 && missionary.first==3;
        }
        bool checkSeen(string seen, bool side)
        {
            bool check;
            if(side)
            {
                for(auto& lseen: leftSeen)
                {
                    return (seen==lseen);
                        
                }
                check = false;
            }
            else
            {
                for(auto& rseen: rightSeen)
                {
                    return (seen==rseen);
                        
                }
                check = false;

            }
            return check;

        }
    public:
        bool boatSide;
        SIDE cannibal,missionary;
        SIDE boat;
        int cannibalCount;
        int missionaryCount;
        static int count;
        string etatPrint;
        string label;
        string labelChain;
        string leftS,rightS;
        vector<string> leftSeen;
        vector<string> rightSeen;

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
        void createNode(int cannibal, int missionary)
        {
            
            State * state = new State(data);
            Node< State* >* choice = new Node< State* >( state ,this->indice*100+10*cannibal+missionary);
            choice->data->label = "C"+to_string(cannibal) + "M"+to_string(missionary);
            choice->data->labelChain +=choice->data->label +"->";
            
            cout<<"sides\n";
            cout<<data->leftS<<"\n";
            cout<<data->rightS<<"\n";
            state->transport(cannibal,missionary);
            state->process();
            if(data->boatSide) // M side
            {
                cout<<"missionary\n";
                cout<<data->missionary.first <<"k"<< data->missionary.second<<"\n";
                cout<<cannibal <<"k"<< missionary<<"\n";

                if(data->missionary.first==cannibal && data->missionary.second == missionary)
                    return;
                
            }
            else
            {
                cout<<"cannibal\n";
                cout<<data->cannibal.first <<"k"<< data->cannibal.second<<"\n";
                cout<<cannibal <<"k"<< missionary<<"\n";
                if(data->cannibal.first==cannibal && data->cannibal.second == missionary)
                    return;
                
                
            }
            children.push_back(choice);
            choice->haveChild();

        }
        void makechilds(int& cannibalSize, int& missionarySize)
        {
            /**
             *   -00 
             *  11 CM == 11 MC
             *  01 M
             *  10 C
             *  20 CC
             *  02 MM
             * */
            
            for(int i = 0; i < 3; i++)
            {
                for(int j = 0; j < 3; j++)
                {
                    if( (i+j) > 0 && (i+j) <=2)
                    {
                        //cout<<i<<" "<<j<<"\n";
                        //cout<<cannibalSize<<" "<<missionarySize<<"\n";
                        if(cannibalSize>=i && missionarySize>=j)
                        {   
                            cout<<"creation\n";
                            cout<<cannibalSize<<" "<<missionarySize<<"\n";
                            cout<<i<<" "<<j<<"\n";
                            createNode(i,j);
                        }
                    }
                }    
            }
        }
        void haveChild()
        {

            if(data->boatSide) // M side
            {
                makechilds(data->missionary.first,data->missionary.second);


            }
            else // C side
            {
                makechilds(data->cannibal.first,data->cannibal.second);


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


    





}