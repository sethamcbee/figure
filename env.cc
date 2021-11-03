/**
 * @file env.cc
 */

#include "env.h"

namespace Figure
{

Env::Value Env::get(const Key& k) const
{
    auto e = map.find(k);
    if (e != map.end())
    {
        return e->second;
    }
    else if (parent)
    {
        return parent->get(k);
    }
    else
    {
        Exp* ret{nullptr};
        return ret;
    }
}

Env init_env()
{
    Env env;

    env.map[Id{"quote"}] = KeywordQuote{};
    env.map[Id{"lambda"}] = KeywordLambda{};
    env.map[Id{"if"}] = KeywordIf{};
    env.map[Id{"set!"}] = KeywordSet{};

    return env;
}

}
