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
#include <unistd.h>
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

int find_samename_net(vector <int> pos, vector <Net> netlist, int net){
    for(int it = 0; it < pos.size(); it++){
        if(netlist[pos[it]].name == netlist[net].name){
            //cout << netlist[net].name;
            return 1;
        }
    }
    return 0;
}

void construct_net_matrix(vector<Transistor> trans){
    vector <string> inputs, pins_and_nets;

    for(int it=0; it<trans.size(); it++){
        inputs.push_back(trans[it].get_gate());
        if(find(pins_and_nets.begin(), pins_and_nets.end(), trans[it].get_drain()) == pins_and_nets.end()){
            pins_and_nets.push_back(trans[it].get_drain());
        }
        if(find(pins_and_nets.begin(), pins_and_nets.end(), trans[it].get_source()) == pins_and_nets.end()){
            pins_and_nets.push_back(trans[it].get_source());
        }
    }
    
    vector <int> v(pins_and_nets.size(), 0);
    vector < vector<int> > net_matrix(inputs.size(), v);
    
    for(int it=0; it<trans.size(); it++){
        int input_index =   distance(inputs.begin(), find(inputs.begin(), inputs.end(), trans[it].get_gate()));
        int drain_index =   distance(pins_and_nets.begin(), find(pins_and_nets.begin(), pins_and_nets.end(), trans[it].get_drain()));
        int source_index =  distance(pins_and_nets.begin(), find(pins_and_nets.begin(), pins_and_nets.end(), trans[it].get_source()));
        //cout << input_index << "\n";
        //cout << drain_index << "\n";
        //cout << source_index << "\n";
        //std::distance(input_index, inputs.size());
        net_matrix[input_index][drain_index] = 1;
        net_matrix[input_index][source_index] = 1;
    }
    
    vector <int> visited(inputs.size(), 0);
    int parallel_count=1;
    
    for(int row1=0; row1 < net_matrix.size(); row1++){
        visited[row1]=1;
        
        for(int row2=0; row2 < net_matrix.size(); row2++){
            if((visited[row2] == 0) && (net_matrix[row1] == net_matrix[row2])){
                visited[row2]=1;
                cout << inputs[row1];
                cout << inputs[row2];
                parallel_count++;
            }
        }
    }
    cout << "Elementos em paralelo: " << parallel_count << "\n";
    
    for(int row=0; row < net_matrix.size(); row++){
        for(int column=0; column < net_matrix[row].size(); column++){
            cout << net_matrix[row][column] << " ";
        }
        cout << "\n";
    }
    
    for(int it=0; it<inputs.size(); it++)
        cout << inputs[it] << " ";
    cout << "\n";
    for(int it=0; it<pins_and_nets.size(); it++)
        cout << pins_and_nets[it] << " ";
    cout << "\n";
    
}

int print_layout(vector<int> pos, vector<Net> netlist_p, int maxFins, int maxTracks){
    string grade[maxFins][(2*maxTracks)-1];
    cout << "\n";
    Net lastName;
    int trackIndex = 0;
    for(int it = 0; it < pos.size(); it++){
        //cout << it;
        //cout << lastName << netlist_p[it].name;
        if((lastName.name != netlist_p[pos[it]].name) && (netlist_p[pos[it]].type == lastName.type)){
            
            for(int row=0; row<maxFins; row++){
                grade[row][trackIndex] = "GATE";
                grade[row][trackIndex+1] = "BREAK";
                grade[row][trackIndex+2] = "GATE";
                grade[row][trackIndex+3] = netlist_p[pos[it]].name;    
            }
            trackIndex = trackIndex+4;
            lastName = netlist_p[pos[it]];        
        } else if ((lastName.name != netlist_p[pos[it]].name)){
                for(int row=0; row<maxFins; row++){
                    grade[row][trackIndex] = netlist_p[pos[it]].name;    
                }
                trackIndex++;
                lastName = netlist_p[pos[it]]; 
        }
    }

    for(int row = 0; row < maxFins; row++){
        for(int column = 0; column < trackIndex; column++){
            cout << grade[row][column] << " ";
        }
        cout << "\n";
    }
    return trackIndex;
}

vector<int> fstackPlacement(Grapher gp, vector<Net> netlist_p, int maxTracks, int maxFins){
        //cout << "Edges of MST are \n";
    vector< pair <int, int> > mst_wt = kruskalMST(gp.edges, gp.V);
    
    //for(int it = 0; it < mst_wt.size(); it++)
    //    cout << mst_wt[it].first << " - " << mst_wt[it].second << "\n";
    
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
//    for(int row = 0; row < netlist_p.size(); row++){
//        for(int column = 0; column < netlist_p.size(); column++){
//            cout << adjMatrix[row][column] << " ";
//            }
//        cout << "\n";
//        }
    
    vector <int> pos(netlist_p.size(), -1);
    vector <int> visited(netlist_p.size());
    vector <int> queue;
    
    int it=0, initial_it=0;
    
    while(it < netlist_p.size()){   
        initial_it = it;
        //cout << "initial: " << initial_it << "**\n";
        int vert = find_leaf(adjMatrix, netlist_p.size(), visited);
        if(vert != -1){
            queue.push_back(vert);
            pos[it] = vert;
            //cout << vert;
            visited[pos[it]] = 1;            
        } else cout << "Não achou vértice folha restante.\n";
        vert = 0;
        it++;
        while(vert != -1){
            
            vert =  find_connected_vertice(adjMatrix, pos[it-1], visited);
            if(vert != -1){
                queue.push_back(vert);
                pos[it] = vert;
                //cout << pos[it];
                it++;
                visited[vert] =1;
            }
        }
        
        if(count_vertex_degree(adjMatrix, netlist_p.size(), pos[it-1]) % 2 != 0){
            //cout << "node with odd degree\n";
            //initial_it;
            //cout << netlist_p[pos[initial_it]].name <<  "==" << netlist_p[queue[2]].name ;
            //cout << netlist_p[pos[initial_it]].name << "!=" <<  netlist_p[queue[0]].name;
            if((netlist_p[pos[initial_it]].name == netlist_p[queue[2]].name))
            {
                //cout << "teste1";
                //it = initial_it;
                for(int rev = queue.size()-1; rev >=0; rev--){
                    pos[it] = queue[rev];
                    it++;
                }
            } 
            else if(find_samename_net(pos, netlist_p, queue[2]) && (netlist_p[pos[initial_it-1]].name != netlist_p[queue[0]].name)){
                //cout << "teste2";
                it = initial_it;
                for(int rev = queue.size()-1; rev >=0; rev--){
                    pos[it] = queue[rev];
                    it++;
                }                
            }
        }
        queue.erase(queue.begin(), queue.end());
        //cout << pos[it-1];
        vert=0;
        if(count_vertex_degree(adjMatrix, netlist_p.size(), pos[it-1]) > 1){
            vert = find_nonvis_node(adjMatrix, pos[it-1], visited);
            pos[it] = vert;
            //cout << vert;
            it++;
            visited[vert]=1;
        }
        
        while(vert != -1){    
            //cout << pos[it-1];
            vert =  find_connected_vertice_toleaf(adjMatrix, pos[it-1], visited);
            if(vert != -1){
                pos[it] = vert;
                //cout << pos[it];
                it++;
                visited[vert] =1;
            }
        }
    }
    return pos;
}

vector <int> sstackPlacement(vector <int> fpos, vector<Net> fnetlist, vector <Net> snetlist, int maxFins, int maxTracks){
    Net last_net;
    vector <int> visited(snetlist.size()), spos(snetlist.size());
    int pos_index=1;
    int selected_gate;
    

    
    for(int it=1; it<fpos.size(); it++){
        if(fnetlist[fpos[it]].type == GATE){
            for(int it2=1; it2<snetlist.size(); it2++){
                if((snetlist[it2].name == fnetlist[fpos[it]].name) && (snetlist[it2].type == GATE) && (visited[it2] != 1)){
//                    cout << snetlist[it2-1].name;
//                    cout << snetlist[it2].name;
//                    cout << snetlist[it2+1].name;
                    visited[it2+1] = 1;
                    visited[it2] = 1;
                    visited[it2-1] = 1;
                    selected_gate = it2;
                    spos[pos_index-1] = selected_gate-1;
                    spos[pos_index] = selected_gate;
                    spos[pos_index+1] = selected_gate+1;
                    pos_index = pos_index+3;
                }
            }
        }
    }
//    cout << "\n";
//    for(int it=0; it<spos.size(); it++){
//        cout << snetlist[spos[it]].name << " ";        
//    }
    
    int index=0;
    if((snetlist[spos[index]].name == snetlist[spos[index+3]].name) && (snetlist[spos[index]].type == ACTIVE) && (snetlist[spos[index+3]].type == ACTIVE)){
            spos[index] = spos[index+2];
            spos[index+2] = spos[index+3];
        }
    
    for(int it=5; it<spos.size()-3; it++){
        if((snetlist[spos[it]].name == snetlist[spos[it+3]].name) && (snetlist[spos[it]].type == ACTIVE) && (snetlist[spos[it+3]].type == ACTIVE)){
            spos[it+3] = spos[it+1]; 
            spos[it+1] = spos[it];
            //it = it+3;
        }
    }
    cout << "\n";
    for(int it=0; it<spos.size(); it++){
        cout << snetlist[spos[it]].name << " ";        
    }
    return spos;
} 

int max(int i, int v){
    if(i < v){
        return v;
    } else return i;
}

vector <int> optimizing_spos(vector <int> pos, vector<Net> netlist){
    for(int it=0; it<pos.size()-3;it++){
        if((netlist[pos[it]].name != "VDD") && (netlist[pos[it]].name != "GND") && (netlist[pos[it]].type != GATE)){
            if(netlist[pos[it]].name != netlist[pos[it+1]].name){
                for(int it2=it+3; it2<pos.size();it2++){
                    if((netlist[pos[it]].name == netlist[pos[it2]].name) && (netlist[pos[it2-1]].type == GATE) && (netlist[pos[it2-2]].name != netlist[pos[it2-3]].name)){
                        int temp = pos[it2-2];
                        pos[it2-2] = pos[it2];
                        pos[it2] = temp;
                    }
                }            
            }
        }
    }
    
//    for(int it=0; it<pos.size(); it++)
//        cout << netlist[pos[it]].name << " ";
//    return pos;
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
    //cout << "Estimated maximum width: " << estWidth << "n\nEstimated height: " << estHeight << "n\nEstimated maximum area:" << estArea << "n²\n";
    
    
    vector<Net> netlist_n = indexNetlist(nSt);
    vector<Net> netlist_p = indexNetlist(pSt);
    
    vector<Grapher> eulerGraphs;

    cout << "NETLIST P\n";
    int count =0;
    for (auto it = netlist_p.begin(); it != netlist_p.end(); it++){
        std::cout << count << ": " << it->name << " " << it->type << " " << it->wTransistor << "\n";
        count++;
    }
    
    cout << "NETLIST N\n";
    count =0;
    for (auto it = netlist_n.begin(); it != netlist_n.end(); it++){
        std::cout << count << ": " << it->name << " " << it->type << " " << it->wTransistor << "\n";
        count++;
    }
    
    vector< pair<int, iPair> > edges_p = consEdge(pSt, netlist_p);
    Grapher gp(netlist_p.size(), edges_p.size(), edges_p);
   
        vector< pair<int, iPair> > edges_n = consEdge(nSt, netlist_n);
    Grapher gn(netlist_n.size(), edges_n.size(), edges_n);
    
    vector<int> fp_pos, fn_pos, sp_pos, sn_pos;
    int fp_pos_tracks, fn_pos_tracks, sp_pos_tracks, sn_pos_tracks;
    
    fp_pos = fstackPlacement(gp, netlist_p, maxTracks, maxFins);
    sn_pos = sstackPlacement(fp_pos, netlist_p, netlist_n, maxFins, maxTracks);
    fp_pos_tracks = print_layout(fp_pos, netlist_p, maxFins, maxTracks);    
    sn_pos_tracks = print_layout(sn_pos, netlist_n, maxFins, maxTracks);
    sn_pos = optimizing_spos(sn_pos, netlist_n);
//    sn_pos_tracks = print_layout(sn_pos, netlist_n, maxFins, maxTracks);
//    int fp_max = max(fp_pos_tracks, sn_pos_tracks);
//    cout << "\n\n";
     
//    fn_pos = fstackPlacement(gn, netlist_n, maxTracks, maxFins);
//    sp_pos = sstackPlacement(fn_pos, netlist_n, netlist_p, maxFins, maxTracks);
//    fn_pos_tracks = print_layout(fn_pos, netlist_n, maxFins, maxTracks);
//    sp_pos_tracks = print_layout(sp_pos, netlist_p, maxFins, maxTracks);  
//    sp_pos = optimizing_spos(sp_pos, netlist_p);
//    sp_pos_tracks = print_layout(sp_pos, netlist_p, maxFins, maxTracks);  
//    int fn_max = max(fn_pos_tracks, sp_pos_tracks);
//    
//    if(fp_max > fn_max)
//        cout << "Segunda opção";
//    else cout << "Primeira opção";

}
    
    
    
    



