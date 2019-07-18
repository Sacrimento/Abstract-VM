#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include <vector>
#include <algorithm>
#include <regex>
#include "AVM.h"

namespace Parser
{
    struct token
    {
        std::string     func;
        std::string     value;
        int             line;
    };
    
    std::list<Parser::token>   parseFile(char *file);

    std::vector<std::string> const allowedCmds{"pop", "dump", "add", "sub", "mul", "div", "mod", "print"};
    std::vector<std::string> const allowedConstructor{"int8", "int16", "int32", "float", "double"};
}

#endif