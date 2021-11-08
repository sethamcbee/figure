/**
 * @file exp.h
 */

#pragma once

#include "ref.h"

namespace Figure
{

struct Env;
struct Datum;

struct Exp
{
    virtual void print() const;

    virtual void error();
};

Ref<Exp> make_exp(Env& env, const Datum& datum);

}
