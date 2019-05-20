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

typedef pair<int, int> iPair;

struct Grapher
{
    int V, E;
    vector< pair<int, iPair> > edges;
    Grapher(int V, int E, vector< pair<int, iPair> > ed){
        this->V = V;
        this->E = E;
        this->edges = ed;
    }
};

struct DisjointSets 
    { 
        int *parent, *rnk; 
        int n; 
  
        // Constructor. 
        DisjointSets(int n) 
        { 
            // Allocate memory 
            this->n = n; 
            parent = new int[n+1]; 
            rnk = new int[n+1]; 
            
            // Initially, all vertices are in 
            // different sets and have rank 0. 
            for (int i = 0; i <= n; i++) 
            { 
                rnk[i] = 0; 
                
                //every element is parent of itself 
                parent[i] = i; 
            } 
        } 
        
        // Find the parent of a node 'u' 
        // Path Compression 
        int find(int u) 
        { 
            /* Make the parent of the nodes in the path 
               from u--> parent[u] point to parent[u] */
            if (u != parent[u]) 
                parent[u] = find(parent[u]); 
            return parent[u]; 
        } 
        
        // Union by rank 
        void merge(int x, int y) 
        { 
            x = find(x), y = find(y); 
            
            /* Make tree with smaller height 
            a subtree of the other tree  */
            if (rnk[x] > rnk[y]) 
                parent[y] = x; 
            else // If rnk[x] <= rnk[y] 
                parent[x] = y; 
            
            if (rnk[x] == rnk[y]) 
                rnk[y]++; 
        } 
        

    };




int findIndex(vector<string> netlist, string u);
vector< pair<int, iPair> > consEdge(vector<Transistor> stack, vector<Net> netvector);
void printGraph(Grapher graph, int N);
int detectDuality(vector<Transistor> transistors);
vector< pair<int, int> > kruskalMST(vector< pair<int, iPair> > edges, int V);
#endif /* GRAPHER_H */
