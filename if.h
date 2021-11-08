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
    const Datum* source = nullptr;
    Id keyword;
    Ref<Exp> test;
    Ref<Exp> consequent;
    Ref<Exp> alternate;

    If(Env&, const Datum& datum);

    virtual void print(std::ostream& o) const;

    void error() const;
    void error(const std::string& err) const;
};

Ref<If> make_if(Env& env, const Datum& datum);

}
