/**
 * @file exp.h
 */

#pragma once

#include <iostream>

#include "literal.h"
#include "datum.h"
#include "ref.h"

namespace Figure
{
    
struct Env;
struct Datum;

struct Exp
{
    const Datum* source = nullptr;
    
    virtual void print(std::ostream& o) const;

    virtual Ref<Exp> eval(const Env& env) const;

    virtual void error() const;
};

Ref<Exp> make_exp(Env& env, const Datum& datum);

}
