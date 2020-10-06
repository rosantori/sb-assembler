#pragma once

#include <string>

using std::string;

class Inst
{
    public:
        Inst(string, int);
        string getCode();
        int getSize();

    protected:
        string code;
        int size;
};