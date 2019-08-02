#ifndef VM_H
#define VM_H

#include "Operand.h"
#include <limits>
#include <cfloat>
#include <cmath>
#include <map>
#include <utility>

class VM
{
    private:
        std::list<IOperand const *>  _list;
        std::map<std::string, void (VM::*)(void)>   _instr;

        VM(VM const & src) = delete;
        VM &operator=(VM const & rhs) = delete;

        IOperand const *    createInt8( std::string const & value ) const;
        IOperand const *    createInt16( std::string const & value ) const;
        IOperand const *    createInt32( std::string const & value ) const;
        IOperand const *    createFloat( std::string const & value ) const;
        IOperand const *    createDouble( std::string const & value ) const;

        std::pair<IOperand const *, IOperand const *>   getOperands(void);

        void    pop(void);
        void    dump(void);
        void    add(void);
        void    sub(void);
        void    mul(void);
        void    div(void);
        void    mod(void);
        void    print(void);
        void    assert(void);

    public:
        VM();
        virtual ~VM();
        IOperand const *    createOperand( eOperandType type, std::string const & value ) const;
        void                exec(std::list<Parser::token> tokens);

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

        class PopEmptyException : public std::exception
        {
            public:
                virtual const char	*what() const throw();
        };

        class ValueNotInt8Exception : public std::exception
        {
            public:
                virtual const char	*what() const throw();
        };

        class AssertionFailException : public std::exception
        {
            public:
                virtual const char	*what() const throw();
        };

        class StackTooSmallException : public std::exception
        {
            public:
                virtual const char	*what() const throw();
        };
};

#endif