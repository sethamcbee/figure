/**
 * @file sequence.cc
 */

#include "command.h"
#include "sequence.h"

namespace Figure
{

Sequence::Sequence(
    Env& env, DatumList::const_iterator begin,
    DatumList::const_iterator end)
{
    auto last = std::prev(end);
    auto it = begin;
    while (it != last)
    {
        commands.push_back(make_command(env, *it));
        ++it;
    }
    expression = make_exp(env, *last);
}

void Sequence::print(std::ostream& o) const
{
    const char* space = "";
    for (const auto& cmd : commands)
    {
        o << space;
        cmd->print(o);
        space = " ";
    }
    o << space;
    expression->print(o);
}

Ref<Exp> make_sequence(
    Env& env, DatumList::const_iterator begin,
    DatumList::const_iterator end)
{
    Sequence tmp{env, begin, end};
    return make_ref(tmp);
}

}
