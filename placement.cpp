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

using namespace std;

void place(string fileName){
    vector<Transistor> nSt;
    vector<Transistor> pSt;
    
    vector<Transistor> list_transistors(parser(fileName));
    
    nSt = nStack(list_transistors); 
    pSt = pStack(list_transistors);

    vector<string> netlist_n = indexNetlist(nSt);
    vector<string> netlist_p = indexNetlist(pSt);
    
    vector<Grapher> eulerGraphs;
    
    for (auto i = netlist_p.begin(); i != netlist_p.end(); ++i)
        std::cout << *i << ' ';
    std::cout << "\n";
    Grapher gp(consEdge(pSt, netlist_p), netlist_p.size());
    printGraph(gp, netlist_p.size());
    gp.test();
            
    for (auto i = netlist_n.begin(); i != netlist_n.end(); ++i)
        std::cout << *i << ' ';
    std::cout << "\n";
    Grapher gn(consEdge(nSt, netlist_n), netlist_n.size());
    printGraph(gn, netlist_n.size());
    gn.test();
        
    detectDuality(nSt, pSt);
    
    int maxTracks = 3*(std::max(nSt.size(), pSt.size())); // gets highest number of transistors between N and P stacks;
    
    int nFins=0, pFins=0;
    
    for(auto it = nSt.begin(); it != nSt.end(); it++){
        if(it->get_nfin() > nFins)
            nFins = it->get_nfin();
    }
    for(auto it = pSt.begin(); it != pSt.end(); it++){
        if(it->get_nfin() > pFins)
            pFins = it->get_nfin();
    }
    
    int maxFins = nFins + pFins + 4;
    int grade[maxFins][maxTracks];
    int estWidth = 54*maxTracks;
    int estHeight = 64 + 20*(maxFins-1) + 7*maxFins;
    int estArea = estWidth*estHeight;
    
    cout << maxFins << " " << maxTracks << "\n";
    cout << "Estimated maximum width: " << estWidth << "n\nEstimated height: " << estHeight << "n\nEstimated maximum area:" << estArea << "n²\n";
    
    
    
}