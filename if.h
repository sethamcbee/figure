/**
 * @file if.h
 */

#pragma once

#include <variant>

namespace Figure
{

struct Env;
struct Exp;

struct NoAlternate {};

struct If
{
    using Value = std::list<std::variant<Exp, NoAlternate>>;

    Value value;

    If(Env& e, const Datum& d);

    void print() const;
};

}
