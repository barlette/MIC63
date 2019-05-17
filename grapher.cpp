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


Stack divide_into_stacks(list<Transistor> ts)
{
    Stack st;
    std::list<Transistor>::iterator it;
    for(it = ts.begin(); it != ts.end(); it++){
        if(it->get_type() == 'N') st.nStack.push_back(Transistor(it->get_drain(), it->get_gate(), it->get_source(), it->get_bgate(), it->get_type(), it->get_width(), it->get_length(), it->get_nfin()));
        else st.pStack.push_back(Transistor(it->get_drain(), it->get_gate(), it->get_source(), it->get_bgate(), it->get_type(), it->get_width(), it->get_length(), it->get_nfin()));
    }

    cout << "stack P: \n";
    print_list(st.pStack);
    cout << "stack N: \n";
    print_list(st.nStack);
    
    return st;
}

int findIndex(vector<string> adj, string u){
    
    auto it = find(adj.begin(), adj.end(), u);
    
    if(it != adj.end()){
        return distance(adj.begin(), it);
    } else return -1;
}

vector<int> addEdge(vector<int> adj, string u, string v, vector<string> netlist){
    //vector<int> temp = adj;
    adj[findIndex(netlist,u)].push_back(findIndex(netlist,v));
    adj[findIndex(netlist,v)].push_back(findIndex(netlist,u));
    return adj;
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

vector<int> grapher(list<Transistor> stack){
    indexNetlist(stack);
    vector<int> adj[stack.size()];
    
    vector<string> netlist = indexNetlist(stack);
    
    
    std::list<Transistor>::iterator it;
    for(it = stack.begin(); it != stack.end(); it++){
        adj = addEdge(adj, it->get_drain(), it->get_gate(), netlist);
        adj = addEdge(adj, it->get_source(), it->get_gate(), netlist);
    }
    
    return adj;
}


