/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   stack.h
 * Author: brltt
 *
 * Created on May 16, 2019, 8:43 PM
 */

#ifndef STACK_H
#define STACK_H

#include <fstream>
#include <iomanip>
#include <iostream>
#include "parser.h"
#include "circuit.h"
#include <vector>
#include <string>
#include <regex>
#include <iterator>
#include "grapher.h"

vector<Transistor> nStack(vector<Transistor> ts);
vector<Transistor> pStack(vector<Transistor> ts);
vector<Net> indexNetlist (vector<Transistor> ts);
//int findIndex(vector<string> netlist, string u);
int detectDuality(vector<Transistor> ns, vector<Transistor> ps);
#endif /* STACK_H */

