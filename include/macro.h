#pragma once
#include <string>
#include <map>
#include <vector>
#include <fstream>

// Receive the macro and inserts it at table_mnt and table_mdt 
void initializeMacro(std::ifstream&, std::vector<std::string>, std::string, std::map<std::string,int>&, std::map<std::string,std::vector<std::string>>&);

// Expand macros and write preFile
void runMacro(std::ofstream&, std::vector<std::string>, std::map<std::string, int>&, std::map<std::string,std::vector<std::string>>&);

