/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "../opencv4/opencv2/highgui.hpp"
#include "../opencv4/opencv2/opencv.hpp"
#include "../opencv4/opencv2/imgproc.hpp"
#include <iostream>
#include "view.h"
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
#include <opencv4/opencv2/imgcodecs.hpp>
#include "view.h"

using namespace cv;
using namespace std;

int height, width;

class point {
public:
    point( int a = 0, int b = 0 ) { x = a; y = b; }
    bool operator ==( const point& o ) { return o.x == x && o.y == y; }
    point operator +( const point& o ) { return point( o.x + x, o.y + y ); }
    int x, y;
};
 

 
class node {
public:
    bool operator == (const node& o ) { return pos == o.pos; }
    bool operator == (const point& o ) { return pos == o; }
    point pos, parent;
    int f, g, h;
};

//class map {
//public:
//    int largura, altura;
//    map(coord s, coord f, int Height, int Width) {
//        int temp[Height][Width];
//            for(int it = 0; it<Height; it++){
//                for(int it2=0;it<Width;it2++){
//                    if(it2 == s.first && it == s.second){
//                        temp[it][it2] = 1;
//                    } else if (it2 == f.first && it == f.second){
//                        temp[it][it2] = 1;                        
//                    }
//                }
//            }
//        
//        largura = Width;
//        altura = Height;
//        //m = temp;
////        for( int r = 0; r < altura; r++ )
////            for( int s = 0; s < largura; s++ )
////                m[s][r] = t[r][s];
//    }
//    vector < vector <int>> m;
//    int operator() ( int x, int y ) { return m[x][y]; }
//    
//};
//
//class aStar {
//public:
//    
//    aStar() {
//        neighbours[0] = point( -1, -1 ); neighbours[1] = point(  1, -1 );
//        neighbours[2] = point( -1,  1 ); neighbours[3] = point(  1,  1 );
//        neighbours[4] = point(  0, -1 ); neighbours[5] = point( -1,  0 );
//        neighbours[6] = point(  0,  1 ); neighbours[7] = point(  1,  0 );
//    }
// 
//    int calcDist( point& p ){
//        // need a better heuristic
//        int x = end.x - p.x, y = end.y - p.y;
//        return( x * x + y * y );
//    }
//    
//    bool isValid(point& p, int Width, int Height){
//        return (p.x >-1 && p.y > -1 && p.x < Width && p.y < Height);
//    }
//    
//    bool existPoint( point& p, int cost ) {
//        std::list<node>::iterator i;
//        i = std::find( closed.begin(), closed.end(), p );
//        if( i != closed.end() ) {
//            if( ( *i ).cost + ( *i ).dist < cost ) return true;
//            else { closed.erase( i ); return false; }
//        }
//        i = std::find( open.begin(), open.end(), p );
//        if( i != open.end() ) {
//            if( ( *i ).cost + ( *i ).dist < cost ) return true;
//            else { open.erase( i ); return false; }
//        }
//        return false;
//    }
// 
//    bool fillOpen( node& n ) {
//        int stepCost, nc, dist;
//        point neighbour;
// 
//        for( int x = 0; x < 8; x++ ) {
//            // one can make diagonals have different cost
//            stepCost = x < 4 ? 1 : 1;
//            neighbour = n.pos + neighbours[x];
//            if( neighbour == end ) return true;
// 
//            if(isValid( neighbour, m.largura, m.altura ) && m( neighbour.x, neighbour.y ) != 1 ) {
//                nc = stepCost + n.cost;
//                dist = calcDist( neighbour );
//                if( !existPoint( neighbour, nc + dist ) ) {
//                    node m;
//                    m.cost = nc; m.dist = dist;
//                    m.pos = neighbour; 
//                    m.parent = n.pos;
//                    open.push_back( m );
//                }
//            }
//        }
//        return false;
//    }
// 
//    bool search( point& s, point& e, map& mp ) {
//        node n; end = e; start = s; m = mp;
//        n.cost = 0; n.pos = s; n.parent = 0; n.dist = calcDist( s ); 
//        open.push_back( n );
//        while( !open.empty() ) {
//            //open.sort();
//            node n = open.front();
//            open.pop_front();
//            closed.push_back( n );
//            if( fillOpen( n ) ) return true;
//        }
//        return false;
//    }
// 
//    int path( std::list<point>& path ) {
//        path.push_front( end );
//        int cost = 1 + closed.back().cost; 
//        path.push_front( closed.back().pos );
//        point parent = closed.back().parent;
// 
//        for( std::list<node>::reverse_iterator i = closed.rbegin(); i != closed.rend(); i++ ) {
//            if( ( *i ).pos == parent && !( ( *i ).pos == start ) ) {
//                path.push_front( ( *i ).pos );
//                parent = ( *i ).parent;
//            }
//        }
//        path.push_front( start );
//        return cost;
//    }
// 
//    map m; 
//    point end, start;
//    point neighbours[8];
//    std::list<node> open;
//    std::list<node> closed;
//};    

int calcDist( point& p, point &end ){
    // need a better heuristic
    int x = end.x - p.x, y = end.y - p.y;
    return( x * x + y * y );
}
    
int renderizeMatrix(vector<int> p_pos, vector<Net> netlist_p, vector<int> n_pos, vector<Net> netlist_n, int nfin, int ntracks){

    
    vector <Net> new_p, new_n, global;
    vector <int> new_ppos, new_npos;
    int exists=0;
    //new indexing
    for(int it=0; it<netlist_p.size();it++){
        exists=0;
        for(int it2=0; it2<new_p.size(); it2++){
            if(netlist_p[it].name == new_p[it2].name){
                exists=1;
            }
        }
        if(exists == 0){
            new_p.push_back(netlist_p[it]);
        }
    }

    for(int it=0; it<netlist_n.size();it++){
        exists=0;
        for(int it2=0; it2<new_n.size(); it2++){
            if(netlist_n[it].name == new_n[it2].name){
                exists=1;
            }
        }
        if(exists == 0){
            new_n.push_back(netlist_n[it]);
        }
    }
    cout << "\n";
    for(int it2=0; it2<new_p.size(); it2++){   
        cout << new_p[it2].name << " ";
    } 
    cout << "\n";
    for(int it2=0; it2<new_n.size(); it2++){   
        cout << new_n[it2].name << " ";
    }    
    
    for(int it=0;it<new_p.size();it++){
        global.push_back(new_p[it]);
    }
    
    for(int it=0;it<new_n.size();it++){
        exists=0;
        for(int it2=0;it2<global.size();it2++){
            if(new_n[it].name == global[it2].name){
                exists=1;
            }
        }
        if(exists == 0){
            global.push_back(new_n[it]);
        }            
    }
    
    cout << "GLOBAL\n";
    for(int it2=0; it2<global.size(); it2++){   
        cout << global[it2].name << " ";
    }     
    
    
    for(int it=0; it<p_pos.size();it++){
        for(int it2=0; it2<global.size();it2++){
            if(netlist_p[p_pos[it]].name == global[it2].name){
                new_ppos.push_back(it2);
                cout << it2;
            }
        }
    }
   
    for(int it=0; it<n_pos.size();it++){
        for(int it2=0; it2<global.size();it2++){
            if(netlist_n[n_pos[it]].name == global[it2].name){
                new_npos.push_back(it2);
                cout << it2;
            }
        }
    }
    
    ntracks=0;
    for(int it=0;it<new_ppos.size()-1;it++){
        if(new_ppos[it] != new_ppos[it+1])
            ntracks++;
    }
    
    height = (2*nfin)*7 + (2*nfin-1)*20 + 64;
    width = ntracks * 54;
    
    int     GCUT[height][width];
    int     M1[height][width];
    int     LIG[height][width];
    int     FIN[height][width];
    int     CGATE[height][width];
    int     LISD[height][width];
    int     SDT[height][width];
    int     CACTIVE[height][width];
    int    SIGNAL[height][width];    
    
    for(int it=0; it<height; it++){
        for(int it2=0; it2<width; it2++){
            if(((it >= 0) && (it <= 43)) || ((it >= (height-44)) && (it <= (height-1)))){
                GCUT[it][it2] = 1;
            }
            if(((it >= 13) && (it <= 30)) || ((it >= (height-31)) && (it <= (height-14)))){
                M1[it][it2] = 1;
            }
            if(((it >= 14) && (it <= 29)) || ((it >= (height-30)) && (it <= (height-15)))){
                LIG[it][it2] = 1;
            }            
            //cout << GCUT[it][it2];
            cout << LIG[it][it2];
        }
        cout << "\n";
    }
   //cout << nfin;
    cout << height << " nm " << width << " nm\n";
    
    int finHeight=0;
    for(int itFin=0; itFin<(2*nfin); itFin++){
        finHeight = 32+(itFin*27);
        for(int it=finHeight; it<finHeight+7; it++){
            for(int it2=0; it2<width; it2++){
                FIN[it][it2] = 1;
            }
        }
    }
    
    
    
    int gateWidth=0;
    for(int itG=0; itG<ntracks; itG++){
        gateWidth = 17+(itG*54);
        for(int it=gateWidth; it<gateWidth+20; it++){
            for(int it2=0; it2<height; it2++){
                CGATE[it2][it] = 1;
            }
        }
    }
    
    vector <int> p_pos_types, n_pos_types;
    
    for(int it=0; it<new_ppos.size()-2;it=it+3){
        p_pos_types.push_back(ACTIVE);
        p_pos_types.push_back(GATE);
        p_pos_types.push_back(ACTIVE);
    }
    
    for(int it=0; it<new_npos.size()-2;it=it+3){
        n_pos_types.push_back(ACTIVE);
        n_pos_types.push_back(GATE);
        n_pos_types.push_back(ACTIVE);
    }
    
    for(int it=0; it<new_npos.size();it++){
        cout << n_pos_types[it] << " ";

    }
    cout << "\n";
    
    signal mSignal[global.size()];
    
    int last_ppos, sxcoord = 42, sycoord = 48, salimcoord = 22, salimHeight = 108, sHeigth = 81, actWidth = 24, space = 5, ligcontxcoord = 70, ligcontycoord = 145, ligContWidth = 22, ligContHeight = 20;
    gateWidth = 20;
    for(int it=0; it<new_ppos.size(); it++){
        cout << "testeXXX\n";
        cout << "it: " << global[new_ppos[it]].name << "\n";
        cout << p_pos_types[it];
            if(p_pos_types[it] == GATE){
                for(int it2=sxcoord;it2<sxcoord+gateWidth;it2++){
                    for(int it3=43; it3<height-44;it3++){
                        SIGNAL[it3][it2] = new_ppos[it]+1;
                    }
                }
                for(int it2 = ligcontxcoord; it2 <= ligcontxcoord+ligContWidth; it2++){
                    for(int it3=ligcontycoord; it3 <= ligcontycoord+ligContHeight; it3++){
                        LIG[it3][it2] = 1;
                    }
                }
                point2 temp((ligcontxcoord+(ligContWidth/2)), (ligcontycoord+(ligContHeight/2)));
                mSignal[new_ppos[it]].instances.push_back(temp);
                ligcontxcoord = ligcontxcoord + 54;
                sxcoord = sxcoord + 25;
            } else if((global[new_ppos[it]].name != global[new_ppos[it-1]].name) && (p_pos_types[it] != p_pos_types[it-1]) && (p_pos_types[it] == ACTIVE) && (it != 0)){
                point2 temp((sxcoord+(actWidth/2)), (sycoord+(sHeigth/2)));
                mSignal[new_ppos[it]].instances.push_back(temp);
                
                if(global[new_ppos[it]].name == "VDD") {
                    cout << "teste1\n";
                    for(int it2=sxcoord;it2<sxcoord+actWidth;it2++){
                        for(int it3=salimcoord; it3<salimcoord+salimHeight;it3++){
                            SIGNAL[it3][it2] = new_ppos[it]+1;
                        }
                    }                    
                } else {
                    // cout << it-1 << " " << it << "\n";
                    for(int it2=sxcoord;it2<sxcoord+actWidth;it2++){
                        for(int it3=sycoord; it3<sycoord+sHeigth;it3++){
                            SIGNAL[it3][it2] = new_ppos[it]+1;
                        }
                    }
                }
                sxcoord = sxcoord + 29;
            } else if((global[new_ppos[it]].name != global[new_ppos[it-1]].name) && (p_pos_types[it] == p_pos_types[it-1]) && (it != 0)){
                point2 temp((sxcoord+(actWidth/2)), (sycoord+(sHeigth/2)));
                mSignal[new_ppos[it]].instances.push_back(temp);
                sxcoord = sxcoord+79;
                if(global[new_ppos[it]].name == "VDD") {
                    for(int it2=sxcoord;it2<sxcoord+actWidth;it2++){
                        for(int it3=salimcoord; it3<salimcoord+salimHeight;it3++){
                            SIGNAL[it3][it2] = new_ppos[it]+1;
                        }
                    }                    
                } else {
                    for(int it2=sxcoord;it2<sxcoord+actWidth;it2++){
                        for(int it3=sycoord; it3<sycoord+sHeigth;it3++){
                            SIGNAL[it3][it2] = new_ppos[it]+1;
                        }
                    }
                }
                sxcoord = sxcoord + 29;
            } else if(it == 0){
                point2 temp((sxcoord+(actWidth/2)), (sycoord+(sHeigth/2)));
                mSignal[new_ppos[it]].instances.push_back(temp);
                if(global[new_ppos[it]].name == "VDD") {
                    for(int it2=sxcoord;it2<sxcoord+actWidth;it2++){
                        for(int it3=salimcoord; it3<salimcoord+salimHeight;it3++){
                            SIGNAL[it3][it2] = new_ppos[it]+1;
                        }
                    }                    
                } else {
                    for(int it2=sxcoord;it2<sxcoord+actWidth;it2++){
                        for(int it3=sycoord; it3<sycoord+sHeigth;it3++){
                            SIGNAL[it3][it2] = new_ppos[it]+1;
                        }
                    }
                }
                sxcoord = sxcoord + 29;
            }
    }
    //54+48+81
    
    sxcoord = 42;
    sycoord = 183;
    for(int it=0; it<new_npos.size(); it++){
        if(/*(global[new_npos[it]].name != global[new_npos[it-1]].name) &&*/ (n_pos_types[it] != n_pos_types[it-1]) && (n_pos_types[it] == ACTIVE)/*&& (it != 0)*/){
               cout << it-1 << " " << it << "\n";
                if(global[new_npos[it]].name == "GND") {
                    for(int it2=sxcoord;it2<sxcoord+actWidth;it2++){
                        for(int it3=sycoord; it3<sycoord+salimHeight;it3++){
                            SIGNAL[it3][it2] = new_npos[it]+1;
                        }
                    }                    
                } else {
                    for(int it2=sxcoord;it2<sxcoord+actWidth;it2++){
                        for(int it3=sycoord; it3<sycoord+sHeigth;it3++){
                            SIGNAL[it3][it2] = new_npos[it]+1;
                        }
                    }
                }
                point2 temp((sxcoord+(actWidth/2)), (sycoord+(sHeigth/2)));
                mSignal[new_npos[it]].instances.push_back(temp);
                sxcoord = sxcoord + 54;
            } else if((global[new_npos[it]].name != global[new_npos[it-1]].name) && (n_pos_types[it] == n_pos_types[it-1])/*&& (it != 0)*/){
                point2 temp((sxcoord+(actWidth/2)), (sycoord+(sHeigth/2)));
                mSignal[new_npos[it]].instances.push_back(temp);
                cout << it-1 << " " << it << "\n";
                sxcoord = sxcoord+54;
                if(global[new_npos[it]].name == "GND") {
                    for(int it2=sxcoord;it2<sxcoord+actWidth;it2++){
                        for(int it3=sycoord; it3<sycoord+salimHeight;it3++){
                            SIGNAL[it3][it2] = new_npos[it]+1;
                        }
                    }                    
                } else {
                    for(int it2=sxcoord;it2<sxcoord+actWidth;it2++){
                        for(int it3=sycoord; it3<sycoord+sHeigth;it3++){
                            SIGNAL[it3][it2] = new_npos[it]+1;
                        }
                    }
                sxcoord = sxcoord + 54;
                }
            }
    }

    for(int it=0; it<new_ppos.size(); it++){
        if(global[new_ppos[it]].name == "VDD"){
            for(int it2=0; it2<height; it2++){
                for(int it3=0; it3<width; it3++){
                    if((it2 >= 13) && (it2 <= 30)){
                        SIGNAL[it2][it3] = new_ppos[it]+1;
                    }
                }
            }            
        }
    }
    
    for(int it=0; it<new_npos.size(); it++){
        if(global[new_npos[it]].name == "GND"){
            for(int it2=0; it2<height; it2++){
                for(int it3=0; it3<width; it3++){
                    if((it2 >= (height-31)) && (it2 <= (height-14))){
                        SIGNAL[it2][it3] = new_npos[it]+1;
                    }
                }
            }            
        }
    }

    
    cout << "SIGNALS\n\n\n";    
    for(int it=0; it<height; it++){
        for(int it2=0; it2<width; it2++){
            cout << SIGNAL[it][it2];
        }
        cout << "\n";
    }   
    
    for(int it=0; it<height; it++){
        for(int it2=0; it2<width; it2++){
            cout << CGATE[it][it2];
        }
        cout << "\n";
    }

    
    cout << "LISD\n\n\n";
    Net lastNet;
    int xcoord = 41, xactcoord = 45, ycoord = 48, ycontcoord = 22, lisdWidth = 24, lisdHeight = 81, lisdContHeight = 108, activeInitialWidth = 16, activeContWidth = 38;
    for(int it=0; it<p_pos.size(); it++){
        if(((lastNet.name != netlist_p[p_pos[it]].name) && (netlist_p[p_pos[it]].type == lastNet.type))){ // quebra
            if(netlist_p[p_pos[it]].name == "VDD"){
                if(it != 0){
                    xcoord = xcoord+54;
                    xactcoord = xactcoord+92;
                }
                for(int xc=xcoord; xc<xcoord+lisdWidth; xc++){
                    for(int yc=ycontcoord; yc<ycontcoord+lisdContHeight; yc++){
                        LISD[yc][xc] = 1;
                    }
                    for(int yc=ycoord; yc<ycoord+lisdHeight; yc++){
                        SDT[yc][xc] = 1;
                        //SIGNAL[yc][xc] = p_pos[it] +49;
                    }                  
                }
                for(int xc=xactcoord; xc<xactcoord+activeInitialWidth; xc++){
                    for(int yc=ycoord; yc<ycoord+lisdHeight; yc++){
                        CACTIVE[yc][xc] = 1;
                    }                 
                }      
                xactcoord = xactcoord+activeInitialWidth;
                xcoord = xcoord+54;
            } else {
                if(it != 0){
                    xcoord = xcoord+54;
                    xactcoord = xactcoord+92;
                }
                for(int xc=xcoord; xc<xcoord+lisdWidth; xc++){
                    for(int yc=ycoord; yc<ycoord+lisdHeight; yc++){
                        LISD[yc][xc] = 1;
                        SDT[yc][xc] = 1;
                        //SIGNAL[yc][xc] = static_cast<char>(p_pos[it]);                        
                        //CACTIVE[yc][xc] = 1;
                    }
                }
                for(int xc=xactcoord; xc<xactcoord+activeInitialWidth; xc++){
                    for(int yc=ycoord; yc<ycoord+lisdHeight; yc++){
                        CACTIVE[yc][xc] = 1;
                    }                 
                }      
                xactcoord = xactcoord+activeInitialWidth;
                xcoord = xcoord+54;
            }
        //} else if (((lastNet.name == netlist_p[p_pos[it]].name) && (netlist_p[p_pos[it]].type == lastNet.type))){
          //  xcoord = xcoord+30;
        } else if ((netlist_p[p_pos[it]].type != lastNet.type) && (netlist_p[p_pos[it]].type == ACTIVE)){
            if(netlist_p[p_pos[it]].name == "VDD"){
                for(int xc=xcoord; xc<xcoord+lisdWidth; xc++){
                    for(int yc=ycontcoord; yc<ycontcoord+lisdContHeight; yc++){
                        LISD[yc][xc] = 1;
                        //SIGNAL[yc][xc] = static_cast<char>(p_pos[it]);                      
                    }
                    for(int yc=ycoord; yc<ycoord+lisdHeight; yc++){
                        SDT[yc][xc] = 1;
                        //CACTIVE[yc][xc] = 1;
                    }
                }
                xcoord = xcoord+54;
            } else {
                for(int xc=xcoord; xc<xcoord+lisdWidth; xc++){
                    for(int yc=ycoord; yc<ycoord+lisdHeight; yc++){
                        LISD[yc][xc] = 1;
                        SDT[yc][xc] = 1;
                        //SIGNAL[yc][xc] = static_cast<char>(p_pos[it]);                       
                        //CACTIVE[yc][xc] = 1;
                    }
                }
                xcoord = xcoord+54;
            }
            
            for(int yc=ycoord; yc<ycoord+lisdHeight; yc++){
                for(int xc=xactcoord; xc<xactcoord+activeInitialWidth; xc++){
                    CACTIVE[yc][xc] = 1;                                          
                }
            }
                xactcoord = xactcoord+activeInitialWidth;            
        } else if ((netlist_p[p_pos[it]].type != lastNet.type) && (netlist_p[p_pos[it]].type == GATE)){
                        for(int yc=ycoord; yc<ycoord+lisdHeight; yc++){
                for(int xc=xactcoord; xc<xactcoord+activeContWidth; xc++){
                    CACTIVE[yc][xc] = 1;                                          
                }             
            }  
                xactcoord = xactcoord+activeContWidth;   
        } 
        lastNet = netlist_p[p_pos[it]];  
    }
 ///////////////////////////////////////////////////
    cout << "REDE N\n";
    lastNet.name = "void";
    xcoord = 41, xactcoord = 45, ycoord = 183, ycontcoord = 183, lisdWidth = 24, lisdHeight = 81, lisdContHeight = 108, activeInitialWidth = 16, activeContWidth = 38;
    for(int it=0; it<n_pos.size(); it++){
        if(((lastNet.name != netlist_n[n_pos[it]].name) && (netlist_n[n_pos[it]].type == lastNet.type))){ // quebra
            if(netlist_n[n_pos[it]].name == "GND"){
                cout << "teste1- " << netlist_n[n_pos[it]].name << "\n";
                if(it != 0){
                    xcoord = xcoord+54;
                    xactcoord = xactcoord+92;
                }
                for(int xc=xcoord; xc<xcoord+lisdWidth; xc++){
                    for(int yc=ycontcoord; yc<ycontcoord+lisdContHeight; yc++){
                        LISD[yc][xc] = 1;
                    }
                    for(int yc=ycoord; yc<ycoord+lisdHeight; yc++){
                        SDT[yc][xc] = 1;
                        //SIGNAL[yc][xc] = p_pos[it] +49;
                    }                  
                }
                for(int xc=xactcoord; xc<xactcoord+activeInitialWidth; xc++){
                    for(int yc=ycoord; yc<ycoord+lisdHeight; yc++){
                        CACTIVE[yc][xc] = 1;
                    }                 
                }      
                xactcoord = xactcoord+activeInitialWidth;
                xcoord = xcoord+54;
            } else {
                cout << "teste2- " << netlist_n[n_pos[it]].name << "\n";
                if(it != 0){
                    xcoord = xcoord+54;
                    xactcoord = xactcoord+92;
                }
                for(int xc=xcoord; xc<xcoord+lisdWidth; xc++){
                    for(int yc=ycoord; yc<ycoord+lisdHeight; yc++){
                        LISD[yc][xc] = 1;
                        SDT[yc][xc] = 1;
                        //SIGNAL[yc][xc] = static_cast<char>(p_pos[it]);                        
                        //CACTIVE[yc][xc] = 1;
                    }
                }
                for(int xc=xactcoord; xc<xactcoord+activeInitialWidth; xc++){
                    for(int yc=ycoord; yc<ycoord+lisdHeight; yc++){
                        CACTIVE[yc][xc] = 1;
                    }                 
                }      
                xactcoord = xactcoord+activeInitialWidth;
                xcoord = xcoord+54;
            }
        //} else if (((lastNet.name == netlist_p[p_pos[it]].name) && (netlist_p[p_pos[it]].type == lastNet.type))){
          //  xcoord = xcoord+30;
        } else if ((netlist_n[n_pos[it]].type != lastNet.type) && (netlist_n[n_pos[it]].type == ACTIVE)){
            if(netlist_n[n_pos[it]].name == "GND"){
                cout << "teste3- " << netlist_n[n_pos[it]].name << "\n";
                for(int xc=xcoord; xc<xcoord+lisdWidth; xc++){
                    for(int yc=ycontcoord; yc<ycontcoord+lisdContHeight; yc++){
                        LISD[yc][xc] = 1;
                        //SIGNAL[yc][xc] = static_cast<char>(p_pos[it]);                      
                    }
                    for(int yc=ycoord; yc<ycoord+lisdHeight; yc++){
                        SDT[yc][xc] = 1;
                        //CACTIVE[yc][xc] = 1;
                    }
                }
                xcoord = xcoord+54;
            } else {
                cout << "teste4 - " << netlist_n[n_pos[it]].name << "\n";
                for(int xc=xcoord; xc<xcoord+lisdWidth; xc++){
                    for(int yc=ycoord; yc<ycoord+lisdHeight; yc++){
                        LISD[yc][xc] = 1;
                        SDT[yc][xc] = 1;
                        //SIGNAL[yc][xc] = static_cast<char>(p_pos[it]);                       
                        //CACTIVE[yc][xc] = 1;
                    }
                }
                xcoord = xcoord+54;
            }
            
            for(int yc=ycoord; yc<ycoord+lisdHeight; yc++){
                for(int xc=xactcoord; xc<xactcoord+activeInitialWidth; xc++){
                    CACTIVE[yc][xc] = 1;                                          
                }
            }
                xactcoord = xactcoord+activeInitialWidth;            
        } else if ((netlist_n[n_pos[it]].type != lastNet.type) && (netlist_n[n_pos[it]].type == GATE)){
            
                        for(int yc=ycoord; yc<ycoord+lisdHeight; yc++){
                for(int xc=xactcoord; xc<xactcoord+activeContWidth; xc++){
                    CACTIVE[yc][xc] = 1;                                          
                }             
            }  
                xactcoord = xactcoord+activeContWidth;   
        } 
        lastNet = netlist_n[n_pos[it]];  
    }
    
 //////////////////////////////////////////////////////////////
    cout << "LISD\n\n";
        for(int it=0; it<height; it++){
            for(int it2=0; it2<width; it2++){
                cout << LISD[it][it2];
            }
            cout << "\n";
        }
    cout << "SDT\n\n";    
        for(int it=0; it<height; it++){
            for(int it2=0; it2<width; it2++){
                cout << SDT[it][it2];
            }
            cout << "\n";
        }
    cout << "ACTIVE\n\n";    
        for(int it=0; it<height; it++){
            for(int it2=0; it2<width; it2++){
                cout << CACTIVE[it][it2];
            }
            cout << "\n";
        }   

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    Mat img(height, width, CV_8UC4, Scalar(0,0,0,255));
    

    
    if (img.empty()) 
    {
        cout << "\n Image not created. You"
                     " have done something wrong. \n";
        return -1;    // Unsuccessful.
    }

//    int     GCUT[height][width]; ok
//    int     M1[height][width]; ok
//    int     LIG[height][width]; ok
//    int     FIN[height][width]; ok
//    int     CGATE[height][width]; ok
//    int     LISD[height][width]; ok
//    int     SDT[height][width]; ok
//    int     CACTIVE[height][width];
//    int    SIGNAL[height][width];
    
    for(int it=0; it<height; it++){
        for(int it2=0; it2<width; it2++){
            if(CGATE[it][it2] == 1){
                img.at<Vec4b>(it, it2) = Vec4b(0, 0, 255, 100);                
            }
            if(GCUT[it][it2] == 1){
                img.at<Vec4b>(it, it2) = Vec4b(255, 0, 255, 100);                
            }
            if(LIG[it][it2] == 1){
                img.at<Vec4b>(it, it2) = Vec4b(0, 157, 255, 100);                
            }
            if(M1[it][it2] == 1){
                img.at<Vec4b>(it, it2) = Vec4b(255, 0, 0, 100);                
            } 
            if(CACTIVE[it][it2] == 1){
                img.at<Vec4b>(it, it2) = Vec4b(0, 255, 0, 100);                
            } 
            if(LISD[it][it2] == 1){
                img.at<Vec4b>(it, it2) = Vec4b(0, 255, 255, 100);                
            }  
            if(SDT[it][it2] == 1){
                img.at<Vec4b>(it, it2) = Vec4b(255, 255, 255, 50);                
            }
            if(FIN[it][it2] == 1){
                img.at<Vec4b>(it, it2) = Vec4b(0, 255, 0, 100);                
            }           
        }
    }
    
    vector <Mat> dst;
    Mat gate(height, width, CV_8UC4, Scalar(0,0,0,0));
    Mat gcut(height, width, CV_8UC4, Scalar(0,0,0,0));
    Mat lig(height, width, CV_8UC4, Scalar(0,0,0,0));
    Mat m1(height, width, CV_8UC4, Scalar(0,0,0,0));
    Mat active(height, width, CV_8UC4, Scalar(0,0,0,0));
    Mat lisd(height, width, CV_8UC4, Scalar(0,0,0,0));
    Mat sdt(height, width, CV_8UC4, Scalar(0,0,0,0));
    Mat cfin(height, width, CV_8UC4, Scalar(0,0,0,0));
    
    for(int it=0; it<height; it++){
        for(int it2=0; it2<width; it2++){
            if(CGATE[it][it2] == 1){
                gate.at<Vec4b>(it, it2) = Vec4b(0, 0, 255, 255);                
            }
            if(GCUT[it][it2] == 1){
                gcut.at<Vec4b>(it, it2) = Vec4b(255, 0, 255, 255);                
            }
            if(LIG[it][it2] == 1){
                lig.at<Vec4b>(it, it2) = Vec4b(0, 157, 255, 255);                
            }
            if(M1[it][it2] == 1){
                m1.at<Vec4b>(it, it2) = Vec4b(255, 0, 0,255);                
            } 
            if(CACTIVE[it][it2] == 1){
                active.at<Vec4b>(it, it2) = Vec4b(0, 255, 0,255);                
            } 
            if(LISD[it][it2] == 1){
                lisd.at<Vec4b>(it, it2) = Vec4b(0, 255, 255,255);                
            }  
            if(SDT[it][it2] == 1){
                sdt.at<Vec4b>(it, it2) = Vec4b(255, 255, 255,255);                
            }
            if(FIN[it][it2] == 1){
                cfin.at<Vec4b>(it, it2) = Vec4b(0, 255, 0,255);                
            }           
        }
    }

    dst.push_back(gate);
    dst.push_back(gcut);
    dst.push_back(lig);
    dst.push_back(m1);
    dst.push_back(active);
    dst.push_back(lisd);    
    dst.push_back(sdt);
    dst.push_back(cfin);
    
    Mat merged(height, width, CV_8UC4, Scalar(0,0,0,0));
    
    merged = dst[0];
    
    for(Mat mat : dst){
        cv::addWeighted(merged, 1, mat, 0.5, 0, merged);
    }
    
    
    
    cv::imwrite("layout.png", img);
    
    cv::imwrite("gate.png", gate);
    cv::imwrite("gcut.png", gcut);
    cv::imwrite("lig.png", lig);
    cv::imwrite("m1.png", m1);
    cv::imwrite("active.png", active);
    cv::imwrite("lisd.png", lisd);
    cv::imwrite("sdt.png", sdt);
    cv::imwrite("cfin.png", cfin);
    
    cv::imwrite("merged.png", merged);
    
    //namedWindow("A_good_name");    
    //imshow("A_good_name", img);
    //waitKey(1000); //wait infinite time for a keypress
    //destroyWindow("A_good_name"); 
    
    //ROTEAMENTO
    
    int POINTS[height][width];
    
    
    
    for(int it=0; it<global.size();it++){
        for(int it2=0; it2<mSignal[it].instances.size();it2++){
            cout << it << "-> x: " << mSignal[it].instances[it2].s.first << "| y: " << mSignal[it].instances[it2].s.second << "\n";
        }
    }
    
    
    //select first route
    int temp_distance=0, first=0, distance, s, f, o;
    
    
    for(int it=0; it<global.size();it++){
        if(mSignal[it].instances.size() > 1){
            for(int it2=0; it2<mSignal[it].instances.size();it2++){
                for(int it3=0; it3<mSignal[it].instances.size();it3++){
                    temp_distance = pow(abs(mSignal[it].instances[it2].s.first - mSignal[it].instances[it3].s.first),2)+pow(abs(mSignal[it].instances[it2].s.second - mSignal[it].instances[it3].s.second),2);
                    cout << temp_distance << "\n";
                    if((first == 0) && (temp_distance != 0)){
                        distance = temp_distance;
                        first = 1;
                        s = it2;
                        f = it3;
                        o = it;
                    } else if ((temp_distance < distance) && (temp_distance != 0)){
                        distance = temp_distance;
                        s = it2;
                        f = it3;
                        o = it;
                    }
                }
            }
        }
    }
    
    

    
    

    
    int stepCost, nc, dist;
    point neighbour;
    point neighbours[8];
    neighbours[0] = point( -1, -1 ); 
    neighbours[1] = point(  1, -1 );
    neighbours[2] = point( -1,  1 ); 
    neighbours[3] = point(  1,  1 );
    neighbours[4] = point(  0, -1 ); 
    neighbours[5] = point( -1,  0 );
    neighbours[6] = point(  0,  1 ); 
    neighbours[7] = point(  1,  0 );
    
    bool resultSearch;
    bool resultFillOpen;
    
    //node start.pos(mSignal[o].instances[s].s.first, mSignal[o].instances[s].s.second);
    //node finish.pos(mSignal[o].instances[f].s.first, mSignal[o].instances[f].s.second);
    node start, finish;
    start.pos.x = mSignal[o].instances[s].s.first;
    start.pos.y = mSignal[o].instances[s].s.second;
    
    finish.pos.x = mSignal[o].instances[f].s.first;
    finish.pos.y = mSignal[o].instances[f].s.second;
    cout << "start x: " << start.pos.x << " start y: " << start.pos.y << "\n"; 
    cout << "finish x: " << finish.pos.x << " finish y: " << finish.pos.y << "\n"; 
    
    start.f = start.g = start.h = 0;
    finish.f = finish.g = finish.h = 0;
    
    std::list<node> open, closed;
    
    open.push_back(start);
    node current_node;
    node temp;
    int current_index;
    int temp_it=0;
    while(!open.empty()){
       current_node = open.front();
       current_index = 0;
       std::list<node>::iterator it2;
       
       temp_it=0;
       for(it2=open.begin();it2 != open.end();it2++){
           if(it2->f < current_node.f){
               current_node.f = it2->f;
               current_node.g = it2->g;
               current_node.h = it2->h;
               current_node.parent = it2->parent;
               current_node.pos = it2->pos;
               current_index = temp_it;
           }
       temp_it++;
       }
       
       
       open.erase(std::find(open.begin(), open.end(), current_node));
       closed.push_back(current_node);
       
       if(current_node == finish){
           list<point> path;
           point current = current_node.pos;
           while ((current.x != start.pos.x) && (current.y != start.pos.y)){
               path.push_back(current);
               current = current_node.parent;
           }
           
       } else {
           vector<point> surr;
           point temp;
           temp.x = 0;
           temp.y = -1;
           surr.push_back(temp);
           temp.x = 0;
           temp.y = 1;
           surr.push_back(temp);
           temp.x = -1;
           temp.y = 0;
           surr.push_back(temp);
           temp.x = 1;
           temp.y = 0;
           surr.push_back(temp);
           
           vector<point> node_position;
           list<node> children;
           node temp_node;
           for(int it=0;it<surr.size();it++){
               temp.x = surr[it].x + current_node.pos.x;
               temp.y = surr[it].y + current_node.pos.y;
               temp_node.pos.x = temp.x;
               temp_node.pos.y = temp.y;
               temp_node.parent.x = current_node.pos.x;
               temp_node.parent.y = current_node.pos.y;
               children.push_back(temp_node);
               //node_position.push_back(temp);   
           }
           for(int it=0;it<children.size();it++){
               for(int it2=0;it2<closed.size();it2++){
                   if(children[it] == closed[it2]){
                       continue;
                   }
               }
           
               children[it].
           }
           
           
       }
       
    }
    
    
    
    
    
    
    
    
    
    
    
    
    
    return 0;
}