/**
 * @file sequence.h
 */

#pragma once

#include <list>

#include "datum.h"
#include "exp.h"

namespace Figure
{

struct Env;

struct Sequence : public Exp
{
    std::list<Ref<Exp>> commands;
    Ref<Exp> expression;

    Sequence(
        Env& env, DatumList::const_iterator begin,
        DatumList::const_iterator end);

    virtual void print(std::ostream& o) const;
};

Ref<Exp> make_sequence(
    Env& env, DatumList::const_iterator begin,
    DatumList::const_iterator end);

}
