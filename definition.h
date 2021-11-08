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
    const Datum* source = nullptr;
    Id keyword;
    Id identifier;
    Ref<Exp> expression;

    DefineVar(Env& env, const Datum& datum);

    virtual void print(std::ostream& o) const;

    void error() const;
    void error(const std::string& err) const;
};

Ref<Exp> make_definition(Env& env, const Datum& datum);

bool is_definition(const Datum& datum);

}
