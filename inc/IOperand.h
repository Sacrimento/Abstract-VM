#ifndef IOPERAND_H
#define IOPERAND_H

#include "AVM.h"

enum eOperandType
{
    AVM_INT8,
    AVM_INT16,
    AVM_INT32,
    AVM_FLOAT,
    AVM_DOUBLE
};

class IOperand
{

    public:
        virtual int getPrecision( void ) const = 0; // Precision of the type of the instance
        virtual eOperandType getType( void ) const = 0; // Type of the instance
        
        virtual IOperand const * operator+( IOperand const & rhs ) const = 0; // Sum
        virtual IOperand const * operator-( IOperand const & rhs ) const = 0; // Difference
        virtual IOperand const * operator*( IOperand const & rhs ) const = 0; // Product
        virtual IOperand const * operator/( IOperand const & rhs ) const = 0; // Quotient
        virtual IOperand const * operator%( IOperand const & rhs ) const = 0; // Modulo
        virtual std::string const & toString( void ) const = 0; // String representation of the instance
        
        virtual ~IOperand( void ) {}
};

#endif
