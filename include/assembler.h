#pragma once

#include <map>
#include <string>
#include "Inst.h"

#define FIRST_STEP   1
#define SECOND_STEP  2

using std::string;
using std::map;

void initializeInsts(map<string, Inst>&);
bool assembler(string, string);
