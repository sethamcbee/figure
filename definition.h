/**
 * @file definition.h
 */

#pragma once

#include "datum.h"
#include "exp.h"
#include "literal.h"

namespace Figure
{

struct Env;

struct Definition : public Exp
{};

struct DefineVar : public Definition
{
    Id keyword;
    Id identifier;
    Ref<Exp> expression;

    DefineVar(Env& env, const DatumList& datum);

    virtual void print(std::ostream& o) const;
};

Ref<Exp> make_definition(Env& env, const DatumList& datum);

bool is_definition(const Datum& datum);

}
