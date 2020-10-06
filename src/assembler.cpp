#include "assembler.h"
#include "file.h"

#include <fstream>
#include <iostream>
#include <vector>

using std::ofstream;
using std::cout;
using std::endl;
using std::vector;

void initializeInsts(map <string, Inst > &table_insts)
{
    table_insts.insert(std::pair<string, Inst> ( "ADD", Inst("1", 2)));
    table_insts.insert(std::pair<string, Inst> ( "SUB", Inst( "2", 2 )));
    table_insts.insert(std::pair<string, Inst> ( "MULT", Inst( "3", 2 )));
    table_insts.insert(std::pair<string, Inst> ( "DIV", Inst( "4", 2 )));
    table_insts.insert(std::pair<string, Inst> ( "JMP", Inst( "5", 2 )));
    table_insts.insert(std::pair<string, Inst> ( "JMPN", Inst( "6", 2 )));
    table_insts.insert(std::pair<string, Inst> ( "JMPP", Inst( "7", 2 )));
    table_insts.insert(std::pair<string, Inst> ( "JMPZ", Inst( "8", 2 )));
    table_insts.insert(std::pair<string, Inst> ( "COPY", Inst( "9", 3 )));
    table_insts.insert(std::pair<string, Inst> ( "LOAD", Inst( "10", 2 )));
    table_insts.insert(std::pair<string, Inst> ( "STORE", Inst( "11", 2 )));
    table_insts.insert(std::pair<string, Inst> ( "INPUT", Inst( "12", 2 )));
    table_insts.insert(std::pair<string, Inst> ( "OUTPUT", Inst( "13", 2 )));
    table_insts.insert(std::pair<string, Inst> ( "STOP", Inst( "14", 1 )));
    return;
}

bool assembler(string fileName)
{
    ifstream file;
    ofstream objFile;

    file.open(fileName+".pre");
    if(!file.is_open()) 
    {
        cout << "Error! CANNOT OPEN " << fileName+".pre" << endl;
        return false;
    }

    u_int addr = 0;
    vector<string> tokens;

    map<string, Inst> table_insts;
    initializeInsts(table_insts);

    map<string, int> table_labels;

    //  The first line will always be "SECTION TEXT", so it can be ignored
    tokens = nextLine(file);
    bool SECTION_DATA = false;

    while(!file.eof())
    {
        #if DEBUG
        cout << "The address is : " << addr << endl;
        #endif

        tokens = nextLine(file);
        if(file.eof())
            break;

        if(tokens[0].rfind(':') != string::npos)
        {
            #if DEBUG
            cout << tokens[0] << " is a label! Its address is : " << addr << endl;
            #endif

            table_labels.insert(std::pair<string, int>(tokens[0].substr(0, tokens[0].size()-1), addr));
            tokens.erase(tokens.begin());
        }
        if(!tokens[0].compare("SECTION"))
        {
            SECTION_DATA = true;
            continue;
        }

        if (!SECTION_DATA) 
            addr += (table_insts.at(tokens[0])).getSize();
        else 
            addr++;
        #if DEBUG
        cout << "LINE /" << newLine(tokens, 0) << "/" << endl;
        #endif
        
    }

    #if DEBUG
    cout << "First step completed." << endl;
    #endif

    //  Returning to the begin
    file.clear();
    file.seekg(0, std::ios::beg);
    SECTION_DATA = false;

    addr = 0;

    tokens = nextLine(file);

    int size;
    string code;

    objFile.open(fileName+".obj");
    if(!objFile.is_open())
    {
        cout << "Error! CANNOT CREATE " << fileName+".obj" <<endl;
        return false;
    }

    while(!file.eof())
    {
        tokens = nextLine(file);
        if(file.eof())
            break;
        
        if (tokens[0].rfind(':') != string::npos)
            tokens.erase(tokens.begin());

        if(!tokens[0].compare("SECTION"))
        {
            SECTION_DATA = true;
            continue;
        }

        if(table_insts.count(tokens[0]) && !SECTION_DATA)
        {
            #if DEBUG
            cout << newLine(tokens, 1);
            #endif
            code = (table_insts.at(tokens[0])).getCode();
            objFile <<  code + " ";

            size = (table_insts.at(tokens[0])).getSize();
            #if DEBUG
            cout << "The size of "<< tokens[0] << " is " << size << endl;
            #endif

            tokens.erase(tokens.begin());
            for ( int i = 0; i < size-1 ; i++)
            {
                if(tokens[i].rfind(',') != string::npos)
                    tokens[i] = tokens[i].substr(0, tokens[i].size()-1);

                tokens[i] = table_labels.at(tokens[i]);
                objFile << tokens[i] + " ";
            }
            continue;

        }

        if(SECTION_DATA)
        {
            if(!tokens[0].compare("SPACE"))
            {
                objFile << "0 ";
            } 
            else if( !tokens[0].compare("CONST"))
            {
                objFile << tokens[1]+" ";
            }
        }

    }

    #if DEBUG
    cout << "Second step completed." << endl;
    #endif

    objFile.close();
    file.close();
    return true;
}