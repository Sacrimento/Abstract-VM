#ifndef VM_H
#define VM_H

#include "Operand.h"
#include <limits>
#include <cfloat>
#include <cmath>

class VM
{
    private:
        std::list<IOperand*>    _list;
        VM(VM const & src) = delete;
        VM &operator=(VM const & rhs) = delete;
        IOperand const * createInt8( std::string const & value ) const;
        IOperand const * createInt16( std::string const & value ) const;
        IOperand const * createInt32( std::string const & value ) const;
        IOperand const * createFloat( std::string const & value ) const;
        IOperand const * createDouble( std::string const & value ) const;

    public:
        VM();
        virtual ~VM();
        IOperand const * createOperand( eOperandType type, std::string const & value ) const;

        class OverflowException : public std::exception
        {
            public:
                virtual const char	*what() const throw();
        };

        class UnderflowException : public std::exception
        {
            public:
                virtual const char	*what() const throw();
        };

};

#endif