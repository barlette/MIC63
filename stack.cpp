/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <fstream>
#include <iomanip>
#include <iostream>
#include "parser.h"
#include "circuit.h"
#include <vector>
#include <string>
#include <regex>
#include <list>
#include <iterator>
#include "stack.h"
#include "grapher.h"

list<Transistor> nStack(list<Transistor> ts)
{
    list<Transistor> n;
    std::list<Transistor>::iterator it;
    for(it = ts.begin(); it != ts.end(); it++){
        if(it->get_type() == 'N') n.push_back(Transistor(it->get_drain(), it->get_gate(), it->get_source(), it->get_bgate(), it->get_type(), it->get_width(), it->get_length(), it->get_nfin()));
    }
    
    cout << "stack N: \n";
    print_list(n);
    
    return n;
}

list<Transistor> pStack(list<Transistor> ts)
{
    list<Transistor> p;
    std::list<Transistor>::iterator it;
    for(it = ts.begin(); it != ts.end(); it++){
        if(it->get_type() == 'P') p.push_back(Transistor(it->get_drain(), it->get_gate(), it->get_source(), it->get_bgate(), it->get_type(), it->get_width(), it->get_length(), it->get_nfin()));
    }
    
    cout << "stack N: \n";
    print_list(p);
    
    return p;
}

int findIndex(vector<string> netlist, string u){
    
    auto it = find(netlist.begin(), netlist.end(), u);
    
    if(it != netlist.end()){
        return distance(netlist.begin(), it);
    } else return -1;
}

vector<string> indexNetlist (list<Transistor> ts){
    vector<string> net_index;
    std::list<Transistor>::iterator it;
    for(it = ts.begin(); it != ts.end(); it++){
        if(find(net_index.begin(), net_index.end(), it->get_drain()) == net_index.end()){
            net_index.push_back(it->get_drain());
        }
        
        if(find(net_index.begin(), net_index.end(), it->get_gate()) == net_index.end()){
            net_index.push_back(it->get_gate());
        }
        
        if(find(net_index.begin(), net_index.end(), it->get_source()) == net_index.end()){
            net_index.push_back(it->get_source());
        }
    }
    return net_index;
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