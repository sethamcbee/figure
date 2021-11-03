/**
 * @file lambda.h
 */

#pragma once

#include <list>

#include "formals.h"

namespace Figure
{

struct Env;
struct Exp;

using ExpList = std::list<Exp>;

struct Lambda
{
    Formals formals;
    ExpList bodies;

    Lambda(const Env* e, const Datum& d);

    void print() const;
};

}
