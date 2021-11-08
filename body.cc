/**
 * @file body.cc
 */

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
        const auto& l = std::get<DatumList>(it->value);
        auto def = make_definition(env, l);
        definitions.push_back(def);
        ++it;
    }
    sequence = make_sequence(env, it, end);
}

void Body::print() const
{
    const char* space = "";
    for (auto def : definitions)
    {
        std::cout << space;
        def->print();
        space = " ";
    }
    std::cout << space;
    sequence->print();
}

Ref<Exp> make_body(
    Env& env, DatumList::const_iterator begin,
    DatumList::const_iterator end)
{
    Body tmp{env, begin, end};
    return make_ref(tmp);
}

}
