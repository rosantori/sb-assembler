#include "pre.h"
#include "file.h"
#include "macro.h"
#include <fstream>
#include <map>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::ofstream;

int whichPreToken(string token) 
{
    if(!token.compare("MACRO"))
        return MACRO;
    else if(!token.compare("EQU"))
        return EQU;
    else if(!token.compare("IF"))
        return IF;
    return 0;
}

bool pre(string fileName) 
{
    int stage = PRE_SECTION; 
    ifstream file;
    ofstream preFile;
    string line;

    file.open(fileName+".asm");
    if(!file.is_open()) 
    {
        cout << "Error! CANNOT OPEN " << fileName+".asm" << endl;
        return false;
    }

    preFile.open(fileName+".pre");
    if(!preFile.is_open())
    {
        cout << "Error! CANNOT CREATE " << fileName+".pre" <<endl;
        return false;
    }

    vector<string> tokens;

    map<string, int> table_mnt;
    map<string, vector<string>> table_mdt;

    map<string,string> table_equ;

    while(!file.eof())
    {
        tokens = nextLine(file);
        if(!tokens.size())
            break;
        
        //Substitute the EQs
        if(stage != PRE_SECTION)
        { 
            for(u_int i = 0 ; i < tokens.size(); i++)
            {
                if(table_equ.count(tokens[i]))
                    tokens[i] = table_equ.at(tokens[i]);
            }

            if(!tokens[0].compare("IF"))
            {
                if (!stoi(tokens[1]))
                    tokens = nextLine(file);
                continue;    
            }

        }

        if (stage == PRE_SECTION) //Find directives MACRO and EQU and SECTIONs 
        {   
            if(!tokens[0].compare("SECTION")) // Finding SECTION TEXT
            {
                //cout << "AHCEIEIEIEIEEI SECTION!\n";
                stage = SECTION_TEXT;
                preFile << newLine(tokens, LINE_BREAK);
                continue;
            }

            if(tokens[0].rfind(":") != string::npos)
            {
                string label = "";
                for(u_int i=0; i < tokens[0].size()-1; i++)
                    label+=tokens[0].at(i);

                //cout << "ESSE TOKEN É UM RÓTULO!! -> "<< label <<endl;
                vector<string> fullInst;
                switch(whichPreToken(tokens[1]))
                {
                    case IF:
                        cout << "ERROR!  \""<< newLine(tokens, 0) << "\" IFs must be in a SECTION\n";
                        break;
                    case EQU:
                        table_equ.insert(std::pair<string,string>(label, tokens[2]));
                        break;
                    case MACRO:
                        cout << "ERROR! \""<< newLine(tokens,0) << "\" MACROs must be at SECTION TEXT!\n";
                        initializeMacro(file, tokens, label, table_mnt, table_mdt);
                        break;
                    default:
                        break;
                }
                
            }

        }
        else if (stage == SECTION_TEXT) //Expand MACROS and Find IFs
        {
            if(!tokens[0].compare("SECTION")) // Finding SECTION TEXT
            {
                //cout << "ACHEI SECTION!!\n";
                stage = SECTION_DATA;
                preFile << newLine(tokens, LINE_BREAK);
                continue;
            }
            //Substitute the EQs
            for(u_int i = 0 ; i < tokens.size(); i++)
            {
                if(table_equ.count(tokens[i]))
                    tokens[i] = table_equ.at(tokens[i]);
            }

            if(tokens[0].rfind(":") != string::npos)
            {
                string label = "";
                for(u_int i=0; i < tokens[0].size()-1; i++)
                    label+=tokens[0].at(i);

                //cout << "ESSE TOKEN É UM RÓTULO!! -> "<< label <<endl;
                vector<string> fullInst;
                switch(whichPreToken(tokens[1]))
                {
                    case IF:    
                        cout << "ERROR! It is not possible to have IF at \""<< newLine(tokens, 0) << "\" \n";
                        break;
                    case EQU:
                        cout << "ERROR! \""<< newLine(tokens, 0) << "\" EQUs must be outside the SECTIONS\n";
                        break;
                    case MACRO:
                        initializeMacro(file, tokens, label, table_mnt, table_mdt);
                        break;
                    default:
                        if (table_mnt.count(tokens[1]))
                            runMacro(preFile, tokens, table_mnt, table_mdt);
                        else
                            preFile << newLine(tokens, LINE_BREAK);
                        break;
                }
                continue;
                
            }
            else if(table_mnt.count(tokens[0]) )
            {
                runMacro(preFile, tokens, table_mnt, table_mdt);
                continue;
            }

            preFile << newLine(tokens, LINE_BREAK);
        }
        else if (stage == SECTION_DATA)
            preFile << newLine(tokens, LINE_BREAK);

        /*cout << "TOKENS> /";
        for (u_int i = 0; i < tokens.size() ; i ++)
            cout << tokens[i] << '/';
        cout << endl;*/
    }

    file.close();
    preFile.close();
    return true;
}