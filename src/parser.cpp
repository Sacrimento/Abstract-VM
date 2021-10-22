#include "../inc/AVM.h"

namespace Parser
{
    bool    isNumber(std::string &str)
    {
        auto it = str.begin();
        bool dot = false;

        if (str.empty())
            return false;

        if (*it == '+' || *it == '-')
            it++;
        if (*it == '.')
            str.insert(it, '0');
        while (it != str.end())
        {
            if (!std::isdigit(*it))
            {
                if (*it == '.')
                {
                    if (dot)
                        return false;
                    else
                        dot = true;
                }
                else
                    return false;
            }
            it++;
        }

        if (*str.rbegin() == '.')
            return false;

        return true;
    }

    bool    inAllowed(std::string token, std::vector<std::string> allowed) {
        return std::find(allowed.begin(), allowed.end(), token) != allowed.end();
    }

    std::vector<std::string>  split(std::string line, char delimiter) {
        size_t  pos = 0;
        std::vector<std::string>  ret;
        std::string s;

        while ((pos = line.find(delimiter)) != std::string::npos) {
            s = line.substr(0, pos);
            if (!s.empty())
                ret.push_back(s);
            line.erase(0, pos + 1);
        }
        ret.push_back(line);

        return ret;
    }

    std::vector<std::string>    splitCmdWArgs(std::string token) {
        size_t pos1, pos2 = 0;
        std::vector<std::string>    ret;
        std::string s;

        if ((pos1 = token.find('(')) == std::string::npos || (pos2 = token.find(')')) == std::string::npos)
            throw ParserException("missing parenthese");

        if (pos2 < pos1)
            throw ParserException("parenthese error");

        ret.push_back(token.substr(0, pos1));
        s = token.substr(pos1 + 1, pos2 - pos1 - 1);

        if (!isNumber(s))
            throw ParserException("not a number");

        ret.push_back(s);

        s = token.substr(pos2 + 1, token.size());
        if (!s.empty())
            throw ParserException(std::string("\"" + s + "\" : unknow statement"));

        return ret;
    }

    std::list<Parser::token>   getToken(std::string & line, int lineIdx)
    {
        std::vector<std::string>    lineTokens;
        std::vector<std::string>    cmdWArgs;
        std::list<Parser::token>    ret;

        if (line.back() == '\r' || line.back() == '\n')
            line.pop_back();

        lineTokens = split(line, ' ');
        
        if (lineTokens.size() == 0)
            return ret;
        if (lineTokens.size() > 2)
            throw ParserException(std::string("\"" + line + "\": unknown statement"));
        
        if (inAllowed(line, Parser::allowedCmds))
            ret.push_back({line, "", lineIdx});
        else if (inAllowed(lineTokens[0], Parser::allowedCmdsWArgs)) {
            if (lineTokens.size() < 2)
                throw ParserException(std::string("not enough arguments for command " + lineTokens[0]));

            cmdWArgs = splitCmdWArgs(lineTokens[1]);

            if (!inAllowed(cmdWArgs[0], Parser::allowedTypes))
                throw ParserException(std::string("\"" + cmdWArgs[0] + "\" : unknown type"));

            ret.push_back({cmdWArgs[0], cmdWArgs[1], lineIdx});

            if (lineTokens[0] == "assert")
                ret.push_back({lineTokens[0], "", lineIdx});
        }
        else
            throw ParserException(std::string("\"" + line + "\": unknown statement"));
        return ret;
    }

    std::list<Parser::token>    checkTokens(std::list<Parser::token> &tokens, int line, bool &error)
    {
        if (tokens.empty() || std::find_if(tokens.begin(), tokens.end(), [](auto t){ return t.func == "exit"; }) == tokens.end())
        {
            std::cerr << "Line " << line << ": exit instruction not found\n";
            error = true;
        }
        return tokens;
    }

    std::list<Parser::token>   parseFile(std::istream &stream, bool &error, bool isFile)
    {
        std::string     line;
        std::list<Parser::token>    tokens;
        int             lineCount = 0;
        size_t          foundComment;
        size_t          trailer_idx;

        while (getline(stream, line))
        {
            ++lineCount;
            if (line == ";;" && !isFile)
                return (checkTokens(tokens, lineCount, error));
            foundComment = line.find(';');
            if (foundComment != std::string::npos)
                line = line.substr(0, foundComment);
            std::replace(line.begin(), line.end(), '\t', ' ');
            trailer_idx = line.find_first_not_of(" ");
            if (trailer_idx == std::string::npos)
                continue;
            line = line.substr(trailer_idx, line.find_last_not_of(" ") - trailer_idx + 1);
            if (!line.empty() && line[0] != '\r' && line[0] != '\n')
            {
                try {
                    tokens.splice(tokens.end(), Parser::getToken(line, lineCount));
                }
                catch (Parser::ParserException & e) {
                    std::cerr << "Line " << lineCount << ": " << e.what() << "\n";
                    error = true;
                }
            }
        }
        return (checkTokens(tokens, lineCount, error));
    }

    std::list<Parser::token>    parse(int argc, char **argv, bool &error) {
        std::ifstream   f;

        if (argc > 1)
        {
            f.open(argv[1]);
            if (f.good())
            {
                auto ret = parseFile(f, error, true);
                f.close();
                return ret;
            }
            std::cerr << "Could not open \"" << argv[1] << "\": " << strerror(errno) << '\n';
            error = true;
            return std::list<Parser::token>();
        }
        return parseFile(std::cin, error, false);
    }
}