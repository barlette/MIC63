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

int count_vertex_degree(vector < vector<int> > adj, int n, int vert){
    int sum = 0;
    for(int column = 0; column < n; column++){
        sum = sum+adj[vert][column];
    }
    return sum-2;
}

int find_leaf(vector < vector<int> > adj, int n, vector <int> visited){
    //cout << "teste";
    for(int vert = 0; vert < n; vert++){
        if((count_vertex_degree(adj, n, vert) == 0) && (visited[vert] == 0)){
            //cout << vert;
            return vert;
        }
    } 
    return -1;
}

int find_connected_vertice(vector < vector<int> > adj, int n, vector <int> visited){
    for(int row = 0; row < adj.size(); row++){
            if((adj[row][n] == 1) && (row != n) && (visited[row] == 0))
                return row;
    }
    return -1;
}

int find_connected_vertice_toleaf(vector < vector<int> > adj, int n, vector <int> visited){
    for(int column = 0; column < adj.size(); column++){
            if((adj[n][column] == 1) && (column != n) && (visited[column] == 0))
                return column;
    }
    return -1;
}

int find_nonvis_node(vector < vector<int> > adj, int n, vector <int> visited){
    for(int column = 0; column < adj.size(); column++){
        if((adj[n][column] == 1) && (column != n) && (visited[column] == 0))
            return column;
    }
    return -1;
}

void place(string fileName){
    vector<Transistor> nSt;
    vector<Transistor> pSt;
    
    vector<Transistor> list_transistors(parser(fileName));
    
    nSt = nStack(list_transistors); 
    pSt = pStack(list_transistors);

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
    int totalTracks = (2*maxTracks)-1;

    
    int estWidth = 54*maxTracks;
    int estHeight = 64 + 20*(maxFins-1) + 7*maxFins;
    int estArea = estWidth*estHeight;
    
    //cout << maxFins << " " << maxTracks << "\n";
    cout << "Estimated maximum width: " << estWidth << "n\nEstimated height: " << estHeight << "n\nEstimated maximum area:" << estArea << "n²\n";
    
    
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
    
    vector <int> v(netlist_p.size(), 0);
    vector < vector<int> > adjMatrix(netlist_p.size(), v);
    
//    for(int i=0; i < netlist_p.size(); i++){
//        adjMatrix.push_back(v);
//    }
    
    //monta matriz de adjacencia
    for(int row = 0; row < netlist_p.size(); row++){
        for(int column = 0; column < netlist_p.size(); column++){
            if(row == column)
                adjMatrix[row][column]=1;
        }
    }
    
    for(int it = 0; it < mst_wt.size(); it++){
        //cout << "first: " << mst_wt[it].first << "second: " << mst_wt[it].second << "\n";
        adjMatrix[mst_wt[it].first][mst_wt[it].second] = 1;
        adjMatrix[mst_wt[it].second][mst_wt[it].first] = 1;
    }
    
    
    // imprime matriz adjacencia
    for(int row = 0; row < netlist_p.size(); row++){
        for(int column = 0; column < netlist_p.size(); column++){
            cout << adjMatrix[row][column] << " ";
            }
        cout << "\n";
        }
    
    vector <int> pos(netlist_p.size(), -1);
    vector <int> visited(netlist_p.size());

    
    int it=0;
    
    while(it < netlist_p.size()){
    int vert = find_leaf(adjMatrix, netlist_p.size(), visited);
    if(vert != -1){
        pos[it] = vert;
        cout << vert;
        visited[pos[it]] = 1;            
    } else cout << "Não achou vértice folha restante.\n";
    vert = 0;
    it++;
    while(vert != -1){
        
        vert =  find_connected_vertice(adjMatrix, pos[it-1], visited);
        if(vert != -1){
            pos[it] = vert;
            cout << pos[it];
            it++;
            visited[vert] =1;
        }
    }
    //cout << pos[it-1];
    vert=0;
    if(count_vertex_degree(adjMatrix, netlist_p.size(), pos[it-1]) > 1){
        vert = find_nonvis_node(adjMatrix, pos[it-1], visited);
        pos[it] = vert;
        cout << vert;
        it++;
        visited[vert]=1;
    }
    
    while(vert != -1){    
        //cout << pos[it-1];
        vert =  find_connected_vertice_toleaf(adjMatrix, pos[it-1], visited);
        if(vert != -1){
            pos[it] = vert;
            cout << pos[it];
            it++;
            visited[vert] =1;
        }
    }
    }
    
    string grade[maxFins][(2*maxTracks)-1];
    cout << "\n";
    string lastName = " ";
    int trackIndex = 0;
    for(int it = 0; it < pos.size(); it++){
        //cout << it;
        //cout << lastName << netlist_p[it].name;
        if(lastName != netlist_p[pos[it]].name){
            for(int row=0; row<maxFins; row++){
                grade[row][trackIndex] = netlist_p[pos[it]].name;    
            }
            trackIndex++;
            lastName = netlist_p[pos[it]].name;        
        }
    }

    for(int row = 0; row < maxFins; row++){
        for(int column = 0; column < trackIndex; column++){
            cout << grade[row][column] << " ";
        }
        cout << "\n";
    }

}
    
    
    
    



