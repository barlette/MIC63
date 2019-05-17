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
#include "parser.h"
#include "circuit.h"
#include "stack.h"
#include <vector>
#include <string>
#include <regex>
#include <list>
#include <iterator>

//vector<int> adj;
using namespace std;

struct Edge {
    int src, dest;
};

class Grapher
{
public:
    vector<vector<int>> adj;
    Grapher(list<Transistor> stack, vector<string> netlist){
        adj.resize(netlist.size());
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
        
        for(auto &edge: ed){
            adj[edge.src].push_back(edge.dest);
        }
    }
};
#endif /* GRAPHER_H */
