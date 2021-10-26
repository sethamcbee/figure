/**
 * @file lambda.h
 */

#pragma once

#include <list>
#include <memory>
#include <variant>

#include "sexp.h"

namespace Figure
{

class Exp;

using ExpList = std::list<Exp>;
using ExpPtr = std::unique_ptr<Exp>;

struct NoArgs {};

struct AnyArgs
{
    AnyArgs(const Id& s) : id(s)
    {}

    Id id;
};

struct NArgs
{
    std::list<Id> id;
};

struct RestArgs
{
    std::list<Id> id;
    Id rest;
};

using FormalsVal = std::variant<NoArgs, AnyArgs, NArgs, RestArgs>;

struct Formals
{
    Formals() {}

    Formals(const Sexp& sexp)
    {
        if (auto id = std::get_if<Id>(&sexp.value))
        {
            value = AnyArgs{*id};
        }
        else if (auto l = std::get_if<SexpList>(&sexp.value))
        {
            if (l->size() == 0)
            {
                value = NoArgs{};
            }
            else
            {
                // Check for rest argument.
                size_t dot_count{0};
                for (auto i : *l)
                {
                    if (auto dot = std::get_if<Dot>(&i.value))
                    {
                        ++dot_count;
                    }
                    if (dot_count > 1)
                    {
                        error("Improper pair.");
                    }
                }

                if (dot_count == 1)
                {
                    // Potential pair found.
                    auto it = l->end();
                    std::advance(it, -2);
                    if (auto dot = std::get_if<Dot>(&it->value))
                    {
                        value = RestArgs{};
                        auto& v = std::get<RestArgs>(value);
                        for (auto i = l->begin(); i != it; ++i)
                        {
                            if (auto dot = std::get_if<Dot>(&i->value))
                            {
                                // Skip dot.
                            }
                            else if (auto id = std::get_if<Id>(&i->value))
                            {
                                v.id.push_back(*id);
                            }
                            else
                            {
                                error();
                            }
                        }
                        return;
                    }
                    else
                    {
                        error("Improper rest arg.");
                    }
                }
            }
        }
        else
        {
            error();
        }
    }

    void error()
    {
        error("Unspecified error.");
    }

    void error(std::string_view msg)
    {
        std::cerr << "formals parsing error: " << msg << std::endl;
        exit(1);
    }

    FormalsVal value;
};

struct Lambda
{
    Lambda(const Sexp& sexp)
    {
        const auto& l = std::get<SexpList>(sexp.value);
        auto it = l.begin();
        ++it;
        formals = Formals(*it);
        for (; it != l.end(); ++it)
        {
        }
    }

    Formals formals;
    ExpList body;
};

}
