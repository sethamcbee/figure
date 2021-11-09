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

void Exp::print(std::ostream& o) const
{
    o << "[default exp]";
}

Ref<Exp> Exp::eval(const Env& env) const
{
    return nullptr;
}

void Exp::error() const
{
    error("Unspecified error.");
}

void Exp::error(const std::string& err) const
{
    std::cerr << "Error parsing expression: ";
    std::cerr << err;
    std::cerr << std::endl;
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
                return make_atom<Id>(*id);
            }
            else
            {
                return nullptr;
            }
        }
        else if (std::get_if<Arg>(&val))
        {
            return make_atom<Id>(*id);
        }
        else if (std::get_if<KeywordQuote>(&val))
        {
            return nullptr;
        }
        else if (std::get_if<KeywordLambda>(&val))
        {
            return nullptr;
        }
        else if (std::get_if<KeywordIf>(&val))
        {
            return nullptr;
        }
        else if (std::get_if<KeywordSet>(&val))
        {
            return nullptr;
        }
        else if (std::get_if<KeywordDefine>(&val))
        {
            return nullptr;
        }
        else if (std::get_if<KeywordDefineSyntax>(&val))
        {
            return nullptr;
        }
        else
        {
            return nullptr;
        }
    }
    else if (auto l = std::get_if<DatumList>(&datum.value))
    {
        const auto& first = l->front();
        const auto& id = std::get<Id>(first.value);
        const auto& val = env.get(id);
        if (std::get_if<KeywordQuote>(&val))
        {
            return make_quote(datum);
        }
        else if (std::get_if<KeywordIf>(&val))
        {
            return make_if(env, datum);
        }
        else if (std::get_if<KeywordLambda>(&val))
        {
            return make_lambda(env, datum);
        }
        else if (std::get_if<KeywordSet>(&val))
        {
            return make_set(env, datum);
        }
        else if (std::get_if<KeywordDefine>(&val))
        {
            return nullptr;
        }
        else if (std::get_if<KeywordDefineSyntax>(&val))
        {
            return nullptr;
        }
        else
        {
            return nullptr;
        }
    }
    else
    {
        return nullptr;
    }
}

}
