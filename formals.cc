/**
 * @file formals.cc
 */

#include <iostream>

#include "datum.h"
#include "formals.h"

namespace Figure
{

Formals::Formals()
{
    value = IdList{};
}

Formals::Formals(Env& e, const Datum& d)
{
    if (auto id = std::get_if<Id>(&d.value))
    {
        value = *id;
    }
    else if (auto l = std::get_if<DatumList>(&d.value))
    {
        value = IdList{};
        auto& list = std::get<IdList>(value);
        for (const auto& id : *l)
        {
            auto arg = std::get<Id>(id.value);
            list.push_back(arg);
        }
    }
    else if (auto p = std::get_if<DatumPair>(&d.value))
    {
        value = IdPair{};
        auto& pair = std::get<IdPair>(value);
        auto& list = pair.value;
        for (const auto& id : p->value)
        {
            auto arg = std::get<Id>(id.value);
            list.push_back(arg);
        }
    }
}

void Formals::print() const
{
    if (auto id = std::get_if<Id>(&value))
    {
        std::cout << id->value;
    }
    else if (auto l = std::get_if<IdList>(&value))
    {
        std::cout << "(";
        const char* space = "";
        for (const auto& id : *l)
        {
            std::cout << space << id.value;
            space = " ";
        }
        std::cout << ")";
    }
    else if (auto p = std::get_if<IdPair>(&value))
    {
        const auto& l = p->value;
        auto last = --l.end();
        std::cout << "(";
        const char* space = "";
        for (auto id = l.begin(); id != last; ++id)
        {
            std::cout << space << id->value;
            space = " ";
        }
        std::cout << " . " << last->value << ")";
    }
}

}
