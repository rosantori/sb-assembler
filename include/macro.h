#pragma once

#include <string>
#include <map>
#include <vector>
#include <fstream>

using std::vector;
using std::ifstream;
using std::map;
using std::string;
using std::ofstream;

/* Receive the macro and inserts it at table_mnt and table_mdt */
void initializeMacro(ifstream&, vector<string>, string, map<string,int>&, map<string,vector<string>>&);

void runMacro(ofstream&, vector<string>, map<string, int>&, map<string,vector<string>>&);

