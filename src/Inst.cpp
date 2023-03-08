#include "Inst.h"

Inst::Inst(std::string code, int size){
    this->size = size;
    this->code = code;
}

std::string Inst::getCode()
{
    return code;
}

int Inst::getSize()
{
    return size;
}

