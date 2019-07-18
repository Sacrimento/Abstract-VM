#include "../inc/VM.h"

VM::VM() {}

VM::~VM() {}

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

