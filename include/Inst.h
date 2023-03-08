#pragma once
#include <string>

class Inst
{
public:
    Inst(std::string, int);
    std::string getCode();
    int getSize();

protected:
    std::string code;
    int size;
};