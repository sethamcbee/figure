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

void Sequence::print() const
{
    const char* space = "";
    for (const auto& cmd : commands)
    {
        std::cout << space;
        cmd->print();
        space = " ";
    }
    std::cout << space;
    expression->print();
}

Ref<Exp> make_sequence(
    Env& env, DatumList::const_iterator begin,
    DatumList::const_iterator end)
{
    Sequence tmp{env, begin, end};
    return make_ref(tmp);
}

}
