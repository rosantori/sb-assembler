#include "Inst.h"

Inst::Inst(string code, int size){
    this->size = size;
    this->code = code;
}

string Inst::getCode()
{
    return code;
}

int Inst::getSize()
{
    return size;
}

