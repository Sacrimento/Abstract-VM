#include "../inc/AVM.h"
#include "../inc/Operand.h"
#include "../inc/VM.h"

using namespace std;


int     main(int argc, char **argv)
{
    (void)argc;
    std::list<Parser::token>    tokens = Parser::parseFile(argv[1]);
    VM  vm;

    if (tokens.empty())
        return 1;
    vm.exec(tokens);

    return 0;
}