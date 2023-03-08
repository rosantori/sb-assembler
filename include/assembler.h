#pragma once

#include <map>
#include <string>
#include "Inst.h"

#define FIRST_STEP   1
#define SECOND_STEP  2

void initializeInsts(std::map<std::string, Inst>&);
bool assembler(std::string, std::string);
