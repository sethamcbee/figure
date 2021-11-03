/**
 * @file formals.h
 */

#pragma once

#include <list>
#include <variant>

#include "literal.h"

namespace Figure
{

struct Datum;
struct Env;

using IdList = std::list<Id>;

struct IdPair
{
    IdList value;
};

struct Formals
{
    using Value = std::variant<IdList, Id, IdPair>;

    Value value;

    Formals();
    Formals(Env& e, const Datum& d);

    void print() const;
};

}