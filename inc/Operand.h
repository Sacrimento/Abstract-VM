#ifndef OPERAND_H
#define OPERAND_H

#include "IOperand.h"
#include "VM.h"

class VM;

template <typename T>
class Operand : public IOperand
{
    private:
        T               _value;
        std::string     _sValue;
        eOperandType    _type;
        VM const        *_vm;
        Operand() = delete;

    public:
        Operand(T value, eOperandType type, VM const *vm);
        Operand(Operand<T> const & src);
        virtual ~Operand()  {}
        Operand<T> &        operator=(Operand<T> const & rhs);

        int                 getPrecision( void ) const; // Precision of the type of the instance
        eOperandType        getType( void ) const; // Type of the instance
        
        IOperand const *    operator+( IOperand const & rhs ) const; // Sum
        IOperand const *    operator-( IOperand const & rhs ) const; // Difference
        IOperand const *    operator*( IOperand const & rhs ) const; // Product
        IOperand const *    operator/( IOperand const & rhs ) const; // Quotient
        IOperand const *    operator%( IOperand const & rhs ) const; // Modulo
        IOperand const *    operator^( IOperand const & rhs ) const; // Xor
        IOperand const *    operator&( IOperand const & rhs ) const; // And
        IOperand const *    operator|( IOperand const & rhs ) const; // Or
        std::string const & toString( void ) const; // String representation of the instance

        class DivisionByZeroException : public std::logic_error
        {
            public:
                DivisionByZeroException() : std::logic_error("divison by zero") {}
                DivisionByZeroException(DivisionByZeroException const & src) : std::logic_error("divison by zero") { *this = src; }
                DivisionByZeroException &operator=(DivisionByZeroException const &rhs) { std::logic_error::operator=(rhs); return *this; }
                ~DivisionByZeroException() throw() {}
                DivisionByZeroException(const std::string& what) : std::logic_error(what) {}
        };
        
};

template <typename T>
std::ostream	&operator<<(std::ostream & o, Operand<T> const & rhs)
{
    o << rhs.toString();
    return o;
}

template <typename T>
Operand<T>::Operand(T value, eOperandType type, VM const *vm) : _value(value), _sValue(std::to_string(value)), _type(type), _vm(vm) {}

template <typename T>
Operand<T>::Operand(Operand<T> const & src)
{
    *this = src;
}

template <typename T>
Operand<T> &    Operand<T>::operator=(Operand<T> const & rhs)
{
    if (this != &rhs)
    {
        this->_value = rhs._value;
        this->_sValue = rhs._sValue;
        this->_type = rhs._type;
        this->_vm = rhs._vm;
    }
    return *this;
}

template <typename T>
eOperandType    Operand<T>::getType() const { return this->_type; }

template <typename T>
int             Operand<T>::getPrecision() const { return (sizeof(this->_value) * 8) + (this->_type == AVM_FLOAT ? 1 : 0); }

template <typename T>
IOperand const *    Operand<T>::operator+( IOperand const & rhs ) const
{
    return this->_vm->createOperand((this->getPrecision() > rhs.getPrecision() ? this->getType() : rhs.getType()), std::to_string(std::atof(this->toString().c_str()) + std::atof(rhs.toString().c_str())));
} 

template <typename T>
IOperand const *    Operand<T>::operator-( IOperand const & rhs ) const
{
    return this->_vm->createOperand((this->getPrecision() > rhs.getPrecision() ? this->getType() : rhs.getType()), std::to_string(std::atof(this->toString().c_str()) - std::atof(rhs.toString().c_str())));
}

template <typename T>
IOperand const *    Operand<T>::operator*( IOperand const & rhs ) const
{
    return this->_vm->createOperand((this->getPrecision() > rhs.getPrecision() ? this->getType() : rhs.getType()), std::to_string(std::atof(this->toString().c_str()) * std::atof(rhs.toString().c_str())));
}

template <typename T>
IOperand const *    Operand<T>::operator/( IOperand const & rhs ) const
{
    if (std::atof(rhs.toString().c_str()) == 0)
        throw Operand<T>::DivisionByZeroException();
    return this->_vm->createOperand((this->getPrecision() > rhs.getPrecision() ? this->getType() : rhs.getType()), std::to_string(std::atof(this->toString().c_str()) / std::atof(rhs.toString().c_str())));
}

template <typename T>
IOperand const *    Operand<T>::operator%( IOperand const & rhs ) const
{
    if (std::atof(rhs.toString().c_str()) == 0)
        throw Operand<T>::DivisionByZeroException();
    return this->_vm->createOperand((this->getPrecision() > rhs.getPrecision() ? this->getType() : rhs.getType()), std::to_string(fmod(std::atof(this->toString().c_str()), std::atof(rhs.toString().c_str()))));
}

template <typename T>
IOperand const *    Operand<T>::operator^( IOperand const & rhs ) const
{
    return this->_vm->createOperand((this->getPrecision() > rhs.getPrecision() ? this->getType() : rhs.getType()), std::to_string(std::stoll(this->toString().c_str()) ^ std::stoll(rhs.toString().c_str())));
}

template <typename T>
IOperand const *    Operand<T>::operator&( IOperand const & rhs ) const
{
    return this->_vm->createOperand((this->getPrecision() > rhs.getPrecision() ? this->getType() : rhs.getType()), std::to_string(std::stoll(this->toString().c_str()) & std::stoll(rhs.toString().c_str())));
}

template <typename T>
IOperand const *    Operand<T>::operator|( IOperand const & rhs ) const
{
    return this->_vm->createOperand((this->getPrecision() > rhs.getPrecision() ? this->getType() : rhs.getType()), std::to_string(std::stoll(this->toString().c_str()) | std::stoll(rhs.toString().c_str())));
}

template <typename T>
std::string const & Operand<T>::toString( void ) const
{
    return this->_sValue;
}

#endif
