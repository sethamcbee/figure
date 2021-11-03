/**
 * @file exp.h
 */

#pragma once

#include <variant>
#include <vector>

#include "datum.h"
#include "env.h"
#include "if.h"
#include "lambda.h"
#include "literal.h"
#include "quote.h"

namespace Figure
{

struct Exp;

using ExpVector = std::vector<Exp>;
using ExpList = std::list<Exp>;

struct ExpPair
{
    ExpList value;
};

struct Exp
{
    using Value = std::variant<Bool, Char, Number, String, Id,
          ExpVector, Quote, Lambda, If>;

    Value value;
    Env env;

    Exp();
    Exp(const Env* ctx, const Datum& d);

    void print() const;
};

}
