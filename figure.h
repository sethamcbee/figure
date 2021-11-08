/**
 * @file figure.h
 */

#pragma once

#include <iostream>
#include <string>
#include <string_view>

#include "atom.h"
#include "cons.h"
#include "datum.h"
#include "env.h"
#include "exp.h"
#include "lexer.h"
#include "ref.h"
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
    std::list<Ref<Exp>> exps;

    Program(std::istream& input);

    void print();

    void eval();

    static void repl();

    void compile(std::string_view output);
};

}
