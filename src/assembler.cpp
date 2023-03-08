#include "assembler.h"
#include "file.h"

#include <fstream>
#include <iostream>
#include <vector>

using std::vector;

void initializeInsts(std::map <std::string, Inst > &table_insts)
{
    table_insts.insert(std::pair<std::string, Inst> ( "ADD", Inst("1", 2)));
    table_insts.insert(std::pair<std::string, Inst> ( "SUB", Inst( "2", 2 )));
    table_insts.insert(std::pair<std::string, Inst> ( "MULT", Inst( "3", 2 )));
    table_insts.insert(std::pair<std::string, Inst> ( "DIV", Inst( "4", 2 )));
    table_insts.insert(std::pair<std::string, Inst> ( "JMP", Inst( "5", 2 )));
    table_insts.insert(std::pair<std::string, Inst> ( "JMPN", Inst( "6", 2 )));
    table_insts.insert(std::pair<std::string, Inst> ( "JMPP", Inst( "7", 2 )));
    table_insts.insert(std::pair<std::string, Inst> ( "JMPZ", Inst( "8", 2 )));
    table_insts.insert(std::pair<std::string, Inst> ( "COPY", Inst( "9", 3 )));
    table_insts.insert(std::pair<std::string, Inst> ( "LOAD", Inst( "10", 2 )));
    table_insts.insert(std::pair<std::string, Inst> ( "STORE", Inst( "11", 2 )));
    table_insts.insert(std::pair<std::string, Inst> ( "INPUT", Inst( "12", 2 )));
    table_insts.insert(std::pair<std::string, Inst> ( "OUTPUT", Inst( "13", 2 )));
    table_insts.insert(std::pair<std::string, Inst> ( "STOP", Inst( "14", 1 )));
    return;
}

bool assembler (std::string fileName, std::string name)
{
    std::ifstream file;
    std::ofstream objFile;

    file.open(fileName);
    if(!file.is_open()) 
    {
        std::cout << "Error! CANNOT OPEN " << fileName << std::endl;
        return false;
    }

    u_int addr = 0;
    vector<std::string> tokens;

    std::map<std::string, Inst> table_insts;
    initializeInsts(table_insts);

    std::map<std::string, int> table_labels;

    //  The first line will always be "SECTION TEXT", so it can be ignored
    tokens = nextLine(file);
    bool SECTION_DATA = false;

    while(!file.eof())
    {
        #if DEBUG
        std::cout << "The address is : " << addr << std::endl;
        #endif

        tokens = nextLine(file);
        if(file.eof())
            break;

        if(tokens[0].rfind(':') != std::string::npos)
        {
            #if DEBUG
            std::cout << tokens[0] << " is a label! Its address is : " << addr << std::endl;
            #endif

            table_labels.insert(std::pair<std::string, int>(tokens[0].substr(0, tokens[0].size()-1), addr));
            tokens.erase(tokens.begin());
        }
        if(!tokens[0].compare("SECTION"))
        {
            SECTION_DATA = true;
            continue;
        }

        if (!SECTION_DATA)
        {
            if(table_insts.count(tokens[0]))
                addr += (table_insts.at(tokens[0])).getSize();
            else 
            std::cout << "The inst \"" << tokens[0] << "\" does not exist.\n";
        }
        else 
            addr++;
        #if DEBUG
        std::cout << "LINE /" << newLine(tokens, 0) << "/" << std::endl;
        #endif
        
    }

    #if DEBUG
    std::cout << "First step completed." << std::endl;
    #endif

    //  Returning to the begin
    file.clear();
    file.seekg(0, std::ios::beg);
    SECTION_DATA = false;

    addr = 0;

    tokens = nextLine(file);

    int size;
    std::string code;

    objFile.open(name+".obj");
    if(!objFile.is_open())
    {
        std::cout << "Error! CANNOT CREATE " << name+".obj" <<std::endl;
        return false;
    }

    while(!file.eof())
    {
        tokens = nextLine(file);
        if(file.eof())
            break;
        
        if (tokens[0].rfind(':') != std::string::npos)
            tokens.erase(tokens.begin());

        if(!tokens[0].compare("SECTION"))
        {
            SECTION_DATA = true;
            continue;
        }

        if(table_insts.count(tokens[0]) && !SECTION_DATA)
        {
            #if DEBUG
            std::cout << newLine(tokens, 1);
            #endif
            code = (table_insts.at(tokens[0])).getCode();
            objFile <<  code + " ";

            size = (table_insts.at(tokens[0])).getSize();
            #if DEBUG
            std::cout << "The size of "<< tokens[0] << " is " << size << std::endl;
            #endif

            tokens.erase(tokens.begin());
            for ( int i = 0; i < size-1 ; i++)
            {
                if(tokens[i].rfind(',') != std::string::npos)
                    tokens[i] = tokens[i].substr(0, tokens[i].size()-1);

                if (!table_labels.count(tokens[i]))
                {
                    std::cout << "The token \'"+tokens[i]+"\' is invalid.\n";
                    objFile.close();
                    file.close();
                    return false;
                }
                tokens[i] = std::to_string(table_labels.at(tokens[i]));
                #if DEBUG
                std::cout << "The token is : " << tokens[i] << std::endl;
                #endif
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
    std::cout << "Second step completed." << std::endl;
    #endif

    objFile.close();
    file.close();
    return true;
}