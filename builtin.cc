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
#include "lambda.h"

Exp* builtin_lambda(Env& env, std::vector<Exp*>& args)
{
    Lambda* lam = new Lambda;
    lam->env = env;

    // Get parameters.
    if (args[0]->type == Type::LIST)
    {
        Exp* it = (Exp*)args[0]->data;
        while (it)
        {
            lam->args.push_back(it->get_string());
            it = it->link;
        }
    }

    // Get bodies.
    size_t count = args.size();
    for (size_t i = 1; i < count; ++i)
    {
        lam->bodies.push_back(args[i]);
    }

    // Build expression.
    Exp* exp = new Exp;
    exp->type = Type::LAMBDA;
    exp->data = lam;

    return exp;
}

Exp* builtin_let(Env& env, std::vector<Exp*>& args)
{
    // Build new environment.
    auto new_env = env.spawn();
    if (args[0]->type == Type::LIST)
    {
        Exp* ind = (Exp*)args[0]->data;
        while (ind)
        {
            Exp* var = (Exp*)ind->data;
            new_env->let(var->get_string(), var->link->eval(env));
            ind = ind->link;
        }
        return args[1]->eval(*new_env);
    }
    else if (args[0]->type == Type::SYMBOL)
    {
        new_env->let(args[0]->get_string(), args[1]->eval(env));
        return args[2]->eval(*new_env);
    }
    else
    {
        std::cerr << "Poorly formed let expression.\n";
        std::exit(1);
    }
}

Exp* builtin_if(Env& env, std::vector<Exp*>& args)
{
    // Evaluate condition.
    bool b;

    Exp* cond = args[0]->eval(env);
    if (cond->type == Type::BOOLEAN)
    {
        b = cond->get_bool();
    }
    else
    {
        b = true;
    }

    if (b)
    {
        return args[1]->eval(env);
    }
    else
    {
        return args[2]->eval(env);
    }
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

Exp* builtin_add(Env& env, std::vector<Exp*>& args)
{
    Number_Type n0 = args[0]->eval(env)->get_number();
    Number_Type n1 = args[1]->eval(env)->get_number();

    Exp* ret = new Exp;
    ret->type = Type::NUMBER;
    ret->data = new Number_Type;
    Number_Type* p = (Number_Type*)ret->data;
    *p = n0 + n1;

    return ret;
}

Exp* builtin_sub(Env& env, std::vector<Exp*>& args)
{
    Number_Type n0 = args[0]->eval(env)->get_number();
    Number_Type n1 = args[1]->eval(env)->get_number();

    Exp* ret = new Exp;
    ret->type = Type::NUMBER;
    ret->data = new Number_Type;
    Number_Type* p = (Number_Type*)ret->data;
    *p = n0 - n1;

    return ret;
}

Exp* builtin_mul(Env& env, std::vector<Exp*>& args)
{
    Number_Type n0 = args[0]->eval(env)->get_number();
    Number_Type n1 = args[1]->eval(env)->get_number();

    Exp* ret = new Exp;
    ret->type = Type::NUMBER;
    ret->data = new Number_Type;
    Number_Type* p = (Number_Type*)ret->data;
    *p = n0 * n1;

    return ret;
}

Exp* builtin_div(Env& env, std::vector<Exp*>& args)
{
    Number_Type n0 = args[0]->eval(env)->get_number();
    Number_Type n1 = args[1]->eval(env)->get_number();

    Exp* ret = new Exp;
    ret->type = Type::NUMBER;
    ret->data = new Number_Type;
    Number_Type* p = (Number_Type*)ret->data;
    *p = n0 / n1;

    return ret;
}
