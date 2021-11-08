/**
 * @file exp.cc
 */

#include <iostream>
#include <variant>

#include "atom.h"
#include "datum.h"
#include "env.h"
#include "exp.h"
#include "if.h"
#include "lambda.h"
#include "literal.h"
#include "quote.h"
#include "set.h"

namespace Figure
{

void Exp::print() const
{
    std::cout << "[default exp]";
}

void Exp::error()
{
    std::cout << "Error parsing Exp.\n";
}

Ref<Exp> make_exp(Env& env, const Datum& datum)
{
    if (std::get_if<Bool>(&datum.value))
    {
        return make_atom<Bool>(datum);
    }
    else if (std::get_if<Char>(&datum.value))
    {
        return make_atom<Char>(datum);
    }
    else if (std::get_if<Number>(&datum.value))
    {
        return make_atom<Number>(datum);
    }
    else if (std::get_if<String>(&datum.value))
    {
        return make_atom<String>(datum);
    }
    else if (auto id = std::get_if<Id>(&datum.value))
    {
        auto val = env.get(*id);
        if (auto exp = std::get_if<Ref<Exp>>(&val))
        {
            if (auto val = *exp)
            {
                return val;
            }
            else
            {
                Exp err;
                err.error();
                exit(1);
            }
        }
        else if (std::get_if<Arg>(&val))
        {
            return make_atom<Id>(*id);
        }
        else if (std::get_if<KeywordQuote>(&val))
        {
            Exp err;
            err.error();
            exit(1);
        }
        else if (std::get_if<KeywordLambda>(&val))
        {
            Exp err;
            err.error();
            exit(1);
        }
        else if (std::get_if<KeywordIf>(&val))
        {
            Exp err;
            err.error();
            exit(1);
        }
        else if (std::get_if<KeywordSet>(&val))
        {
            Exp err;
            err.error();
            exit(1);
        }
        else if (std::get_if<KeywordDefine>(&val))
        {
            Exp err;
            err.error();
            exit(1);
        }
        else
        {
            Exp err;
            err.error();
            exit(1);
        }
    }
    else if (auto l = std::get_if<DatumList>(&datum.value))
    {
        const auto& first = l->front();
        const auto& id = std::get<Id>(first.value);
        const auto& val = env.get(id);
        if (std::get_if<KeywordQuote>(&val))
        {
            return make_quote(*l);
        }
        else if (std::get_if<KeywordIf>(&val))
        {
            return make_if(env, *l);
        }
        else if (std::get_if<KeywordLambda>(&val))
        {
            return make_lambda(env, *l);
        }
        else if (std::get_if<KeywordSet>(&val))
        {
            return make_set(env, *l);
        }
        else
        {
            Exp err;
            err.error();
            exit(1);
        }
    }
    else
    {
        Exp err;
        err.error();
        exit(1);
    }
}

}
