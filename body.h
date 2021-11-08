/**
 * @file body.h
 */

#pragma once

#include <list>

#include "datum.h"
#include "exp.h"
#include "sequence.h"

namespace Figure
{

struct Env;

struct Body : Exp
{
    std::list<Ref<Exp>> definitions;
    Ref<Exp> sequence;

    Body(
        Env& env, DatumList::const_iterator begin,
        DatumList::const_iterator end);

    void print(std::ostream& o) const;
};

Ref<Exp> make_body(
    Env& env, DatumList::const_iterator begin,
    DatumList::const_iterator end);

}
