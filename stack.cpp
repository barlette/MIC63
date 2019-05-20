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

vector<Transistor> nStack(vector<Transistor> ts)
{
    vector<Transistor> n;

    for(auto it = ts.begin(); it != ts.end(); it++){
        if(it->get_type() == 'N') n.push_back(Transistor(it->get_drain(), it->get_gate(), it->get_source(), it->get_bgate(), it->get_type(), it->get_width(), it->get_length(), it->get_nfin()));
    }
    
    cout << "stack N: \n";
    print_list(n);
    
    return n;
}

vector<Transistor> pStack(vector<Transistor> ts)
{
    vector<Transistor> p;

    for(auto it = ts.begin(); it != ts.end(); it++){
        if(it->get_type() == 'P') p.push_back(Transistor(it->get_drain(), it->get_gate(), it->get_source(), it->get_bgate(), it->get_type(), it->get_width(), it->get_length(), it->get_nfin()));
    }
    
    cout << "stack P: \n";
    print_list(p);
    
    return p;
}

vector<Net> indexNetlist (vector<Transistor> ts){
    vector<Net> net_index;
    
    Net temp;
 
    for(auto it = ts.begin(); it != ts.end(); it++){
        temp.name = it->get_drain();
        temp.type = ACTIVE;
        temp.wTransistor = it - ts.begin();
        net_index.push_back(temp);

        temp.name = it->get_gate();
        temp.type = GATE;
        temp.wTransistor = it - ts.begin();
        net_index.push_back(temp);
        
        temp.name = it->get_source();
        temp.type = ACTIVE;
        temp.wTransistor = it - ts.begin();
        net_index.push_back(temp);
    }
    
    return net_index;
}



int detectDuality(vector<Transistor> ns, vector<Transistor> ps){
    vector<bool> pv[ps.size()], nv[ns.size()];
    vector<string> ngates, pgates;
    int dual = 0;
    
    // if dual = 1 there are the number of p transistors and n transistors are the same;
    if(ps.size() == ns.size()){
        cout << "Same number of P and N transistors \n";
        dual = 1;
    }
    for(auto it = ns.begin(); it != ns.end(); it++){
        ngates.push_back(it->get_gate());
    }
    for(auto it = ps.begin(); it != ps.end(); it++){
        pgates.push_back(it->get_gate());
    }
    
    if(pgates == ngates){
        cout << "Same gate signals for P and N stacks\n";
        dual = 2;
    }
    return dual;
}

