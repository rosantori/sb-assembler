/* PRE-PROCESSING STAGE -> Open the original file, creates the file.PRE and closes the original file
    1 - Ignore comments, tabs,  extra spaces and extra line breaks
    2 - Run Directives IF and EQU -> IF needs EQU before, EQU does not need IF
    3 - Expand MACROS
*/
#pragma once
#include <string>

enum 
{
    LINE_BREAK = 1,
    PRE_SECTION, SECTION_DATA, SECTION_TEXT,
    MACRO, EQU, IF, LABEL, ENDMACRO,
};


//  Returns if the token is a token of pre process stage
int whichPreToken(std::string); 

// Pre processes the file .asm, returns true for success and false otherwise
bool pre(std::string, std::string);