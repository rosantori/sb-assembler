#include "macro.h"
#include "file.h"
#include "pre.h"
#include <iostream>

using std::cout;
using std::endl;
using std::map;

void initializeMacro(ifstream& file, vector<string> tokens, string name, map<string,int> &table_mnt, map<string,vector<string>> &table_mdt)
{
    vector<string> fullInst;
    map<string, string> argsMacro;
    //flag args is to tell if the args are correct
    bool args = true;
    //Check if there are invalid macro arguments
    if(tokens.size()-2)
    {
        for(u_int i=0; i<tokens.size()-2; i++)
        {
            if(tokens[i+2].rfind(',') != string::npos)
                tokens[i+2] = tokens[i+2].substr(0, tokens[i+2].size()-1);
            //Quick check if they really are arguments
            if(tokens[i+2].at(0) != '&' || tokens[i+2].size() < 2)
            {    
                cout << "ERROR! token: \""<< tokens[i+2]<< "\" na linha \"" << newLine(tokens, 0) <<"\" deve ser argumento de macro\n";
                args=false;
                continue;
            }
            argsMacro.insert(std::pair<string,string>(tokens[i+2], std::to_string(i)));
        }
    }
    
    if(!args)
        cout << "Há argumentos inválidos na MACRO: \"" << name <<"\" .\n";
    else
        table_mnt.insert(std::pair<string,int>(name, tokens.size()-2));

    while(!file.eof())
    {
        tokens = nextLine(file);
        if(!tokens[0].compare("ENDMACRO"))
            break;

        for(u_int i=0; i < tokens.size(); i++)
            if(argsMacro.count(tokens[i]))
                tokens[i] = "#"+ argsMacro.at(tokens[i]);
        fullInst.emplace_back(newLine(tokens,0));
        cout << newLine(tokens, LINE_BREAK);

    }
    
    table_mdt.insert(std::pair<string, vector<string>>(name, fullInst));
   
    return;

}

void runMacro(ofstream& preFile, vector<string> tokens, map<string, int>& table_mnt, map<string,vector<string>>& table_mdt)
{
    //pos is the position of the name of the macro
    int pos = 0;
    //If the first token is not the name of the macro, it must be a label and the name of the macro is the second token
    if (!table_mnt.count(tokens[0]))
        pos++;
    if( (tokens.size()-(pos+1)) != table_mnt.at(tokens[pos]))
    {
        cout << "Incorrect number of MACRO arguments" << endl;
        return;
    }

    if(pos)
    {
        preFile << tokens[0]+" ";
        tokens.erase(tokens.begin());
    }

    vector<string> args;

    if(tokens.size() - 1)
        for (u_int i = 0; i <tokens.size()- 1; i ++)
        {
            if(tokens[i+1].rfind(',') != string::npos)
                tokens[i+1] = tokens[i+1].substr(0, tokens[i+1].size()-1);
            args.emplace_back(tokens[i + 1]);
        }
    
    vector<string> fullInst = table_mdt.at(tokens[0]);

    for(u_int i = 0; i < fullInst.size(); i++)
    {
        tokens = checkLine(fullInst[i]);
        for(u_int y = 0; y < tokens.size(); y++)
        {
            if (tokens[y].c_str()[0] == '#')
                tokens[y] = args[stoi(tokens[y].substr(1, tokens[y].size()-1))];
        }
        preFile << newLine(tokens, LINE_BREAK);
    }
    return;
}
