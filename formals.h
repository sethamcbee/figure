/**
 * @file formals.h
 */

#pragma once

#include <list>
#include <variant>

#include "cons.h"
#include "datum.h"
#include "exp.h"
#include "literal.h"

namespace Figure
{

struct Env;

using IdList = std::list<Id>;

struct IdPair
{
    IdList value;
};

struct Formals : public Exp
{
    using Value = std::variant<IdList, Id, IdPair>;

    Value value;

    Formals();
    Formals(Env& env, const Datum& datum);

    void print(std::ostream& o) const;
};

}
