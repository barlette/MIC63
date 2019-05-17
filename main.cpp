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
        
        list<Transistor> list_transistors(parser(fileName));
        Stack st(divide_into_stacks(list_transistors));
        //vector<string> netlist = indexNetlist(st.nStack);
        
        for (auto i = netlist.begin(); i != netlist.end(); ++i)
            std::cout << *i << ' ';

        //cout << findIndex(netlist, "A") << "\n";
        
        vector<int> graph = 
}