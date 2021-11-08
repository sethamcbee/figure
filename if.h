/**
 * @file if.h
 */

#pragma once

#include "datum.h"
#include "exp.h"
#include "literal.h"

namespace Figure
{

struct Env;

struct If : public Exp
{
    Datum* source = nullptr;
    Id keyword;
    Ref<Exp> test;
    Ref<Exp> consequent;
    Ref<Exp> alternate;

    If(Env&, const Datum& datum);

    virtual void print(std::ostream& o) const;

    virtual void error() const;
    virtual void error(const std::string& err) const;
};

Ref<Exp> make_if(Env& env, const Datum& datum);

}
