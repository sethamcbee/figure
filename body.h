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

struct Definition;
struct Env;

struct Body : Exp
{
    std::list<Ref<Definition>> definitions;
    Ref<Sequence> sequence;

    Body(
        Env& env, DatumList::const_iterator begin,
        DatumList::const_iterator end);

    void print(std::ostream& o) const;
};

Ref<Body> make_body(
    Env& env, DatumList::const_iterator begin,
    DatumList::const_iterator end);

}
