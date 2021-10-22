#include "../inc/VM.h"

VM::VM() : _instr({
        {"pop", &VM::pop},
        {"dump", &VM::dump},
        {"add", &VM::add},
        {"sub", &VM::sub},
        {"mul", &VM::mul},
        {"div", &VM::div},
        {"mod", &VM::mod},
        {"xor", &VM::_xor},
        {"and", &VM::_and},
        {"or", &VM::_or},
        {"max", &VM::max},
        {"min", &VM::min},
        {"avg", &VM::avg},
        {"print", &VM::print},
        {"assert", &VM::assert}
    }) {}

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

int             VM::exec(std::list<Parser::token> tokens)
{
    for (auto t : tokens)
    {
        try
        {
            if (t.value.empty())
            {
                if (t.func == "exit")
                    return 0;
                (this->*_instr[t.func])();
            }
            else
                this->_list.push_front(this->createOperand(static_cast<eOperandType>(std::distance(Parser::allowedTypes.begin(), std::find(Parser::allowedTypes.begin(), Parser::allowedTypes.end(), t.func))), t.value));
        }
        catch(const std::exception& e)
        {
            std::cerr << "Line " << t.line << ": " << e.what() << '\n';
            return 1;
        }
    }
    return 1;
}

std::pair<IOperand const *, IOperand const *>   VM::getOperands(void)
{
    IOperand const *op1;
    IOperand const *op2;

    if (this->_list.size() < 2)
        throw AVM_StackTooSmallException();

    op1 = this->_list.front();
    this->_list.pop_front();
    op2 = this->_list.front();
    this->_list.pop_front();

    return std::make_pair(op1, op2);
}

IOperand const  *VM::createInt8( std::string const & value ) const
{
    double  val = std::atof(value.c_str());
    if (val > std::numeric_limits<char>::max())
        throw AVM_OverflowException();
    else if (val < std::numeric_limits<char>::lowest())
        throw AVM_UnderflowException();
    return new Operand<char>(static_cast<char>(val), AVM_INT8, this);
}

IOperand const  *VM::createInt16( std::string const & value ) const
{
    double  val = std::atof(value.c_str());
    if (val > std::numeric_limits<short>::max())
        throw AVM_OverflowException();
    else if (val < std::numeric_limits<short>::lowest())
        throw AVM_UnderflowException();
    return new Operand<short>(static_cast<short>(val), AVM_INT16, this);
}

IOperand const  *VM::createInt32( std::string const & value ) const
{
    double  val = std::atof(value.c_str());
    if (val > std::numeric_limits<int>::max())
        throw AVM_OverflowException();
    else if (val < std::numeric_limits<int>::lowest())
        throw AVM_UnderflowException();
    return new Operand<int>(static_cast<int>(val), AVM_INT32, this);
}

IOperand const  *VM::createFloat( std::string const & value ) const
{
    double  val = std::atof(value.c_str());
    if (val > std::numeric_limits<float>::max())
        throw AVM_OverflowException();
    else if (val < std::numeric_limits<float>::lowest())
        throw AVM_UnderflowException();
    return new Operand<float>(static_cast<float>(val), AVM_FLOAT, this);
}

IOperand const  *VM::createDouble( std::string const & value ) const
{
    double  val = std::atof(value.c_str());
    if (val > 0 && !std::isfinite(val))
        throw AVM_OverflowException();
    else if (val < 0 && !std::isfinite(val))
        throw AVM_UnderflowException();
    return new Operand<double>(val, AVM_DOUBLE, this);
}

void    VM::deleteOperands(IOperand const *op1, IOperand const *op2)
{
    delete op1;
    delete op2;
}

void    VM::pop(void)
{
    if (this->_list.size() == 0)
        throw AVM_PopEmptyException();
    delete this->_list.front();
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
    try {
        this->_list.push_front(*(op.second) + *(op.first));
        deleteOperands(op.first, op.second);
    } catch (std::runtime_error) {
        deleteOperands(op.first, op.second); throw;
    }
}

void    VM::sub(void)
{
    std::pair<IOperand const *, IOperand const *>   op = this->getOperands();
    try {
        this->_list.push_front(*(op.second) - *(op.first));
        deleteOperands(op.first, op.second);
    } catch (std::runtime_error) {
        deleteOperands(op.first, op.second); throw;   
    }
}

void    VM::mul(void)
{
    std::pair<IOperand const *, IOperand const *>   op = this->getOperands();
    try {
        this->_list.push_front(*(op.second) * *(op.first));
        deleteOperands(op.first, op.second);
    } catch (std::runtime_error) {
        deleteOperands(op.first, op.second); throw;   
    }
}

void    VM::div(void)
{
    std::pair<IOperand const *, IOperand const *>   op = this->getOperands();
    try {
        this->_list.push_front(*(op.second) / *(op.first));
        deleteOperands(op.first, op.second);
    } catch (std::runtime_error) {
        deleteOperands(op.first, op.second); throw;   
    }
}

void    VM::mod(void)
{
    std::pair<IOperand const *, IOperand const *>   op = this->getOperands();
    try {
        this->_list.push_front(*(op.second) % *(op.first));
        deleteOperands(op.first, op.second);
    } catch (std::runtime_error) {
        deleteOperands(op.first, op.second); throw;   
    }
}

void    VM::print(void)
{
    if (this->_list.size() < 1)
        throw AVM_StackTooSmallException("stack too small (print requires at least 1 operand)");
    if (this->_list.front()->getType() != AVM_INT8)
        throw AVM_ValueNotInt8Exception();
    std::cout << static_cast<char>(std::atoi(this->_list.front()->toString().c_str()));
}

void    VM::assert(void)
{
    std::pair<IOperand const *, IOperand const *>   op = this->getOperands();
    if (op.first->getType() != op.second->getType() || std::atof(op.first->toString().c_str()) != std::atof(op.second->toString().c_str()))
    {
        deleteOperands(op.first, op.second);
        throw AVM_AssertionFailException();
    }
    delete op.first;
    this->_list.push_front(op.second);
}

void    VM::_xor(void)
{
    std::pair<IOperand const *, IOperand const *>   op = this->getOperands();
    try {
        this->_list.push_front(*(op.first) ^ *(op.second));
        deleteOperands(op.first, op.second);
    } catch (std::runtime_error) {
        deleteOperands(op.first, op.second); throw;   
    }
}

void    VM::_and(void)
{
    std::pair<IOperand const *, IOperand const *>   op = this->getOperands();
    try {
        this->_list.push_front(*(op.first) & *(op.second));
        deleteOperands(op.first, op.second);
    } catch (std::runtime_error) {
        deleteOperands(op.first, op.second); throw;   
    }
}

void    VM::_or(void)
{
    std::pair<IOperand const *, IOperand const *>   op = this->getOperands();
    try {
        this->_list.push_front(*(op.first) | *(op.second));
        deleteOperands(op.first, op.second);
    } catch (std::runtime_error) {
        deleteOperands(op.first, op.second); throw;   
    }
}

void    VM::max(void)
{
    auto cmp = [](const IOperand* lhs, const IOperand* rhs) { return std::atof(lhs->toString().c_str()) < std::atof(rhs->toString().c_str()); };
    auto max = *std::max_element(this->_list.begin(), this->_list.end(), cmp);
    this->_list.push_front(this->createOperand(max->getType(), max->toString()));
}

void    VM::min(void)
{
    auto cmp = [](const IOperand* lhs, const IOperand* rhs) { return std::atof(lhs->toString().c_str()) < std::atof(rhs->toString().c_str()); };
    auto min = *std::min_element(this->_list.begin(), this->_list.end(), cmp);
    this->_list.push_front(this->createOperand(min->getType(), min->toString()));
}

void    VM::avg(void)
{
    std::list<double> tmp;
    for (auto op : this->_list)
        tmp.push_back(std::atof(op->toString().c_str()));
    this->_list.push_front(this->createDouble(std::to_string(std::accumulate(tmp.begin(), tmp.end(), 0) / tmp.size())));
}
