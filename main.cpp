/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <fstream>
#include <iomanip>
#include <iostream>
#include "parser.h"
#include <vector>
#include <string>
#include <regex>
#include <list>
#include <iterator>
#include "placement.h"

int main(int argc, char *argv[])
{
        string fileName;
        
        if(argc > 1)
        { 
            fileName = argv[1];
        }
        
        place(fileName);
        return 0;
}