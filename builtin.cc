/**
 * @file builtin.cc
 * @brief Implementation of built-in functions.
 */

#include <cstdlib>
#include <iostream>
#include <vector>

#include "builtin.h"
#include "env.h"
#include "exp.h"

Exp* builtin_let(Env& env, std::vector<Exp*>& args)
{
    // Get args.
    std::string id = args[0]->get_string();
    Exp* val = args[1]->eval(env);

    // Build new environment and execute.
    auto new_env = env.spawn();
    new_env->let(id, *val);
    return args[2]->eval(*new_env);
}

Exp* builtin_print(Env& env, std::vector<Exp*>& args)
{
    Exp* val = args[0]->eval(env);
    if (val->type == Type::NUMBER)
    {
        std::cout << val->get_number() << "\n";
    }
    else if (val->type == Type::BOOLEAN)
    {
        std::cout << val->get_bool() << "\n";
    }
    else if (val->type == Type::STRING)
    {
        std::cout << val->get_string() << "\n";
    }

    // Return evaluated expression.
    return val;
}
