/**
 * @file formals.cc
 */

#include "atom.h"
#include "env.h"
#include "formals.h"

namespace Figure
{

Formals::Formals()
{}

Formals::Formals(Env& env, const Datum& datum)
{
    if (auto id = std::get_if<Id>(&datum.value))
    {
        value = *id;
        auto arg = Arg{};
        env.bind(*id, arg);
    }
    else if (auto l = std::get_if<DatumList>(&datum.value))
    {
        value = IdList{};
        auto& list = std::get<IdList>(value);
        auto arg = Arg{};
        for (const auto& datum : *l)
        {
            const auto& id = std::get<Id>(datum.value);
            list.push_back(id);
            env.bind(id, arg);
        }
    }
    else if (auto p = std::get_if<DatumPair>(&datum.value))
    {
        value = IdPair{};
        auto& pair = std::get<IdPair>(value);
        auto& list = pair.value;
        auto arg = Arg{};
        for (const auto& datum : p->value)
        {
            const auto& id = std::get<Id>(datum.value);
            list.push_back(id);
            env.bind(id, arg);
        }
    }
}

void Formals::print(std::ostream& o) const
{
    if (auto id = std::get_if<Id>(&value))
    {
        o << *id;
    }
    else if (auto l = std::get_if<IdList>(&value))
    {
        o << "(";
        const char* space = "";
        for (const auto& id : *l)
        {
            o << space << id;
            space = " ";
        }
        o << ")";
    }
    else if (auto p = std::get_if<IdPair>(&value))
    {
        const auto& l = p->value;
        auto last = --l.end();
        o << "(";
        const char* space = "";
        for (auto id = l.begin(); id != last; ++id)
        {
            o << space << *id;
            space = " ";
        }
        o << " . ";
        o << *last;
        o << ")";
    }
}

}
