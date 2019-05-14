/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   circuit.h
 * Author: bar
 *
 * Created on May 14, 2019, 3:40 PM
 */

#ifndef CIRCUIT_H
#define CIRCUIT_H

#include<iostream>
#include<iomanip>
#include<fstream>
#include <vector>
#include <string>
using namespace std;

class Transistor
{
private:
	char type;
	string drain, source, gate, bgate;
	int width, length, nfin;
        
public:
	Transistor(string drain, string gate, string source, string bgate, char type, int width, int length, int nfin){
		this->drain = drain;
		this->source = source;
		this->gate = gate;
		this->bgate = bgate;
		this->type = type;
		this->width = width;
		this->length = length;
		this->nfin = nfin;
	}
        
	void set_type(char type){
            this->type = type;
	}
	void set_drain(string drain){
            this->drain = drain;
	}
	void set_gate(string gate){
            this->gate = gate;
	}
	void set_source(string source){
            this->source = source;
	}
	void set_bgate(string bgate){
            this->bgate = bgate;
	}
        void set_width(int width){
            this->width = width;
        }
        void set_length(int length){
            this->length = length;
        }
        void set_nfin(int nfin){
            this->nfin = nfin;
        }  
        
	char get_type(){
		return this->type;
	}
	string get_drain(){
		return this->drain;
	}
	string get_gate(){
		return this->gate;
	}
	string get_bgate(){
		return this->bgate;
	}
	string get_source(){
		return this->source;
	}
        int get_width(){
            return this->width;
        }
        int get_length(){
            return this->length;
        }
        int get_nfin(){
            return this->nfin;
        }  
};

#endif /* CIRCUIT_H */

