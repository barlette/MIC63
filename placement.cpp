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

int find_index_pair(int u, vector< pair<int, int> > edges, int whichone){
    for(int i; i < edges.size(); i++){
        if(whichone == 1){
            if(edges[i].first == u){
                return i;
            }
        } else if(whichone ==2) {
            if(edges[i].second == u){
                return i;            
            }
        }
    }
    return 0;
}

int find_nonvis_pair(int u, vector< pair<int, int> > edges, vector <int> vis, int whichone){
    for(int i; i < edges.size(); i++){
        if(whichone == 1){
            if((edges[i].first == u) && (vis[i] != 1)){
                return i;
            }
        } else if(whichone ==2) {
            if((edges[i].second == u) && (vis[i] != 1)){
                return i;            
            }
        }
    }
    return -1;
}

void place(string fileName){
    vector<Transistor> nSt;
    vector<Transistor> pSt;
    
    vector<Transistor> list_transistors(parser(fileName));
    
    nSt = nStack(list_transistors); 
    pSt = pStack(list_transistors);

    
    
    vector<Net> netlist_n = indexNetlist(nSt);
    vector<Net> netlist_p = indexNetlist(pSt);
//    
//    vector<Grapher> eulerGraphs;
//    
    int count =0;
    for (auto it = netlist_p.begin(); it != netlist_p.end(); it++){
        std::cout << count << ": " << it->name << " " << it->type << " " << it->wTransistor << "\n";
        count++;
    }
    
    vector< pair<int, iPair> > edges = consEdge(pSt, netlist_p);
    Grapher gp(netlist_p.size(), edges.size(), edges);
    
    //for (auto it = edges.begin(); it != edges.end(); it++){
    //    std::cout << it->first << it->second.first << it->second.second << "\n";
    //}
    
    cout << "Edges of MST are \n";
    vector< pair <int, int> > mst_wt = kruskalMST(gp.edges, gp.V);
    
    for(int it = 0; it < mst_wt.size(); it++)
        cout << mst_wt[it].first << " - " << mst_wt[it].second << "\n";
    

        
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
    string grade[maxFins][(2*maxTracks)-1];
    int totalTracks = (2*maxTracks)-1;
    //vector< vector<int> > grade;
    
    int estWidth = 54*maxTracks;
    int estHeight = 64 + 20*(maxFins-1) + 7*maxFins;
    int estArea = estWidth*estHeight;
    
    //cout << maxFins << " " << maxTracks << "\n";
    cout << "Estimated maximum width: " << estWidth << "n\nEstimated height: " << estHeight << "n\nEstimated maximum area:" << estArea << "n²\n";
    
    string netName;
    string lastName = "inv";
    
    int n_index = 0, last_index=0;
    int edges_count =0;
    vector <int> visited(mst_wt.size(), 0);
    
    for(int column = 2; column < totalTracks-2; column++){

        if(edges_count < mst_wt.size()){
            //cout << n_index;
            netName = netlist_p[mst_wt[n_index].first].name; 
            //cout << netName;
            visited[n_index] = 1;
            last_index = n_index;
            n_index = find_nonvis_pair(mst_wt[n_index].second, mst_wt, visited, 1);
            //cout << n_index;    
        if(lastName != netName){
            for(int row = 0; row < maxFins; row++){
                grade[row][column] = netName;
            } 
        } else column--;

        if(n_index == -1){
            netName = netlist_p[mst_wt[last_index].second].name; 
            if(lastName != netName){
                column++;
                for(int row = 0; row < maxFins; row++){
                    grade[row][column] = netName;
            } 
            } else column--;
        }        
        lastName = netName;
        //cout << "Edge count: " << edges_count << "\n";
        } else if (edges_count == mst_wt.size()){
            //cout << mst_wt[last_index].second;
            //cout << netlist_p[mst_wt[last_index].second].name;
            netName = netlist_p[mst_wt[last_index].second].name;
            if(lastName != netName){
                for(int row = 0; row < maxFins; row++){
                    grade[row][column] = netName;
                } 
            } else column--;              
        }
        edges_count++;
    }
    
    
    
    
    
    
    for(int row = 0; row < maxFins; row++){
        for(int column = 2; column < totalTracks-2; column++){
            cout << grade[row][column] << " ";
        }
        cout << "\n";
    }
}

