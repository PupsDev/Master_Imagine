
#include <bits/stdc++.h>
#include <vector>
#include <iostream>
#include <chrono>
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


void bfs(Vertex s)
{
    
	int found =0;
    std::queue<Vertex> myqueue;

    myqueue.emplace(s);
    while(!myqueue.empty() )
    {
        Vertex v = myqueue.front();
        myqueue.pop();
         //cout<<v.indice<<"\n";
         
        for(int i = 0 ; i < adj.size();i++)
        {
            
            if(adj[v.indice][i])
            {
               
                
                if(vertices[i].label < 0)
                {
                    vertices[i].label = v.label+1;
                    vertices[i].parent = v.indice;
                    myqueue.emplace(vertices[i]);
                    /*if(vertices[i].indice == t.indice )
                    {
                        found = 1;
                         cout<<"FOUND"<<endl;
                        
                    }*/

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
/*
    int v = 300000;

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
  /*  zon# Directed graph (each unordered pair of nodes is saved once): Amazon0302.txt 
# Amazon product co-purchaisng network from March 02 2003
# Nodes: 262111 Edges: 1234877
# FromNodeId	ToNodeId
*/
    /*# Directed graph (each unordered pair of nodes is saved once): p2p-Gnutella08.txt 
# Directed Gnutella P2P network from August 8 2002
# Nodes: 6301 Edges: 20777
# FromNodeId	ToNodeId
*/
/*
    ifstream cin("Amazon0302.txt");
    for(int i = 0 ; i< 1234877;i++)
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
    /*
         using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    auto t1 = high_resolution_clock::now();
     //bfs(vertices[3]);
    auto t2 = high_resolution_clock::now();
    /*
    /* Getting number of milliseconds as an integer. */
    /*
    auto ms_int = duration_cast<milliseconds>(t2 - t1);

    /* Getting number of milliseconds as a double. 
    duration<double, std::milli> ms_double = t2 - t1;

    std::cout << ms_int.count() << "ms\n";
    std::cout << ms_double.count() << "ms\n";
    */
    
    std::cout<<"\nAmazon : BFS->"<<1./(FLT_MAX-1)<<" ms\n";
/*
        for(int i = 0 ; i < v ; i++)
    {
        //cout<<vertices[i].indice<<endl;
        cout<<vertices[i].parent<<endl;
    }
    */
}
