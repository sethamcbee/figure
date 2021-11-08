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
    const Datum* source;
    Id keyword;
    Id identifier;
    Ref<Exp> expression;

    Set(Env& env, const Datum& l);

    virtual void print(std::ostream& o) const;

    virtual void error() const;
    virtual void error(const std::string& err) const;
};

Ref<Exp> make_set(Env& env, const Datum& datum);

}
