/**
 * @file pattern.cc
 */

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
    else if (auto lp = std::get_if<DatumList>(&datum.value))
    {
        const auto& l = *lp;
    }
    else if (auto p = std::get_if<DatumPair>(&datum.value))
    {
        const auto& l = p->value;
    }
    else
    {
        std::cerr << "Invalid pattern.\n";
        exit(1);
    }
}

}
