/**
 * @file env.cc
 */

#include <cstdlib>
#include <iostream>

#include "env.h"
#include "exp.h"

const Exp& Env::get(const std::string& name)
{
    auto it = symbols.find(name);
    if (it != symbols.end())
    {
        return symbols[name];
    }
    else if (parent != nullptr)
    {
        return parent->get(name);
    }
    else
    {
        std::cerr << "Reference to unbound symbol: " << name << "\n";
        std::exit(1);
    }
}

void Env::let(const std::string& name, const Exp& val)
{
    symbols[name] = val;
}

std::unique_ptr<Env> Env::spawn()
{
    std::unique_ptr<Env> new_env(new Env);
    new_env->parent = this;
    return new_env;
}
