/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


#include <fstream>
#include <iomanip>
#include <iostream>
#include "../include/parser.h"
#include "../include/circuit.h"
#include <vector>
using namespace std;

int main(int argc, char *argv[])
{
	string line, token, delimiter = " ";
	size_t pos;
	int curLine = 0, nTransistor=0;
	std::string file_name = "default";
	vector <Transistor> c;
	Transistor temp(" "," "," "," ",' ',0,0,0);

	if (argc > 1) { file_name = argv[1]; }
	cout << file_name + "\n";
	ifstream inFile(file_name.c_str() );

	if(!inFile)
	{
		cerr 	<< "unable to open input file: "
			<< file_name;
		return -1;
	}

	char ch;
	while(!inFile.eof())
	{
		curLine++;
		getline(inFile, line);
		if((line.find(sub_circuit_start, 0)) != string::npos)
		{
			getline(inFile, line);
			while((line.find(sub_circuit_end)) == -1)
			{
				getline(inFile, line);
				cout << line << "\n";
				while((line.find(transistor)) != -1)
				{
					nTransistor++;
					cout << "Transistor: " << nTransistor << "\n";
					int nElement = 0;
					while ((pos = line.find(delimiter)) != std::string::npos)
					{
						token = line.substr(0, pos);
						std::cout << token << std::endl;


						if(nElement != 0){
							switch(nElement){
								case 1:
									temp.set_drain(token);
									break;
								case 2:
									temp.set_gate(token);
									break;
								case 3:
									temp.set_source(token);
									break;
								case 4:
									temp.set_bgate(token);
									break;
								case 5:
									if(!token.compare("nmos_rvt")){
										temp.set_type('N');
									} else temp.set_type('P');
									break;
							}

						}
						line.erase(0, pos + delimiter.length());
						nElement++;
					}
					std::cout << line << std::endl;
					getline(inFile, line);
				}
				cout << "found: " << sub_circuit_start << "line: " << curLine << endl;
				}
		}
	}

	cout << temp.get_type() << " " << temp.get_bgate() << " " << temp.get_gate() << " " << temp.get_source() << " " << temp.get_drain() << " " << "\n";
}

