/**
 * @file exp.h
 */

#pragma once

#include <iostream>
#include <string>

#include "literal.h"
#include "datum.h"
#include "ref.h"

namespace Figure
{

struct Env;
struct Datum;

struct Exp
{
    virtual void print(std::ostream& o) const;

    virtual Ref<Exp> eval(const Env& env) const;

    virtual void error() const;
    virtual void error(const std::string& err) const;
};

Ref<Exp> make_exp(Env& env, const Datum& datum);

}
