#include "../inc/AVM.h"
#include "../inc/Operand.h"
#include "../inc/VM.h"

using namespace std;


int     main(int argc, char **argv)
{
    (void)argc;
    bool error = false;
    std::list<Parser::token>    tokens = Parser::parse(argc, argv, error);
    VM  vm;

    if (error)
        return 1;
    return vm.exec(tokens);
}