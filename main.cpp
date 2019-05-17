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
#include "grapher.h"
#include "stack.h"

int main(int argc, char *argv[])
{
        string fileName;
        
        if(argc > 1)
        { 
            fileName = argv[1];
        }
        list<Transistor> nSt;
        list<Transistor> pSt;

        
        list<Transistor> list_transistors(parser(fileName));
        
        nSt = nStack(list_transistors); 
        pSt = pStack(list_transistors);

        vector<string> netlist_n = indexNetlist(nSt);
        vector<string> netlist_p = indexNetlist(pSt);
        
        for (auto i = netlist_p.begin(); i != netlist_p.end(); ++i)
            std::cout << *i << ' ';

        //cout << findIndex(netlist, "A") << "\n";
        Grapher g(nSt, netlist_n);
    
        printGraph(g, netlist_n.size());
        
        return 0;
}