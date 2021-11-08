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
    Id keyword;
    Ref<Exp> test;
    Ref<Exp> consequent;
    Ref<Exp> alternate;

    If(Env&, const DatumList& l);

    virtual void print() const;
};

Ref<Exp> make_if(Env& env, const DatumList& l);

}
