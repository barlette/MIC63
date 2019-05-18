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
    vector<vector<int>> adj;
    Grapher(vector<Edge> ed, int N){
        adj.resize(N);
        
        for(auto &edge: ed){
            adj[edge.src].push_back(edge.dest);
        }
    }
    
    //void DFSUtil(int v, bool visited[]) 
    //{ 
    // Mark the current node as visited and print it 
    //visited[v] = true; 
  
    // Recur for all the vertices adjacent to this vertex 
    //vector<int>::iterator i; 
    //for (i = adj[v].begin(); i != adj[v].end(); ++i) 
    //    if (!visited[*i]) 
    //        DFSUtil(*i, visited); 
    //} 
    
};

int findIndex(vector<string> netlist, string u);
vector<Edge> consEdge(list<Transistor> stack, vector<string> netlist);
void printGraph(Grapher graph, int N);
#endif /* GRAPHER_H */
