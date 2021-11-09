/**
 * @file pattern.cc
 */

#include "env.h"
#include "pattern.h"

namespace Figure
{

PatternId::PatternId(const Id& v)
{
    value = v;
}

void PatternId::print(std::ostream& o) const
{
    o << value;
}

PatternString::PatternString(const String& v)
{
    value = v;
}

void PatternString::print(std::ostream& o) const
{
    o << value;
}

PatternChar::PatternChar(const Char& v)
{
    value = v;
}

void PatternChar::print(std::ostream& o) const
{
    o << value;
}

PatternBool::PatternBool(const Bool& v)
{
    value = v;
}

void PatternBool::print(std::ostream& o) const
{
    o << value;
}

PatternNumber::PatternNumber(const Number& v)
{
    value = v;
}

void PatternNumber::print(std::ostream& o) const
{
    o << value;
}

void PatternUnderscore::print(std::ostream& o) const
{
    o << "_";
}

PatternList::PatternList(Env& env, const Datum& datum)
{
    const auto& l = std::get<DatumList>(datum.value);
    for (const auto& d : l)
    {
        auto pat = make_pattern(env, d);
        value.push_back(pat);
    }
}

void PatternList::print(std::ostream& o) const
{
    o << "(";
    const char* space = "";
    for (const auto& pat : value)
    {
        o << space;
        pat->print(o);
        space = " ";
    }
    o << ")";
}

Ref<Pattern> make_pattern(Env& env, const Datum& datum)
{
    if (auto id = std::get_if<Id>(&datum.value))
    {
        if (id->value == "_")
        {
            PatternUnderscore tmp{};
            return make_ref(tmp);
        }
        else if (id->value == "...")
        {
            return nullptr;
        }
        else
        {
            PatternId tmp{*id};
            return make_ref(tmp);
        }
    }
    else if (auto str = std::get_if<String>(&datum.value))
    {
        PatternString tmp{*str};
        return make_ref(tmp);
    }
    else if (auto ch = std::get_if<Char>(&datum.value))
    {
        PatternChar tmp{*ch};
        return make_ref(tmp);
    }
    else if (auto b = std::get_if<Bool>(&datum.value))
    {
        PatternBool tmp{*b};
        return make_ref(tmp);
    }
    else if (auto n = std::get_if<Number>(&datum.value))
    {
        PatternNumber tmp{*n};
        return make_ref(tmp);
    }
    else if (auto lp = std::get_if<DatumList>(&datum.value))
    {
        const auto& l = *lp;

        // Check for ellipses.
        DatumList::const_iterator ellipses{l.end()};
        for (auto e = l.begin(); e != l.end(); ++e)
        {
            if (auto id = std::get_if<Id>(&e->value))
            {
                if (id->value == "...")
                {
                    ellipses = e;
                }
            }
        }
        if (ellipses != l.end())
        {
            // Ellipses not supported currently.
            return nullptr;
        }
        else
        {
            PatternList tmp{env, datum};
            return make_ref(tmp);
        }
    }
    else if (auto p = std::get_if<DatumPair>(&datum.value))
    {
        const auto& l = p->value;
        return nullptr;
    }
    else
    {
        std::cerr << "Invalid pattern.\n";
        exit(1);
    }
}

}
