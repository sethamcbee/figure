/**
 * @file proc.h
 */

#pragma once

#include <memory>

#include "literal.h"
#include "ref.h"

namespace Figure
{

struct Env;
struct Exp;
struct Datum;

struct Proc
{
    Env* env = nullptr;
    Ref<Exp> op = nullptr;
    ExpList args;

    Proc(Env& e, const Datum& d);

    void print() const;
};

}
