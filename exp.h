/**
 * @file exp.h
 */

#pragma once

#include <utility>

#include "env.h"
#include "sexp.h"

namespace Figure
{

class Exp;

using List = std::list<Exp>;
using ExpPtr = std::unique_ptr<Exp>;
using ExpVal = std::variant<
    Bool, Number, Char, String, Id, List>;

class Exp
{
public:

    Exp(ExpVal e);

    Exp(Sexp s);

    const void print();

    ExpVal eval();

    void error();
    void error(std::string_view msg);

    ExpVal value;
    Env env;

private:

    Sexp source;
};

};
