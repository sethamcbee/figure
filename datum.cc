/**
 * @file datum.cc
 */

#include <iostream>

#include "datum.h"
#include "sexp.h"

namespace Figure
{

DatumPair::operator DatumList&()
{
    return value;
}

DatumPair::operator DatumList() const
{
    return value;
}

Datum::Datum()
{
    value = Bool{false};
    pos = 0;
}

Datum::Datum(Value v, Location p, const Sexp* s)
{
    value = v;
    pos = p;
    src = s;
}

Datum::Datum(const Sexp& s)
{
    pos = s.pos;
    src = &s;

    if (auto b = std::get_if<Bool>(&s.value))
    {
        value = *b;
    }
    else if (auto c = std::get_if<Char>(&s.value))
    {
        value = *c;
    }
    else if (auto n = std::get_if<Number>(&s.value))
    {
        value = *n;
    }
    else if (auto str = std::get_if<String>(&s.value))
    {
        value = *str;
    }
    else if (auto id = std::get_if<Id>(&s.value))
    {
        if ((std::string)*id == ".")
        {
            error("Invalid pair formation.");
        }
        value = *id;
    }
    else if (auto l = std::get_if<SexpList>(&s.value))
    {
        // Check if this might be a pair or improper list.
        if (l->size() < 3)
        {
            value = DatumList{};
            auto& list = std::get<DatumList>(value);
            for (const auto& sexp : *l)
            {
                auto datum = Datum{sexp};
                list.push_back(datum);
            }
        }
        else
        {
            auto dot = --l->end();
            --dot;
            auto id = std::get_if<Id>(&dot->value);
            if (id && (std::string)*id == ".")
            {
                // Pair/improper list.
                value = DatumPair{};
                auto& pair = std::get<DatumPair>(value);
                auto& list = pair.value;
                for (auto it = l->begin(); it != dot; ++it)
                {
                    auto datum = Datum{*it};
                    list.push_back(datum);
                }
                // Skipped dot.
                auto datum = Datum{l->back()};
                list.push_back(datum);
            }
            else
            {
                // Proper list.
                value = DatumList{};
                auto& list = std::get<DatumList>(value);
                for (const auto& sexp : *l)
                {
                    auto datum = Datum{sexp};
                    list.push_back(datum);
                }
            }
        }
    }
    else if (auto v = std::get_if<SexpVector>(&s.value))
    {
        const SexpList& l = v->value;
        value = DatumVector{};
        auto& vec = std::get<DatumVector>(value);
        vec.resize(l.size());
        for (const auto& sexp : l)
        {
            auto datum = Datum{sexp};
            vec.push_back(datum);
        }
    }
    else
    {
        error("Invalid s-exp type.");
    }
}

void Datum::print(std::ostream& o) const
{
    if (auto id = std::get_if<Id>(&value))
    {
        o << id->value;
    }
    else if (auto b = std::get_if<Bool>(&value))
    {
        o << b->value;
    }
    else if (auto n = std::get_if<Number>(&value))
    {
        o << n->value;
    }
    else if (auto c = std::get_if<Char>(&value))
    {
        o << c->value;
    }
    else if (auto s = std::get_if<String>(&value))
    {
        o << "\"" << s->value << "\"";
    }
    else if (auto l = std::get_if<DatumList>(&value))
    {
        o << "(";
        const char* space = "";
        for (const auto& datum : *l)
        {
            o << space;
            datum.print(o);
            space = " ";
        }
        o << ")";
    }
    else if (auto p = (DatumList*)std::get_if<DatumPair>(&value))
    {
        o << "(";
        const char* space = "";
        auto last = --p->end();
        for (auto datum = p->begin(); datum != last; ++datum)
        {
            o << space;
            datum->print(o);
            space = " ";
        }
        o << " . ";
        last->print(o);
        o << ")";
    }
    else if (auto v = std::get_if<DatumVector>(&value))
    {
        o << "#(";
        const char* space = "";
        for (const auto& datum : *v)
        {
            o << space;
            datum.print(o);
            space = " ";
        }
        o << ")";
    }
    else
    {
        o << "[unprintable]";
    }
}

void Datum::error()
{
    error("Unspecified error.");
}

void Datum::error(std::string_view e)
{
    if (src)
    {
        std::cerr << "\nAt s-exp: ";
        src->print(std::cerr);
    }
    std::cerr << "\nDatum parsing error: " << e << std::endl;
    exit(1);
}

}
