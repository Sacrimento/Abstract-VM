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
    
    std::list<Parser::token>   parse(int argc, char **argv, bool &error);

    std::vector<std::string> const allowedCmds{"pop", "dump", "add", "sub", "mul", "div", "mod", "xor", "and", "or", "max", "min", "avg", "print", "exit"};
    std::vector<std::string> const allowedTypes{"int8", "int16", "int32", "float", "double"};
    std::vector<std::string> const allowedCmdsWArgs{"assert", "push"};

    class ParserException : public std::invalid_argument
    {
        public:
                ParserException() : std::invalid_argument("invalid argument") {}
                ParserException(ParserException const & src) : std::invalid_argument("invalid argument") { *this = src; }
                ParserException &operator=(ParserException const &rhs) { std::invalid_argument::operator=(rhs); return *this; }
                ~ParserException() throw() {}
                ParserException(const std::string& what) : std::invalid_argument(what) {}
    };
}

#endif