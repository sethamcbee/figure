/**
 * @file exp.cc
 */

#include <iostream>

#include "exp.h"

namespace Figure
{

Exp::Exp()
{
    value = Bool{false};
}

Exp::Exp(const Env* ctx, const Datum& d)
{
    env.parent = ctx;
    
    if (auto b = std::get_if<Bool>(&d.value))
    {
        value = *b;
    }
    else if (auto c = std::get_if<Char>(&d.value))
    {
        value = *c;
    }
    else if (auto n = std::get_if<Number>(&d.value))
    {
        value = *n;
    }
    else if (auto str = std::get_if<String>(&d.value))
    {
        value = *str;
    }
    else if (auto id = std::get_if<Id>(&d.value))
    {
        auto val = env.get(*id);
        if (auto kwd = std::get_if<KeywordQuote>(&val))
        {
            std::cerr << "\nInvalid keyword.\n";
            exit(1);
        }
        else if (auto exp = *std::get_if<Exp*>(&val))
        {
        }
        else
        {
            std::cerr << "\nIdentifier not found.\n";
            exit(1);
        }
    }
    else if (auto l = std::get_if<DatumList>(&d.value))
    {
        const auto& first = l->front();
        if (auto id = std::get_if<Id>(&first.value))
        {
            auto val = env.get(*id);
            if (std::get_if<KeywordQuote>(&val))
            {
                // TODO: Check length.
                auto it = l->begin();
                ++it;
                value = Quote{*it};
            }
            else if (std::get_if<KeywordLambda>(&val))
            {
                value = Lambda{env, d};
            }
            else if (std::get_if<KeywordIf>(&val))
            {
                value = If{env, d};
            }
            else
            {
                std::cerr << "\nInvalid list.\n";
                exit(1);
            }
        }
        else
        {
            std::cerr << "\nInvalid list element.\n";
            exit(1);
        }
    }
    else
    {
        std::cerr << "\nInvalid datum.\n";
        exit(1);
    }
}

void Exp::print() const
{
    if (auto b = std::get_if<Bool>(&value))
    {
        std::cout << b->value;
    }
    else if (auto c = std::get_if<Char>(&value))
    {
        std::cout << c->value;
    }
    else if (auto n = std::get_if<Number>(&value))
    {
        std::cout << n->value;
    }
    else if (auto str = std::get_if<String>(&value))
    {
        std::cout << "\"" << str->value << "\"";
    }
    else if (auto id = std::get_if<Id>(&value))
    {
        std::cout << id->value;
    }
    else if (auto quote = std::get_if<Quote>(&value))
    {
        quote->print();
    }
    else if (auto lambda = std::get_if<Lambda>(&value))
    {
        lambda->print();
    }
    else if (auto if_form = std::get_if<If>(&value))
    {
        if_form->print();
    }
}

}
