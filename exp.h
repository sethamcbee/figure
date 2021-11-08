/**
 * @file exp.h
 */

#pragma once

#include <iostream>

#include "ref.h"

namespace Figure
{
    
struct Env;
struct Datum;

struct Exp
{
    virtual void print(std::ostream& o) const;

    virtual Ref<Exp> eval(const Env& env) const;

    virtual void error();
};

Ref<Exp> make_exp(Env& env, const Datum& datum);

}
