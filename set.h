/**
 * @file set.h
 */

#pragma once

#include "literal.h"
#include "ref.h"

namespace Figure
{

struct Env;
struct Exp;
struct Datum;

struct Set
{
    Id id;
    Env* env;

    Set(Env& e, const Datum& d);

    void print() const;
};

}
