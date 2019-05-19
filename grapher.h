/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   grapher.h
 * Author: brltt
 *
 * Created on May 16, 2019, 8:56 PM
 */

#ifndef GRAPHER_H
#define GRAPHER_H

#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <list>
#include <iterator>
//#include "parser.h"
#include "circuit.h"
//#include "stack.h"

//vector<int> adj;
using namespace std;

struct Edge {
    int src, dest;
};

class Grapher
{
public:
    int V;
    
    vector<vector<int>> adj;
    Grapher(vector<Edge> ed, int N){
        adj.resize(N);
        V = N;
        for(auto &edge: ed){
            adj[edge.src].push_back(edge.dest);
        }
    }
    
    void DFSUtil(int v, bool visited[]) 
    { 
        //Mark the current node as visited and print it 
        visited[v] = true; 
  
        // Recur for all the vertices adjacent to this vertex 
        vector<int>::iterator i; 
        for (i = adj[v].begin(); i != adj[v].end(); ++i) 
            if (!visited[*i]) 
                DFSUtil(*i, visited); 
    } 
    
    bool isConnected()
    {
        bool visited[V];
        int i;
        for (i = 0; i < V; i++)
        {
            visited[i] = false;
        }
        for (i = 0; i < V; i++)
        {
            if (adj[i].size() != 0)
            {
                break;
            }
        }
        if (i == V)
        {
            return true;
        }
        DFSUtil(i, visited);
        for (i = 0; i < V; i++)
        {
            if (visited[i] == false && adj[i].size() > 0)
            {
                return false;
            }
        }
        return true;
    }
    
    /* The function returns one of the following values 
   0 --> If grpah is not Eulerian 
   1 --> If graph has an Euler path (Semi-Eulerian) 
   2 --> If graph has an Euler Circuit (Eulerian)  */
    int isEulerian() 
    { 
    // Check if all non-zero degree vertices are connected 
        //if (isConnected() == false) 
        //    return 0; 
  
    // Count vertices with odd degree 
        int odd = 0; 
        for (int i = 0; i < V; i++) 
            if (adj[i].size() & 1) 
              odd++; 
  
    // If count is more than 2, then graph is not Eulerian 
        if (odd > 2) 
            return 0; 
  
    // If odd count is 2, then semi-eulerian. 
    // If odd count is 0, then eulerian 
    // Note that odd count can never be 1 for undirected graph 
        return (odd)? 1 : 2; 
    } 

    int test() 
    { 
    int res = isEulerian(); 
    if (res == 0){
        cout << "graph is not Eulerian\n"; 
        return 0;
    }
    else if (res == 1) {
        cout << "graph has a Euler path\n"; 
        return 1;
    }
    else{
        cout << "graph has a Euler cycle\n"; 
        return 2;
    }  
    }
};

int findIndex(vector<string> netlist, string u);
vector<Edge> consEdge(vector<Transistor> stack, vector<string> netvector);
void printGraph(Grapher graph, int N);
int detectDuality(vector<Transistor> transistors);
#endif /* GRAPHER_H */
