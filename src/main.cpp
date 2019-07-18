#include "../inc/AVM.h"
#include "../inc/Operand.h"
#include "../inc/VM.h"

using namespace std;


int     main(int argc, char **argv)
{
    std::list<Parser::token>    tokens = Parser::parseFile(argv[1]);

    if (tokens.empty())
        return 1;
    for (auto t : tokens)
        std::cout << t.func << " " << t.value << "\n";

    return 0;
}