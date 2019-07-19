#include "../inc/VM.h"

VM::VM() : _instr({
        {"pop", &VM::pop},
        {"dump", &VM::dump},
        {"add", &VM::add},
        {"sub", &VM::sub},
        {"mul", &VM::mul},
        {"div", &VM::div},
        {"mod", &VM::mod},
        {"print", &VM::print},
        {"assert", &VM::assert}}) {}

VM::~VM()
{
    for (auto op : this->_list)
        delete op;
}

IOperand const  *VM::createOperand(eOperandType type, std::string const & value) const
{
    static IOperand const *(VM::*funcs[5])(std::string const &) const = {
        &VM::createInt8,
        &VM::createInt16,
        &VM::createInt32,
        &VM::createFloat,
        &VM::createDouble
    };
    return (this->*funcs[type])(value);
}

void            VM::exec(std::list<Parser::token> tokens)
{
    for (auto t : tokens)
    {
        // std::cout << t.func + " " + t.value + "\n";
        try
        {
            if (t.value.empty())
                (this->*_instr[t.func])();
            else
                this->_list.push_front(this->createOperand(static_cast<eOperandType>(std::distance(Parser::allowedConstructor.begin(), std::find(Parser::allowedConstructor.begin(), Parser::allowedConstructor.end(), t.func))), t.value));
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return;
        }
        
    }
}

std::pair<IOperand const *, IOperand const *>   VM::getOperands(void)
{
    IOperand const *op1;
    IOperand const *op2;

    if (this->_list.size() < 2)
        std::cout << "less than 2\n";

    op1 = this->_list.front();
    this->_list.pop_front();
    op2 = this->_list.front();
    this->_list.pop_front();

    return std::make_pair(op1, op2);
}

IOperand const  *VM::createInt8( std::string const & value ) const
{
    double  val = std::atof(value.c_str());
    // if (val > std::numeric_limits<char>::max())
    //     throw VM::OverflowException();
    // else if (val < std::numeric_limits<char>::lowest())
    //     throw VM::UnderflowException();
    return new Operand<char>(static_cast<char>(val), AVM_INT8, this);
}

IOperand const  *VM::createInt16( std::string const & value ) const
{
    double  val = std::atof(value.c_str());
    // if (val > std::numeric_limits<short>::max())
    //     throw VM::OverflowException();
    // else if (val < std::numeric_limits<short>::lowest())
    //     throw VM::UnderflowException();
    return new Operand<short>(static_cast<short>(val), AVM_INT16, this);
}

IOperand const  *VM::createInt32( std::string const & value ) const
{
    double  val = std::atof(value.c_str());
    // if (val > std::numeric_limits<int>::max())
    //     throw VM::OverflowException();
    // else if (val < std::numeric_limits<int>::lowest())
    //     throw VM::UnderflowException();
    return new Operand<int>(static_cast<int>(val), AVM_INT32, this);
}

IOperand const  *VM::createFloat( std::string const & value ) const
{
    double  val = std::atof(value.c_str());
    // if (val > std::numeric_limits<float>::max())
    //     throw VM::OverflowException();
    // else if (val < std::numeric_limits<float>::lowest())
    //     throw VM::UnderflowException();
    return new Operand<float>(static_cast<float>(val), AVM_FLOAT, this);
}

IOperand const  *VM::createDouble( std::string const & value ) const
{
    double  val = std::atof(value.c_str());
    // if (val > 0 && !std::isfinite(val))
    //     throw VM::OverflowException();
    // else if (val < 0 && !std::isfinite(val))
    //     throw VM::UnderflowException();
    return new Operand<double>(val, AVM_DOUBLE, this);
}

void    VM::pop(void)
{
    if (this->_list.size() == 0)
        std::cout << "pop empty\n";
    this->_list.pop_front();
}

void    VM::dump(void)
{
    for (auto op : this->_list)
        std::cout << op->toString() << std::endl;
}
        
void    VM::add(void)
{
    std::pair<IOperand const *, IOperand const *>   op = this->getOperands();
    this->_list.push_front(*(op.first) + *(op.second));
    delete op.first;
    delete op.second;
}

void    VM::sub(void)
{
    std::pair<IOperand const *, IOperand const *>   op = this->getOperands();
    this->_list.push_front(*(op.first) - *(op.second));
    delete op.first;
    delete op.second;
}

void    VM::mul(void)
{
    std::pair<IOperand const *, IOperand const *>   op = this->getOperands();
    this->_list.push_front(*(op.first) * *(op.second));
    delete op.first;
    delete op.second;
}

void    VM::div(void)
{
    std::pair<IOperand const *, IOperand const *>   op = this->getOperands();
    this->_list.push_front(*(op.first) / *(op.second));
    delete op.first;
    delete op.second;
}

void    VM::mod(void)
{
    std::pair<IOperand const *, IOperand const *>   op = this->getOperands();
    this->_list.push_front(*(op.first) % *(op.second));
    delete op.first;
    delete op.second;
}

void    VM::print(void)
{
    if (this->_list.front()->getType() != AVM_INT8)
        std::cout << "not char\n";
    std::cout << static_cast<char>(this->_list.front()->toString()[0]);
}

void    VM::assert(void)
{
    std::pair<IOperand const *, IOperand const *>   op = this->getOperands();
    if (op.first->getType() != op.second->getType() || op.first->toString() != op.second->toString())
        std::cout << "assertion failed\n";
    delete op.first;
    this->_list.push_front(op.second);
}
