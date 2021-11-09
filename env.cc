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
        Ref<Exp> ret{nullptr};
        return ret;
    }
}

void Env::set(const Key& k, const Value& v)
{
    auto cur_env = this;
    while (cur_env)
    {
        auto e = cur_env->map.find(k);
        if (e != cur_env->map.end())
        {
            // Found the variable.
            cur_env->map[k] = v;
            return;
        }
        else
        {
            cur_env = cur_env->parent;
        }
    }
    // Variable not found.
    std::cerr << "Invalid variable set.\n";
    std::exit(1);
}

void Env::bind(const Key& k, const Value& v)
{
    map[k] = v;
}

Env init_env()
{
    Env env;

    env.map[Id{"quote"}] = KeywordQuote{};
    env.map[Id{"lambda"}] = KeywordLambda{};
    env.map[Id{"if"}] = KeywordIf{};
    env.map[Id{"set!"}] = KeywordSet{};
    env.map[Id{"define"}] = KeywordDefine{};
    env.map[Id{"define-syntax"}] = KeywordDefineSyntax{};

    return env;
}

}
