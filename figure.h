/**
 * @file figure.h
 */

#pragma once

#include <iostream>
#include <string>
#include <string_view>

#include "lexer.h"
#include "sexp.h"

namespace Figure
{

class Program
{
public:
    
    Program(std::istream& input);

    void print()
    {
    }

    void eval()
    {}

    void compile(std::string_view output)
    {}

private:

    Lexer lexer;
    SexpList sexps;
};

}
