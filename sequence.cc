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
        auto cmd = make_command(env, *it);
        if (cmd)
        {
            commands.push_back(cmd);
            ++it;
        }
        else
        {
            std::cerr << "At character: " << it->pos << std::endl;
            std::cerr << "At datum: ";
            it->print(std::cerr);
            std::cerr << "\nError processing sequence.\n";
            exit(1);
        }
    }
    expression = make_exp(env, *last);
    if (!expression)
    {
        std::cerr << "At character: " << last->pos << std::endl;
        std::cerr << "At datum: ";
        last->print(std::cerr);
        std::cerr << "\nError processing sequence.\n";
        exit(1);
    }
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

Ref<Sequence> make_sequence(
    Env& env, DatumList::const_iterator begin,
    DatumList::const_iterator end)
{
    Sequence tmp{env, begin, end};
    return make_ref(tmp);
}

}
