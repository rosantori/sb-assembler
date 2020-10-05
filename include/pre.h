/* PRE-PROCESSING STAGE -> Open the original file, creates the file.PRE and closes the original file
    1 - Ignore comments, tabs,  extra spaces and extra line breaks
    2 - Run Directives IF and EQU -> IF needs EQU before, EQU does not need IF
    3 - Expand MACROS
*/
#pragma once

#define PRE_SECTION     1
#define SECTION_DATA    2
#define SECTION_TEXT    3

#define MACRO       4
#define EQU         5
#define IF          6
#define LABEL       7
#define ENDMACRO    8

#define LINE_BREAK  true

#define FALSE 0

#include <string>

using std::string;

int whichPreToken(string); 

bool pre(string);