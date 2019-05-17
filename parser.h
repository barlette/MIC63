/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   newfile.h
 * Author: bar
 *
 * Created on May 14, 2019, 3:38 PM
 */

#ifndef PARSER_H
#define PARSER_H

#define sub_circuit_start ".SUBCKT"
#define transistor "MM"
#define sub_circuit_end ".ENDS"
#define w "w="
#define l "l="
#define fin "nfin="
#include <list>
#include "circuit.h"


int int_ext(std::string input);
void print_list(list<Transistor> ts);
list<Transistor> parser(string fileName);

#endif /* NEWFILE_H */

