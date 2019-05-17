/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   grapher.h
 * Author: brltt
 *
 * Created on May 16, 2019, 8:56 PM
 */

#ifndef GRAPHER_H
#define GRAPHER_H

#include <fstream>
#include <iomanip>
#include <iostream>
#include "parser.h"
#include "circuit.h"
#include "stack.h"
#include <vector>
#include <string>
#include <regex>
#include <list>
#include <iterator>

Stack divide_into_stacks(list<Transistor> ts);
vector<string> indexNetlist (list<Transistor> ts);
int findIndex(vector<string> adj, string u);
vector<int> grapher(list<Transistor> stack);
vector<int> addEdge(vector<int> adj, string u, string v, vector<string> netlist);
#endif /* GRAPHER_H */

