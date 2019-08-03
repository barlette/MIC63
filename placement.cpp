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
#include "view.h"


using namespace std;

//int height, width; 

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

int print_layout(vector<int> pos, vector<Net> netlist_p, int maxFins, int maxTracks, int output){
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
    if(output){
        for(int row = 0; row < maxFins; row++){
            for(int column = 0; column < trackIndex; column++){
                cout << grade[row][column] << " ";
            }
            cout << "\n";
        }
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
                    //cout << selected_gate-1 << " " << selected_gate << " " << selected_gate+1 << " "; 
                    spos[pos_index] = selected_gate;
                    spos[pos_index+1] = selected_gate+1;
                    pos_index = pos_index+3;
                    it2 = snetlist.size();
                }
            }
        }
    }
    
//    cout << "TESTANDO MIRROR\n";
//    for(int it=0; it<spos.size(); it++){
//        cout << spos[it] << " ";        
//    }
    
//    int index=0;
//    if((snetlist[spos[index]].name == snetlist[spos[index+3]].name) && (snetlist[spos[index]].type == ACTIVE) && (snetlist[spos[index+3]].type == ACTIVE)){
//            spos[index] = spos[index+2];
//            spos[index+2] = spos[index+3];
//        }
//    
//    for(int it=5; it<spos.size()-3; it++){
//        if((snetlist[spos[it]].name == snetlist[spos[it+3]].name) && (snetlist[spos[it]].type == ACTIVE) && (snetlist[spos[it+3]].type == ACTIVE)){
//            spos[it+3] = spos[it+1]; 
//            spos[it+1] = spos[it];
//            //it = it+3;
//        }
//    }
//    cout << "\n";
//    for(int it=0; it<spos.size(); it++){
//        cout << snetlist[spos[it]].name << " ";        
//    }
    return spos;
} 

int max(int i, int v){
    if(i < v){
        return v;
    } else return i;
}

/*
 *  OTIMIZAÇÃO SPOS I
 *  RECONHECE PADRÕES DO TIPO:
 *                  NET3 - A - VDD - GND - B - NET3
 *                  VDD - A - NET3 - GND - B - NET3
 *                  NET3 - A - VDD - NET3 - B - GND
    , ONDE SE PODE ESPELHAR UM DOS TRANSISTORES
 * 
 * RECEBE COMO ARGUMENTO:
 *                          POSICIONAMENTO PRÉVIO   -> vector <int> pos;
 *                          NETLIST                 -> vector <Net> netlist;
 */
vector <int> optimizing_sposI(vector <int> spos, vector<Net> snetlist){
    vector<int> temp;
    vector < vector<int> > sep;
    for(int it =0; it<spos.size();it++){
        //cout << it << " ";
        vector<int>::const_iterator first = spos.begin()+it;
        vector<int>::const_iterator last = spos.begin()+it+3;
        temp = vector <int> (first, last);        
        sep.push_back(temp);
        it = it+2;
    }
    
    if(sep.size() == 1){
        return spos;
    }
    
//    cout << "\n";
//    for(int it =0; it<sep.size(); it++){
//        for(int it2 = 0; it2<sep[it].size();it2++){
//            cout << sep[it][it2] << " ";
//        }
//        cout << "\n";
//    }
//   
//    cout << "\n";
    
    for(int it =0; it<sep.size()-1; it++){
        int it2 = it+1;
        if((it == 0) || snetlist[sep[it][0]].name != snetlist[sep[it-1][sep[it-1].size()-1]].name){
            if((snetlist[sep[it][0]].name == snetlist[sep[it2][0]].name)){
                reverse(sep[it].begin(), sep[it].end());
            } else if (snetlist[sep[it][0]].name == snetlist[sep[it2][sep[it2].size()-1]].name){
                reverse(sep[it].begin(), sep[it].end());
                reverse(sep[it2].begin(), sep[it2].end());              
            } else if(snetlist[sep[it][sep[it].size()-1]].name == snetlist[sep[it2][sep[it2].size()-1]].name){
                //cout << "ALO";
                reverse(sep[it2].begin(), sep[it2].end());              
            }
        } else {            
            if(snetlist[sep[it][sep[it].size()-1]].name == snetlist[sep[it2][sep[it2].size()-1]].name){
                //cout << "ALO";
                reverse(sep[it2].begin(), sep[it2].end());              
            }            
        }
    }
 

    int index =0;
    for(int it =0; it<sep.size(); it++){
        for(int it2 = 0; it2<sep[it].size();it2++){
            //cout << sep[it][it2] << " ";
            spos[index] = sep[it][it2];
            index++;
        }
        //cout << "\n";
    }
    
    return spos;
}

vector <int> optimizing_sposIII(vector <int> spos, vector<Net> snetlist){
    vector<int> temp;
    vector < vector<int> > sep;
    for(int it =0; it<spos.size();it++){
        if((snetlist[spos[it]].type == ACTIVE) && (snetlist[spos[it+1]].type == GATE) && (it==0 || (snetlist[spos[it]].name != snetlist[spos[it-1]].name))){
            cout << "it number:" << it << "\n";
            cout<< "it: " << snetlist[spos[it]].name << " ";
            for(int it2 = it+1; it2 <spos.size(); it2++){
                //cout << "it2 number: " << it2 << " ";
                if (it2 == spos.size()-1){
                    cout << "TESTE5555";
                    cout << "it2: " << snetlist[spos[it2]].name << " ";
                    vector<int>::const_iterator first = spos.begin()+it;
                    vector<int>::const_iterator last = spos.begin()+it2+1;
                    temp = vector <int> (first, last);
                    it=it2+1;
                    it2 = spos.size();                    
                } else if((snetlist[spos[it2]].name != snetlist[spos[it2+1]].name) && (snetlist[spos[it2]].type == ACTIVE) && (snetlist[spos[it2+1]].type == ACTIVE)){
                    cout << "it2: " << snetlist[spos[it2]].name << " ";
                    vector<int>::const_iterator first = spos.begin()+it;
                    vector<int>::const_iterator last = spos.begin()+it2+1;
                    temp = vector <int> (first, last);
                    //it=it2+1;
                    it2 = spos.size();
                }
            }
            
            for(int it3 =0;it3<temp.size();it3++)
                cout << temp[it3] << " ";
            sep.push_back(temp);
        }
    }
    
    if(sep.size() == 1){
        return spos;
    }
    
    cout << "\n";
    for(int it =0; it<sep.size(); it++){
        for(int it2 = 0; it2<sep[it].size();it2++){
            cout << sep[it][it2] << " ";
        }
        cout << "\n";
    }
    
   // cout << "\n";
    
    for(int it =0; it<sep.size(); it++){
        for(int it2 = it+1; it2<sep.size();it2++){
            if(snetlist[sep[it][0]].name == snetlist[sep[it2][0]].name){
                reverse(sep[it].begin(), sep[it].end());
                temp = sep[it+1];
                sep[it+1] = sep[it2];
                sep[it2] = temp;
            } else if (snetlist[sep[it][0]].name == snetlist[sep[it2][sep[it2].size()-1]].name){
                reverse(sep[it].begin(), sep[it].end());
                reverse(sep[it2].begin(), sep[it2].end());
                temp = sep[it+1];
                sep[it+1] = sep[it2];
                sep[it2] = temp;               
            } else if(snetlist[sep[it][sep[it].size()-1]].name == snetlist[sep[it2][0]].name){
                temp = sep[it+1];
                sep[it+1] = sep[it2];
                sep[it2] = temp; 
            } else if(snetlist[sep[it][sep[it].size()-1]].name == snetlist[sep[it2][sep[it2].size()-1]].name){
                reverse(sep[it2].begin(), sep[it2].end());
                temp = sep[it+1];
                sep[it+1] = sep[it2];
                sep[it2] = temp;                 
            }
        }
    }

    int index =0;
    vector<int> temp_pos; 
    for(int it =0; it<sep.size(); it++){
        for(int it2 = 0; it2<sep[it].size();it2++){
            //cout << sep[it][it2] << " ";
            temp_pos.push_back(sep[it][it2]);
            //index++;
        }
        //cout << "\n";
    }
    
    return temp_pos;
}

pair< vector <int>, vector <int> > optimizing_sposII(vector <int> spos, vector<Net> snetlist, vector <int> fpos, vector<Net> fnetlist){   

    spos = optimizing_sposIII(spos, snetlist);  

    
    //cout << "teste2\n"; 
    for(int it=0; it<spos.size(); it++){
        //cout << fnetlist[fpos[it]].name << " " << snetlist[spos[it]].name << "\n";
        if((snetlist[spos[it]].type == GATE) && (fnetlist[fpos[it]].name != snetlist[spos[it]].name)){
            //cout << fnetlist[fpos[it]].name << "\n";
            for(int it2=it+3;it2<fpos.size();it2++){
                if(fnetlist[fpos[it2]].name == snetlist[spos[it]].name){
                    auto ind = fpos.insert(fpos.begin()+it-1, fpos[it2-1]);
                    fpos.erase(fpos.begin()+it2);
                    ind = fpos.insert(ind+1, fpos[it2]);
                    fpos.erase(fpos.begin()+it2+1);
                    fpos.insert(ind+1, fpos[it2+1]);                  
                    fpos.erase(fpos.begin()+it2+2);
                    it2 = fpos.size();
                } 
            }
        }
    }
    
    pair < vector <int>, vector <int> > result;
    result.first = spos;
    result.second = fpos;
    return result;
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
    
    nFins = nFins+2;
    pFins = pFins+2;
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
    
    int n_temp_tracks, p_temp_tracks, max_temp;
    pair < vector <int>, vector <int> > opt;    
    fp_pos = fstackPlacement(gp, netlist_p, maxTracks, maxFins);
    sn_pos = sstackPlacement(fp_pos, netlist_p, netlist_n, maxFins, maxTracks);
    cout << "\n";
    for(int it=0; it<sn_pos.size(); it++)
        cout << fp_pos[it] << " ";
        //cout << sn_pos[it] << " ";
    fp_pos_tracks = print_layout(fp_pos, netlist_p, pFins, maxTracks, 1);    
    sn_pos_tracks = print_layout(sn_pos, netlist_n, nFins, maxTracks, 1);
    for(int it=0; it<sn_pos.size(); it++)
        cout << sn_pos[it] << " ";
    
    int last_tracks;
    int f_temp_tracks = fp_pos_tracks;
    int s_temp_tracks = sn_pos_tracks;
    vector<int> s_pos = sn_pos;
    vector<int> f_pos = fp_pos;
    vector<Net> netlist_s = netlist_n;
    vector<Net> netlist_f = netlist_p;
    vector<int> s_temp = s_pos, f_temp = f_pos;
    int last_s_tracks;
    vector <int> last_pos;
    
    //cout << "teste1";
        do {
            //cout << "teste2";
            last_pos = s_temp;
            cout << "Second positioning optimization, transistor mirroring only\n";
            s_temp = optimizing_sposI(s_pos, netlist_s);
            f_temp_tracks = print_layout(f_temp, netlist_f, pFins, maxTracks, 1);    
            s_temp_tracks = print_layout(s_temp, netlist_s, nFins, maxTracks, 1);
        } while(last_pos != s_temp);
        s_pos = s_temp;
        cout << "teste3";
        f_temp_tracks = print_layout(f_pos, netlist_f, pFins, maxTracks, 1);    
        s_temp_tracks = print_layout(s_pos, netlist_s, nFins, maxTracks, 1);
        do {
            cout << "teste4";
            last_s_tracks = s_temp_tracks;
             cout << "Second positioning optimization, blocks of transistors\n";
            opt = optimizing_sposII(s_pos, netlist_s, f_pos, netlist_f);
            s_temp = opt.first;
            f_temp = opt.second;
            f_temp_tracks = print_layout(f_temp, netlist_f, pFins, maxTracks, 1);    
            s_temp_tracks = print_layout(s_temp, netlist_s, nFins, maxTracks, 1);
            if(last_s_tracks > s_temp_tracks){
                s_pos = s_temp;
                f_pos = f_temp;
            }
        } while(last_s_tracks > s_temp_tracks);
        int last_f_tracks; 
        do {
            //cout << "teste654654:1";
            last_f_tracks = f_temp_tracks;
            //cout << "teste654654:2";
            cout << "First positioning optimization, blocks of transistors\n";
            opt = optimizing_sposII(f_pos, netlist_f, s_pos, netlist_s);
            //cout << "teste654654:3";
            f_temp = opt.first;
            s_temp = opt.second;
            f_temp_tracks = print_layout(f_temp, netlist_f, pFins, maxTracks, 1);    
            s_temp_tracks = print_layout(s_temp, netlist_s, nFins, maxTracks, 1);
            if(last_f_tracks > f_temp_tracks){
                s_pos = s_temp;
                f_pos = f_temp;
            }
        } while(last_f_tracks > f_temp_tracks);
//        cout << "teste5";
//        cout << f_temp_tracks;
        
                do {
            //cout << "teste2";
            last_pos = s_temp;
            cout << "Second positioning optimization, transistor mirroring only\n";
            s_temp = optimizing_sposI(s_pos, netlist_s);
            f_temp_tracks = print_layout(f_temp, netlist_f, pFins, maxTracks, 1);    
            s_temp_tracks = print_layout(s_temp, netlist_s, nFins, maxTracks, 1);
        } while(last_pos != s_temp);
        s_pos = s_temp;
       //cout << "teste3";
        f_temp_tracks = print_layout(f_pos, netlist_f, pFins, maxTracks, 1);    
        s_temp_tracks = print_layout(s_pos, netlist_s, nFins, maxTracks, 1);   
       
        sn_pos = s_pos;
        fp_pos = f_pos;
        
        
        
    

    fn_pos = fstackPlacement(gn, netlist_n, maxTracks, maxFins);
    sp_pos = sstackPlacement(fn_pos, netlist_n, netlist_p, maxFins, maxTracks);
    sp_pos_tracks = print_layout(sp_pos, netlist_p, pFins, maxTracks, 0); 
    fn_pos_tracks = print_layout(fn_pos, netlist_n, nFins, maxTracks, 0);
    
    f_temp_tracks = fn_pos_tracks;
    s_temp_tracks = sp_pos_tracks;
    s_pos = sp_pos;
    f_pos = fn_pos;
    netlist_s = netlist_p;
    netlist_f = netlist_n;
    s_temp = s_pos; 
    f_temp = f_pos;
    
    //cout << "teste1";
        do {
            //cout << "teste2";
            last_s_tracks = s_temp_tracks;
            s_temp = optimizing_sposI(s_pos, netlist_s);
            f_temp_tracks = print_layout(f_temp, netlist_f, pFins, maxTracks, 0);    
            s_temp_tracks = print_layout(s_temp, netlist_s, nFins, maxTracks, 0);
            if(last_s_tracks > s_temp_tracks){
                //cout << "oi\n";
                s_pos = s_temp;
            }
        } while(last_s_tracks > s_temp_tracks);
       // cout << "teste3";
        f_temp_tracks = print_layout(f_pos, netlist_f, pFins, maxTracks, 0);    
        s_temp_tracks = print_layout(s_pos, netlist_s, nFins, maxTracks, 0);
        do {
            // << "teste4";
            last_s_tracks = s_temp_tracks;
            opt = optimizing_sposII(s_pos, netlist_s, f_pos, netlist_f);
            s_temp = opt.first;
            f_temp = opt.second;
            f_temp_tracks = print_layout(f_temp, netlist_f, pFins, maxTracks, 0);    
            s_temp_tracks = print_layout(s_temp, netlist_s, nFins, maxTracks, 0);
            if(last_s_tracks > s_temp_tracks){
                s_pos = s_temp;
                f_pos = f_temp;
            }
        } while(last_s_tracks > s_temp_tracks);
//        //cout << "teste5";
//        //cout << s_temp_tracks; 
        do {
            //cout << "teste6";
            last_f_tracks = f_temp_tracks;
            f_temp = optimizing_sposI(f_pos, netlist_f);
            f_temp_tracks = print_layout(f_temp, netlist_f, pFins, maxTracks, 0);    
            s_temp_tracks = print_layout(s_pos, netlist_s, nFins, maxTracks, 0);
            if(last_f_tracks > f_temp_tracks){
                f_pos = f_temp;
            }
        } while(last_f_tracks > f_temp_tracks);  
        
         do {
            //cout << "teste2";
            last_s_tracks = s_temp_tracks;
            s_temp = optimizing_sposI(s_pos, netlist_s);
            f_temp_tracks = print_layout(f_temp, netlist_f, pFins, maxTracks, 0);    
            s_temp_tracks = print_layout(s_temp, netlist_s, nFins, maxTracks, 0);
            if(last_s_tracks > s_temp_tracks){
                //cout << "oi\n";
                s_pos = s_temp;
            }
        } while(last_s_tracks > s_temp_tracks);
        //cout << "teste3";
        f_temp_tracks = print_layout(f_pos, netlist_f, pFins, maxTracks, 0);    
        s_temp_tracks = print_layout(s_pos, netlist_s, nFins, maxTracks, 0);
        
        sp_pos = s_pos;
        fn_pos = f_pos;
        
        cout << "#### FIRST P ####\n";
        fp_pos_tracks = print_layout(fp_pos, netlist_p, pFins, maxTracks, 1);    
        sn_pos_tracks = print_layout(sn_pos, netlist_n, nFins, maxTracks, 1);         
        cout << "#################\n";
        cout << "#### FIRST N ####\n";
        sp_pos_tracks = print_layout(sp_pos, netlist_p, pFins, maxTracks, 1);         
        fn_pos_tracks = print_layout(fn_pos, netlist_n, nFins, maxTracks, 1);     
        cout << "#################\n";
        
        
        int minfp, minfn, numberTracks;
        minfp =  std::max(fp_pos_tracks, sn_pos_tracks);
        minfn =  std::max(fn_pos_tracks, sp_pos_tracks);
        if(minfp < minfn)
            renderizeMatrix(fp_pos, netlist_p, sn_pos, netlist_n, pFins, minfp);
        else renderizeMatrix(sp_pos, netlist_p, fn_pos, netlist_n, nFins, minfn);

        
}
    
    
    
    



