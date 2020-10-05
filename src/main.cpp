#include <iostream>
#include <cstring>
#include "pre.h"

using std::cout;
using std::endl;

int main(int argc, char *argv[]) {

    //Check if terminal entry is  OK
    if( argc != 3 ||
        argv[1][0] != '-' ||
        !(argv[1][1]=='p' || argv[1][1] == 'o')  ||
        argv[1][2] != '\0') 
    {
        cout << "ERROR! ENTRY MUST BE: ./montador (-p || -o) name_of_file.asm"<< endl;
        return 1;   
    }

    string fileName(argv[2]);
    string name="";

    for(u_int i = 0; i <fileName.rfind("."); i++)
    {
        if (fileName.at(i) == '.')
            break;
        name+=fileName.at(i);
    }

    if (pre(name)) cout << "PRE OKAY!\n";
    if (argv[1][1]=='o'); //COMPILA -> Deleta o .pre depois e deixa só o .obj
    return 0;
}

