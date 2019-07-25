/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

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
#include "grapher.h"

int findIndex(vector<string> netlist, string u){
    
    auto it = find(netlist.begin(), netlist.end(), u);
    
    if(it != netlist.end()){
        return distance(netlist.begin(), it);
    } else return -1;
}

vector< pair<int, iPair> > consEdge(vector<Transistor> stack, vector<Net> netvector){
        vector< pair<int, iPair> > ed;
        pair<int, iPair> temp;
        
        for(int it = 0; it < netvector.size(); it++){
            for(int it2 = it+1; it2 < netvector.size(); it2++){
                if((netvector[it].wTransistor == netvector[it2].wTransistor) &&  (netvector[it].type != netvector[it2].type)){
                    ed.push_back({0, {it, it2}});
                } else if((netvector[it].wTransistor != netvector[it2].wTransistor) && (netvector[it].name == netvector[it2].name)){
                    if(netvector[it].type != netvector[it2].type){
                       ed.push_back({2, {it, it2}}); 
                    } else ed.push_back({1, {it, it2}}); 
            }
        }
        }
        return ed;
}

vector< pair<int, int> > kruskalMST(vector< pair<int, iPair> > edges, int V) 
{ 
    int mst_wt = 0; // Initialize result 
    vector< pair<int, int> > lowcEdges;
    // Sort edges in increasing order on basis of cost 
    sort(edges.begin(), edges.end()); 
  
    // Create disjoint sets 
    DisjointSets ds(V); 
  
    // Iterate through all sorted edges 
    vector< pair<int, iPair> >::iterator it; 
    for (it=edges.begin(); it!=edges.end(); it++) 
    { 
        int u = it->second.first; 
        int v = it->second.second; 
  
        int set_u = ds.find(u); 
        int set_v = ds.find(v); 
  
        // Check if the selected edge is creating 
        // a cycle or not (Cycle is created if u 
        // and v belong to same set) 
        if (set_u != set_v) 
        { 
            // Current edge will be in the MST 
            // so print it 
            cout << u << " - " << v << endl; 
  
            // Update MST weight 
            mst_wt += it->first; 
  
            // Merge two sets 
            ds.merge(set_u, set_v); 
            lowcEdges.push_back({u, v});
        } 
    } 
  
    return lowcEdges; 
} 