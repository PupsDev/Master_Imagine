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
    Node<int>* root = new Node<int>(7,0);

    Node<int>* child = new Node<int>(7,1);
    Node<int>* child1 = new Node<int>(7,2);
    Node<int>* child2 = new Node<int>(7,3);

    
    Node<int>* child01 = new Node<int>(7,4);
    Node<int>* child02 = new Node<int>(7,5);
    Node<int>* child12 = new Node<int>(7,6);

    /*
    child->children.push_back(child01);
    child->children.push_back(child02);

    child1->children.push_back(child12);

    root->children.push_back(child);
    root->children.push_back(child1);
    root->children.push_back(child2);
    */

    root->children.push_back(child);
    root->children.push_back(child2);

    child->children.push_back(root);
    child->children.push_back(child1);

    child1->children.push_back(child);
    child1->children.push_back(child2);

    child2->children.push_back(root);
    child2->children.push_back(child1);

    std::cout << "DFS" << std::endl;
    DFS_recursive(root);
    root->resetDiscovered();
    std::cout << "BFS" << std::endl;
    BFS_iterative(root);
}