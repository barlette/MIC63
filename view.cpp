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
    
    int last_ppos, sxcoord = 42, sycoord = 48, salimcoord = 22, salimHeight = 108, sHeigth = 81, actWidth = 24, space = 5;
    gateWidth = 20;
    for(int it=0; it<new_ppos.size(); it++){
        cout << p_pos_types[it];
            if(p_pos_types[it] == GATE){
                for(int it2=sxcoord;it2<sxcoord+gateWidth;it2++){
                    for(int it3=43; it3<height-44;it3++){
                        SIGNAL[it3][it2] = new_ppos[it]+1;
                    }
                }
                sxcoord = sxcoord + 25;
            } else if((global[new_ppos[it]].name != global[new_ppos[it-1]].name) && (p_pos_types[it] != p_pos_types[it-1]) && (p_pos_types[it] == ACTIVE)/*&& (it != 0)*/){
                if(global[new_ppos[it]].name == "VDD") {
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
            } else if((global[new_ppos[it]].name != global[new_ppos[it-1]].name) && (p_pos_types[it] == p_pos_types[it-1])/*&& (it != 0)*/){
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
                sxcoord = sxcoord + 54;
            } else if((global[new_npos[it]].name != global[new_npos[it-1]].name) && (n_pos_types[it] == n_pos_types[it-1])/*&& (it != 0)*/){
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
    return 0;
}