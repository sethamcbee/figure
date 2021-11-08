/**
 * @file body.cc
 */

#include <iostream>

#include "body.h"
#include "definition.h"
#include "sequence.h"

namespace Figure
{

Body::Body(
    Env& env, DatumList::const_iterator begin,
    DatumList::const_iterator end)
{
    auto it = begin;
    auto last = std::prev(end);
    while (it != last && is_definition(*it))
    {
        auto def = make_definition(env, *it);
        definitions.push_back(def);
        ++it;
    }
    sequence = make_sequence(env, it, end);
}

void Body::print(std::ostream& o) const
{
    const char* space = "";
    for (auto def : definitions)
    {
        o << space;
        def->print(o);
        space = " ";
    }
    o << space;
    sequence->print(o);
}

Ref<Body> make_body(
    Env& env, DatumList::const_iterator begin,
    DatumList::const_iterator end)
{
    Body tmp{env, begin, end};
    return make_ref(tmp);
}

}
