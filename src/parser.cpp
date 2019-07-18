#include "../inc/AVM.h"

namespace Parser
{
    void    checkSyntax(std::list<std::string> & errors, int const & lineCount, std::string & line, std::list<Parser::token> & tokens, bool & exit)
    {
        std::string     leftSide;
        std::string     rightSide;
        size_t          pos1;
        size_t          pos2;

        if (line.back() == '\r' || line.back() == '\n')
            line.pop_back();

        if (line == "exit")
        {
            exit = true;
            return;
        }

        if (std::find(Parser::allowedCmds.begin(), Parser::allowedCmds.end(), line) != Parser::allowedCmds.end())
            tokens.push_back({line, "", lineCount});
        else if ((pos1 = line.find(' ')) != std::string::npos)
        {
            leftSide = line.substr(0, pos1);
            rightSide = line.substr(pos1 + 1);

            if (leftSide != "push" && leftSide != "assert")
                errors.push_back("Line " + std::to_string(lineCount) + ": unknown statement \"" + leftSide + "\"\n");

            if ((pos1 = rightSide.find('(')) == std::string::npos || (pos2 = rightSide.find(')')) == std::string::npos || pos1 > pos2 || pos2 != rightSide.size() - 1 ||
                std::find(Parser::allowedConstructor.begin(), Parser::allowedConstructor.end(), rightSide.substr(0, pos1)) == Parser::allowedConstructor.end() ||
                !std::regex_match(rightSide.substr(pos1 + 1, pos2 - pos1 - 1), std::regex((rightSide.substr(0, 3) == "int" ? "-?[[:d:]]+" : "-?[[:d:]]+.[[:d:]]+"))))
                errors.push_back("Line " + std::to_string(lineCount) + ": unknown statement \"" + rightSide + "\"\n");

            tokens.push_back({rightSide.substr(0, pos1), rightSide.substr(pos1 + 1, pos2 - pos1 - 1), lineCount});

            if (leftSide == "assert")
                tokens.push_back({"assert", "", lineCount});
        }
        else
            errors.push_back("Line " + std::to_string(lineCount) + ": unknown statement \"" + line + "\"\n");
    }

    std::list<Parser::token>   parseFile(char *file)
    {
        std::ifstream   f(file);
        std::string     line;
        std::list<std::string>  errors;
        std::list<Parser::token>    tokens;
        int             lineCount = 0;
        bool            exit = false;
        size_t          foundComment;

        if (!f.good())
            return tokens; 
        while (getline(f, line) && !exit)
        {
            ++lineCount;
            foundComment = line.find(';');
            if (foundComment != std::string::npos)
                line = line.substr(0, foundComment);
            if (!line.empty() && line[0] != '\r' && line[0] != '\n')
                Parser::checkSyntax(errors, lineCount, line, tokens, exit);
        }
        f.close();
        if (!exit)
            errors.push_back("Line " + std::to_string(lineCount) + ": \"exit\" instruction not found\n");
        for (auto e : errors)
            std::cerr << e;
        return (errors.empty() ? tokens : std::list<Parser::token>());
    }

}