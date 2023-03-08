#include "macro.h"
#include "file.h"
#include "pre.h"
#include <iostream>
#include <list>

void initializeMacro(std::ifstream& file, std::vector<std::string> tokens, std::string name, std::map<std::string,int> &table_mnt, std::map<std::string,std::vector<std::string>> &table_mdt)
{
    std::vector<std::string> fullInst;
    std::map<std::string, std::string> argsMacro;
    //flag args is set to tell if the args are valid
    bool args = true;

    //Delete the name of the macro and "MACRO" of the line
    tokens.erase(tokens.begin());
    tokens.erase(tokens.begin());
    
    //Check if there are invalid macro arguments
    if(tokens.size())
    {
        for(u_int i=0; i<tokens.size(); i++)
        {
            if(tokens[i].rfind(',') != std::string::npos)
                tokens[i] = tokens[i].substr(0, tokens[i].size()-1);
            //Quick check if they really are arguments
            if(tokens[i].at(0) != '&' || tokens[i].size() < 2)
            {    
                std::cout << "ERROR! token: \""<< tokens[i]<< "\" na linha \"" << newLine(tokens, 0) <<"\" deve ser argumento de macro\n";
                args=false;
                continue;
            }
            argsMacro.insert(std::pair<std::string,std::string>(tokens[i], std::to_string(i)));
        }
    }
    
    if(!args)
        std::cout << "Há argumentos inválidos na MACRO: \"" << name <<"\" .\n";
    else
        table_mnt.insert(std::pair<std::string,int>(name, tokens.size()));

    while(!file.eof())
    {
        tokens = nextLine(file);
        if(!tokens[0].compare("ENDMACRO"))
            break;

        for(u_int i=0; i < tokens.size(); i++)
        {
            if(tokens[i].rfind(',') != std::string::npos)
                tokens[i] = tokens[i].substr(0, tokens[i].size()-1);
            if(argsMacro.count(tokens[i]))
                tokens[i] = "#"+ argsMacro.at(tokens[i]);
        }
        fullInst.emplace_back(newLine(tokens,0));

        #if DEBUG
        std::cout << newLine(tokens, LINE_BREAK);
        #endif

    }
    
    table_mdt.insert(std::pair<std::string, std::vector<std::string>>(name, fullInst));
   
    return;

}

void runMacro(std::ofstream& preFile, std::vector<std::string> tokens, std::map<std::string, int>& table_mnt, std::map<std::string,std::vector<std::string>>& table_mdt)
{
    //If the first token is not the name of the macro, it must be a label and the name of the macro is the second token
    if (!table_mnt.count(tokens[0]))
    {
        preFile << tokens[0]+" ";
        tokens.erase(tokens.begin());
    }

    std::string name = tokens[0];
    tokens.erase(tokens.begin());
    int size = tokens.size();

    if( size != table_mnt.at(name))
    {
        std::cout << "Incorrect number of MACRO arguments" << std::endl;
        return;
    }

    std::vector<std::string> args;

    if(!tokens.empty())
        for (u_int i = 0; i <tokens.size(); i ++)
        {
            if(tokens[i].rfind(',') != std::string::npos)
                tokens[i] = tokens[i].substr(0, tokens[i].size() - 1);
            args.emplace_back(tokens[i]);
        }
    
    std::vector<std::string> fullInst = table_mdt.at(name);
    for(u_int i = 0; i < fullInst.size(); i++)
    {
        tokens = checkLine(fullInst[i]);
        for(u_int j = 0; j < tokens.size(); j++)
        {
            if(tokens[0].rfind(':') != std::string::npos)
            {
                preFile << tokens[0]+" ";
                tokens.erase(tokens.begin());
                j=0;
            }
            if (tokens[j][0] == '#')
                tokens[j] = args[stoi(tokens[j].substr(1, tokens[j].size()-1))];

            if( j!= 0 && j < tokens.size()-1 )
               tokens[j] += ",";
        }
        preFile << newLine(tokens, LINE_BREAK);
    }
    return;
}
