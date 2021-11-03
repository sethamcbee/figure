/**
 * @file figure.h
 */

#pragma once

#include <iostream>
#include <string>
#include <string_view>

#include "datum.h"
#include "env.h"
#include "exp.h"
#include "lexer.h"
#include "sexp.h"

namespace Figure
{

struct Program
{
    Env env;
    Env* ctx;
    Lexer lexer;
    SexpList sexps;
    DatumList data;
    ExpList exps;

    Program(std::istream& input);

    void print()
    {
    }

    void eval()
    {}

    void compile(std::string_view output)
    {}
};

}
