/**
 * @file proc.h
 */

#pragma once

#include <memory>

#include "literal.h"

namespace Figure
{

struct Env;
struct Exp;
struct Datum;

struct Proc
{
    Env* env = nullptr;
    std::shared_ptr<Exp> op = nullptr;
    ExpList args;

    Proc(Env& e, const Datum& d);

    void print() const;
};

}
