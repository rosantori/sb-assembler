#pragma once

#include <string>
#include <vector>
#include <fstream>

//  Returns the tokens of a line
std::vector<std::string> checkLine(std::string);

//  Returns the tokens of the next line of the opened file
std::vector<std::string> nextLine(std::ifstream&);

//  Turn the tokens into a line again with the correct spaces, the second arg is true for appending 
//    \n' and false for not appending the '\n'
std::string newLine(std::vector <std::string>, bool);
