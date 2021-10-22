#ifndef VM_H
#define VM_H

#include "Operand.h"
#include <limits>
#include <cfloat>
#include <cmath>
#include <map>
#include <utility>
#include <numeric>

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
        void    deleteOperands(IOperand const *, IOperand const *);

        void    pop(void);
        void    dump(void);
        void    add(void);
        void    sub(void);
        void    mul(void);
        void    div(void);
        void    mod(void);
        void    _xor(void);
        void    _and(void);
        void    _or(void);
        void    max(void);
        void    min(void);
        void    avg(void);
        void    print(void);
        void    assert(void);

    public:
        VM();
        virtual ~VM();
        IOperand const *    createOperand( eOperandType type, std::string const & value ) const;
        int                 exec(std::list<Parser::token> tokens);

        class AVM_OverflowException : public std::overflow_error
        {
            public:
                AVM_OverflowException() : std::overflow_error("value overflow") {}
                AVM_OverflowException(AVM_OverflowException const & src) : std::overflow_error("value overflow") { *this = src; }
                AVM_OverflowException &operator=(AVM_OverflowException const &rhs) noexcept { std::overflow_error::operator=(rhs); return *this; }
                ~AVM_OverflowException() throw() {}
                AVM_OverflowException(const std::string& what) : std::overflow_error(what) {}
        };

        class AVM_UnderflowException : public std::underflow_error
        {
            public:
                AVM_UnderflowException() : std::underflow_error("value underflow") {}
                AVM_UnderflowException(AVM_UnderflowException const & src) : std::underflow_error("value underflow") { *this = src; }
                AVM_UnderflowException &operator=(AVM_UnderflowException const &rhs) { std::underflow_error::operator=(rhs); return *this; }
                ~AVM_UnderflowException() throw() {}
                AVM_UnderflowException(const std::string& what) : std::underflow_error(what) {}
        };

        class AVM_PopEmptyException : public std::runtime_error
        {
            public:
                AVM_PopEmptyException() : std::runtime_error("pop on empty stack") {}
                AVM_PopEmptyException(AVM_PopEmptyException const & src) : std::runtime_error("pop on empty stack") { *this = src; }
                AVM_PopEmptyException &operator=(AVM_PopEmptyException const &rhs) { std::runtime_error::operator=(rhs); return *this; }
                ~AVM_PopEmptyException() throw() {}
                AVM_PopEmptyException(const std::string& what) : std::runtime_error(what) {}
        };

        class AVM_ValueNotInt8Exception : public std::runtime_error
        {
            public:
                AVM_ValueNotInt8Exception() : std::runtime_error("top of the stack has not type int8") {}
                AVM_ValueNotInt8Exception(AVM_ValueNotInt8Exception const & src) : std::runtime_error("top of the stack has not type int8") { *this = src; }
                AVM_ValueNotInt8Exception &operator=(AVM_ValueNotInt8Exception const &rhs) { std::runtime_error::operator=(rhs); return *this; }
                ~AVM_ValueNotInt8Exception() throw() {}
                AVM_ValueNotInt8Exception(const std::string& what) : std::runtime_error(what) {}
        };

        class AVM_AssertionFailException : public std::runtime_error
        {
            public:
                AVM_AssertionFailException() : std::runtime_error("assertion failed") {}
                AVM_AssertionFailException(AVM_AssertionFailException const & src) : std::runtime_error("assertion failed") { *this = src; }
                AVM_AssertionFailException &operator=(AVM_AssertionFailException const &rhs) { std::runtime_error::operator=(rhs); return *this; }
                ~AVM_AssertionFailException() throw() {}
                AVM_AssertionFailException(const std::string& what) : std::runtime_error(what) {}
        };

        class AVM_StackTooSmallException : public std::runtime_error
        {
            public:
                AVM_StackTooSmallException() : std::runtime_error("stack too small (less than 2 operands)") {}
                AVM_StackTooSmallException(AVM_StackTooSmallException const & src) : std::runtime_error("stack too small (less than 2 operands)") { *this = src; }
                AVM_StackTooSmallException &operator=(AVM_StackTooSmallException const &rhs) { std::runtime_error::operator=(rhs); return *this; }
                ~AVM_StackTooSmallException() throw() {}
                AVM_StackTooSmallException(const std::string& what) : std::runtime_error(what) {}
        };
};

#endif