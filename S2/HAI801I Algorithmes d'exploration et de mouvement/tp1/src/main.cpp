#include <iostream>
#include <vector>
#include <queue>

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
int main() {
    /*
    Node<char>* A = new Node<char>('A',0);

    Node<char>* B = new Node<char>('B',1);
    Node<char>* C = new Node<char>('C',2);

    Node<char>* D = new Node<char>('D',3);
    Node<char>* E = new Node<char>('E',4);


    A->children.push_back(B);
    A->children.push_back(E);

    B->children.push_back(D);

    C->children.push_back(D);
    C->children.push_back(E);

    D->children.push_back(A);
    D->children.push_back(C);

    E->children.push_back(C);
    */
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

    std::cout << "DFS" << std::endl;
    DFS_recursive(A);
    A->resetDiscovered();
    std::cout << "BFS" << std::endl;
    BFS_iterative(A);
}