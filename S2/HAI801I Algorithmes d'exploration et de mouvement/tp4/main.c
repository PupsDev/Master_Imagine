
#include <bits/stdc++.h>
#include <vector>
#include <iostream>
using namespace std;

typedef struct vertex
{
    int indice;
    int label;
    int parent;
} Vertex;

vector<vector<int>> adj;
vector<Vertex> vertices;


void addEdge(int x,int y)
{
	adj[x][y] = 1;
	adj[y][x] = 1;
}


void bfs(Vertex s, Vertex t)
{
    
	int found =0;
    std::queue<Vertex> myqueue;

    myqueue.emplace(s);
    while(!myqueue.empty() && !found)
    {
        Vertex v = myqueue.front();
        myqueue.pop();
        for(int i = 0 ; i < adj.size();i++)
        {
            if(adj[v.indice][i])
            {
                
                if(vertices[i].label < 0)
                {
                    vertices[i].label = v.label+1;
                    vertices[i].parent = v.indice;
                    myqueue.emplace(vertices[i]);
                    if(vertices[i].indice == t.indice)
                    {
                        found = 1;
                         cout<<"FOUND"<<endl;
                        
                    }

                }
            }
        }
        //break;
    }

}


int main()
{

    
/*
# Directed graph (each unordered pair of nodes is saved once): Wiki-Vote.txt 
# Wikipedia voting on promotion to administratorship (till January 2008). Directed edge A->B means user A voted on B becoming Wikipedia administrator.
# Nodes: 7115 Edges: 103689
# FromNodeId	ToNodeId*/

    int v = 10000;

    adj= vector<vector<int>>(v,vector<int>(v,0));
    Vertex vert[v];
    for(int i = 0 ; i < v ; i++)
    {
        
        vert[i].indice = i;
        vert[i].label = -1;
        vert[i].parent = 0;
        vertices.push_back(vert[i]);
    }
    Vertex s;
    s.indice =0;
    s.label =-1;
    s.parent = 0;
    
    Vertex t;
    t.indice = 5;
    t.label =-1;
    t.parent = 0;


    /*
    addEdge(0,1);
    addEdge(1,2);
    addEdge(2,3);
    addEdge(3,4);
    */
    ifstream cin("Wiki-Vote.txt");
    for(int i = 0 ; i< 103689;i++)
    {
        int e1,e2;
        cin>>e1>>e2;
        addEdge(e1,e2);

    }

/*
    for(int i = 0 ; i < v ; i++)
    {
        for(int j = 0 ; j < v ; j++)
        {
            cout<<adj[i][j];
        }
        
    }
    
*/
    bfs(vertices[8243],vertices[6737]);
/*
        for(int i = 0 ; i < v ; i++)
    {
        //cout<<vertices[i].indice<<endl;
        cout<<vertices[i].parent<<endl;
    }
    */
}
