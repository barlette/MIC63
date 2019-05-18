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
#include <list>
#include <iterator>
#include "grapher.h"

list<Transistor> nStack(list<Transistor> ts);
list<Transistor> pStack(list<Transistor> ts);
vector<string> indexNetlist (list<Transistor> ts);
//int findIndex(vector<string> netlist, string u);

#endif /* STACK_H */

