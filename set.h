/**
 * @file set.h
 */

#pragma once

#include "datum.h"
#include "exp.h"
#include "literal.h"

namespace Figure
{

struct Env;

struct Set : public Exp
{
    Id keyword;
    Id identifier;
    Ref<Exp> expression;

    Set(Env& env, const DatumList& l);

    virtual void print() const;
};

Ref<Exp> make_set(Env& env, const DatumList& l);

}
