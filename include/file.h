#pragma once

#include <string>
#include <vector>
#include <fstream>

using std::vector;
using std::string;
using std::ifstream;

// Separates the tokens of a line
vector<string> checkLine(string);
// Returns the tokens of the next line of the opened file
vector<string> nextLine(ifstream&);
// Turn the tokens into a line again with the correct spaces
string newLine(vector <string>, bool);
