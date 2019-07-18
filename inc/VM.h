#ifndef VM_H
#define VM_H

#include "Operand.h"
#include <limits>
#include <cfloat>
#include <cmath>
#include <map>

class VM
{
    private:
        static std::map<std::string, void (VM::*)(void)>          _instr;
        static std::map<std::string, void (VM::*)(std::string)>   _factory;

        VM(VM const & src) = delete;
        VM &operator=(VM const & rhs) = delete;

        IOperand const *    createInt8( std::string const & value ) const;
        IOperand const *    createInt16( std::string const & value ) const;
        IOperand const *    createInt32( std::string const & value ) const;
        IOperand const *    createFloat( std::string const & value ) const;
        IOperand const *    createDouble( std::string const & value ) const;

        void                pop(void);
        void                dump(void);
        void                add(void);
        void                sub(void);
        void                mul(void);
        void                div(void);
        void                mod(void);
        void                print(void);

        void                int8(std::string s);
        void                int16(std::string s);
        void                int32(std::string s);
        void                float1(std::string s);
        void                double1(std::string s);

    public:
        std::list<IOperand*>    list;
        
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

};

#endif