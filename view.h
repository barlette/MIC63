/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   view.h
 * Author: bar
 *
 * Created on July 29, 2019, 10:12 PM
 */

#ifndef VIEW_H
#define VIEW_H

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

int renderizeMatrix(vector<int> p_pos, vector<Net> netlist_p, vector<int> n_pos, vector<Net> netlist_n, int nfin, int ntracks);

typedef pair<int, int> coord;

struct point2{
    point2(int a, int b){ s.first = a; s.second = b;}
    coord s;
};

struct signal{
    vector <point2> instances;
    vector <int> visited;
};
#endif /* VIEW_H */

