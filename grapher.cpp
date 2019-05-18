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

vector<Edge> consEdge(list<Transistor> stack, vector<string> netlist){
        std::list<Transistor>::iterator it;
        vector<Edge> ed;
        Edge temp;
        for(it = stack.begin(); it != stack.end(); it++){
            temp.dest =  findIndex(netlist, it->get_drain());
            temp.src  =  findIndex(netlist, it->get_gate());
            ed.push_back(temp);
            
            temp.dest = findIndex(netlist, it->get_gate());
            temp.src = findIndex(netlist, it->get_drain());
            ed.push_back(temp);
            
            temp.dest = findIndex(netlist, it->get_source());
            temp.src = findIndex(netlist, it->get_gate());
            ed.push_back(temp);
            
            temp.dest = findIndex(netlist, it->get_gate());
            temp.src = findIndex(netlist, it->get_source());
            ed.push_back(temp);
        }
        return ed;
}

void printGraph(Grapher graph, int N){

    for (int i = 0; i < N; i++)
    {
	// print current vertex number
	cout << i << " --> ";

	// print all neighboring vertices of vertex i
        for (int v : graph.adj[i])
            cout << v << " ";
        cout << endl;
    }
}