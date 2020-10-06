#pragma once

#include <string>
#include <vector>
#include <fstream>

using std::vector;
using std::string;
using std::ifstream;

//  Returns the tokens of a line
vector<string> checkLine(string);

//  Returns the tokens of the next line of the opened file
vector<string> nextLine(ifstream&);

//  Turn the tokens into a line again with the correct spaces, the second arg is true for appending 
//    \n' and false for not appending the '\n'
string newLine(vector <string>, bool);
