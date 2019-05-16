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

using namespace std;

int int_ext(std::string input){

    std::string output = std::regex_replace(
        input,
        std::regex("[^0-9.,]+"),
        std::string("")
        );
    return (std::stoi(output));
}

void print_list(list<Transistor> ts){
    std::list<Transistor>::iterator it;
    for(it = ts.begin(); it != ts.end(); it++){
        cout << it->get_type() << " " << it->get_drain() << " " << it->get_gate() << " " << it->get_source() << " " << it->get_bgate() << " " << it->get_width() <<  " " << it->get_length() << " " << it->get_nfin() << " \n";
    }
}

int main(int argc, char *argv[])
{
    string line, token, delimiter = " ";
    string transistor_temp[8];
    size_t pos;
    int curLine = 0;
    std::string file_name = "default";
    //vector <Transistor> c;
    Transistor temp(" "," "," "," ",' ',0,0,0);
    list<Transistor> list_transistors;
    if (argc > 1) { file_name = argv[1]; }
    //cout << file_name + "\n";
    ifstream inFile(file_name.c_str() );
    
    
    if(!inFile)
    {
        cerr 	<< "unable to open input file: " << file_name;
	return -1;
    }

    while(!inFile.eof())
    {
	curLine++;
	getline(inFile, line);
	if((line.find(sub_circuit_start)) != -1) // se encontrar a string .SUBCKT
	{
            while((line.find(sub_circuit_end)) == -1) // enquanto não encontrar a string .ENDS
            {
                getline(inFile, line);
                while((line.find(transistor, 0)) != std::string::npos) // enquanto encontrar transistores e.g. substring MM
                {
                    int nElement = 0;
                    while (((pos = line.find(delimiter)) != std::string::npos)) // enquanto houver elementos na instanciação do transistor
                    {
                        if(pos != 0){ // para não contar espaços extras entre os elementos da instanciação
                        token = line.substr(0, pos);
                        if(nElement != 0) // para não contar o MM%
                        {
                            transistor_temp[nElement-1] = token;
                        }
                        nElement++;
                        }
                        line.erase(0, pos + delimiter.length()); 
                    }
                    if(nElement != 8){ // caso não haja todos os elementos necessários ou haja elementos a mais e.g. MM%, SOURCE, GATE, DRAIN, BACK GATE, WIDTH, LENGTH, NUMBER OF FINS <- 8 elementos
                        cerr 	<< "invalid transistor instance at line: " << curLine << ":" << file_name;
                        return -1;
                    } else{
                        temp.set_drain(transistor_temp[0]);
                        temp.set_gate(transistor_temp[1]);
                        temp.set_source(transistor_temp[2]);
                        temp.set_bgate(transistor_temp[3]);
                        if(!transistor_temp[4].compare("nmos_rvt"))
                        {
                            temp.set_type('N');
                        } else if(!transistor_temp[4].compare("pmos_rvt"))
                        {
                            temp.set_type('P');
                        } else 
                        {
                            cerr 	<< "invalid transistor instance\n";
                            return -1;
                        }
                        temp.set_width(int_ext(transistor_temp[5]));
                        temp.set_length(int_ext(transistor_temp[6]));
                        temp.set_nfin(int_ext(line));
                    }
                    getline(inFile, line);
                    list_transistors.push_back(temp); // insere numa lista
                }
            }
        }
    } 
    print_list(list_transistors);
}
