#include "pre.h"
#include "file.h"
#include "macro.h"
#include <fstream>
#include <map>
#include <iostream>
#include <vector>

int whichPreToken(std::string token) 
{
    if(!token.compare("MACRO"))
        return MACRO;
    else if(!token.compare("EQU"))
        return EQU;
    else if(!token.compare("IF"))
        return IF;
    return 0;
}

bool pre(std::string fileName, std::string name ) 
{
    int stage = PRE_SECTION; 
    std::ifstream file;
    std::ofstream preFile;
    std::string line;

    file.open(fileName);
    if(!file.is_open()) 
    {
        std::cout << "Error! CANNOT OPEN " << fileName << std::endl;
        return false;
    }

    preFile.open(name +".pre");
    if(!preFile.is_open())
    {
        std::cout << "Error! CANNOT CREATE " << name +".pre" <<std::endl;
        return false;
    }

    std::vector<std::string> tokens;

    std::map<std::string, int> table_mnt;
    std::map<std::string, std::vector<std::string>> table_mdt;

    std::map<std::string,std::string> table_equ;

    while(!file.eof())
    {
        tokens = nextLine(file);
        if(file.eof())
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
                #if DEBUG
                std::cout << "Found SECTION TEXT\n";
                #endif

                stage = SECTION_TEXT;
                preFile << newLine(tokens, LINE_BREAK);
                continue;
            }

            if(tokens[0].rfind(":") != std::string::npos)
            {
                std::string label = tokens[0].substr(0, tokens[0].size()-1);

                #if DEBUG
                std::cout << "This token is a label -> "<< label <<std::endl;
                #endif

                std::vector<std::string> fullInst;
                switch(whichPreToken(tokens[1]))
                {
                    case IF:
                        std::cout << "ERROR!  \""<< newLine(tokens, 0) << "\" IFs must be in a SECTION\n";
                        break;
                    case EQU:
                        table_equ.insert(std::pair<std::string,std::string>(label, tokens[2]));
                        break;
                    case MACRO:
                        std::cout << "ERROR! \""<< newLine(tokens,0) << "\" MACROs must be at SECTION TEXT!\n";
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
                #if DEBUG
                std::cout << "Found SECTION DATA\n";
                #endif

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

            if(tokens[0].rfind(":") != std::string::npos)
            {
                std::string label = tokens[0].substr(0, tokens[0].size()-1);

                #if DEBUG
                std::cout << "This token is a label -> "<< label <<std::endl;
                #endif
                
                std::vector<std::string> fullInst;
                switch(whichPreToken(tokens[1]))
                {
                    case IF:    
                        std::cout << "ERROR! It is not possible to have IF at \""<< newLine(tokens, 0) << "\" \n";
                        break;
                    case EQU:
                        std::cout << "ERROR! \""<< newLine(tokens, 0) << "\" EQUs must be outside the SECTIONS\n";
                        break;
                    case MACRO:
                        #if DEBUG
                        std::cout << "MACRO found! Initializing " << label << std::endl;
                        #endif

                        initializeMacro(file, tokens, label, table_mnt, table_mdt);

                        #if DEBUG
                        std::cout << "Finished " << label << std::endl;
                        #endif

                        break;
                    default:
                        if (table_mnt.count(tokens[1]))
                        {
                            #if DEBUG
                            std::cout << "Running macro " << tokens[1] << std::endl;
                            #endif

                            runMacro(preFile, tokens, table_mnt, table_mdt);
                        }
                        else
                            preFile << newLine(tokens, LINE_BREAK);
                        break;
                }
                continue;
                
            }
            else if(table_mnt.count(tokens[0]) )
            {

                #if DEBUG
                std::cout << "Running macro " << tokens[0] << std::endl;
                #endif
                runMacro(preFile, tokens, table_mnt, table_mdt);
                continue;
            }

            preFile << newLine(tokens, LINE_BREAK);
        }
        else if (stage == SECTION_DATA)
            preFile << newLine(tokens, LINE_BREAK);

        #if DEBUG
        std::cout << "Line > /";
        for (u_int i = 0; i < tokens.size() ; i ++)
            std::cout << tokens[i] << '/';
        std::cout << std::endl;
        #endif
    }

    file.close();
    preFile.close();
    return true;
}