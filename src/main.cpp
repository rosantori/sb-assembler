#include <iostream>
#include <cstring>
#include "pre.h"
#include "assembler.h"

using std::string;
using std::cout;
using std::endl;

int main(int argc, char *argv[]) 
{

    //Check if terminal entry is  OK
    if (argc != 3)
    {
        cout << "ENTRY MUST BE \'./montador -p fileName.asm\' or \'./montador -o fileName.pre\'\n";
        return 1;
    }
    
    string mode(argv[1]);
    std::string fileName(argv[2]);
    
    if((mode.compare ("-p") && mode.compare("-o")) ||
        (!mode.compare("-p") && fileName.find(".asm") == string::npos && fileName.find(".ASM") == string::npos) || 
        (!mode.compare("-o") && fileName.find(".pre") == string::npos && fileName.find(".PRE") == string::npos) )
    {
        cout << "ENTRY MUST BE \'./montador -p fileName.asm\' or \'./montador -o fileName.pre\'\n";
        return 1;
    }

    string name = fileName.substr(0, fileName.size()-4);

    if(!mode.compare("-p") )
    {
        if (pre(fileName, name))
            cout << "File \'"+name+".pre\' created successfully.\n";
        else 
        {
            cout << "Failed to preprocess file.\n";
        }
    }
    else if (!mode.compare("-o"))
    {
        if(assembler(fileName, name))
            cout << "File \'"+name+".obj\' created successfully.\n";
        else 
        {
            cout << "Failed to assemble file.\n";
        }
    }
    return 0;
}

