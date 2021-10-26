/**
 * @file program.h
 */

#pragma once

#include <iostream>
#include <string>
#include <string_view>

#include "exp.h"
#include "sexp.h"

namespace Figure
{

class Program
{
public:

    Program(const std::string& src) :
        lexer{src},
        sexp{lexer.tokens.begin(), lexer.tokens.end()},
        exp{sexp}
    {}

    void print()
    {
        std::cout << "\nLexer:\n======\n";
        lexer.print();
        std::cout << "\n\nSexp:\n======\n";
        sexp.print();
        std::cout << "\n\nExp:\n======\n";
        exp.print();
        std::cout << std::endl;
    }

    void eval()
    {
        auto ret = exp.eval();
        Exp tmp{ret};
        std::cout << "======\nReturned: ";
        tmp.print();
        std::cout << std::endl;
    }

    void compile(std::string_view output)
    {}

private:

    Lexer lexer;
    Sexp sexp;
    Exp exp;
};

}
