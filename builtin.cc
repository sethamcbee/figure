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

/// Replace all instances of substr in src
static void replace_all(
    std::string& str,
    const std::string& from,
    const std::string& to)
{
    size_t len = from.length();
    size_t pos = str.find(from);
    while (pos != std::string::npos)
    {
        str.replace(pos, len, to);
        pos = str.find(from);
    }
}

std::shared_ptr<Exp> builtin_lambda(Env& env, std::vector<std::shared_ptr<Exp>>& args)
{
    Lambda* lam = new Lambda;
    lam->env = env;

    // Get parameters.
    if (args[0]->type == Type::LIST)
    {
        auto it = args[0]->get_list();
        while (it)
        {
            lam->args.push_back(it->get_string());
            it = it->link;
        }
    }
    else if (args[0]->type == Type::SYMBOL)
    {
        // Get single parameter.
        lam->args.push_back(args[0]->get_string());
    }
    else
    {
        std::cerr << "Error: Incorrectly constructed lambda.\n";
        std::exit(1);
    }

    // Get bodies.
    size_t count = args.size();
    for (size_t i = 1; i < count; ++i)
    {
        lam->bodies.push_back(args[i]);
    }

    // Build expression.
    std::shared_ptr<Exp> exp = Exp::spawn();
    exp->type = Type::LAMBDA;
    exp->data = lam;

    return exp;
}

std::shared_ptr<Exp> builtin_let(Env& env, std::vector<std::shared_ptr<Exp>>& args)
{
    // Build new environment.
    auto new_env = env.spawn();
    if (args[0]->type == Type::LIST)
    {
        auto ind = args[0]->get_list();
        while (ind)
        {
            auto var = ind->get_list();
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

std::shared_ptr<Exp> builtin_if(Env& env, std::vector<std::shared_ptr<Exp>>& args)
{
    // Evaluate condition.
    bool b;

    auto cond = args[0]->eval(env);
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

std::shared_ptr<Exp> builtin_display(Env& env, std::vector<std::shared_ptr<Exp>>& args)
{
    auto val = args[0]->eval(env);
    if (val->type == Type::NUMBER)
    {
        std::cout << val->get_number();
    }
    else if (val->type == Type::BOOLEAN)
    {
        std::cout << val->get_bool();
    }
    else if (val->type == Type::STRING)
    {
        // Escape newlines.
        if (val->get_string().find("\\n") == std::string::npos)
        {
            std::cout << val->get_string();
        }
        else
        {
            std::string edit = val->get_string();
            replace_all(edit, "\\n", "\n");
            std::cout << edit;
        }
    }

    // Return evaluated expression.
    return val;
}

std::shared_ptr<Exp> builtin_add(Env& env, std::vector<std::shared_ptr<Exp>>& args)
{
    Number_Type n0 = args[0]->eval(env)->get_number();
    Number_Type n1 = args[1]->eval(env)->get_number();

    auto ret = Exp::spawn();
    ret->type = Type::NUMBER;
    ret->data = new Number_Type;
    Number_Type* p = (Number_Type*)ret->data;
    *p = n0 + n1;

    return ret;
}

std::shared_ptr<Exp> builtin_sub(Env& env, std::vector<std::shared_ptr<Exp>>& args)
{
    Number_Type n0 = args[0]->eval(env)->get_number();
    Number_Type n1 = args[1]->eval(env)->get_number();

    auto ret = Exp::spawn();
    ret->type = Type::NUMBER;
    ret->data = new Number_Type;
    Number_Type* p = (Number_Type*)ret->data;
    *p = n0 - n1;

    return ret;
}

std::shared_ptr<Exp> builtin_mul(Env& env, std::vector<std::shared_ptr<Exp>>& args)
{
    Number_Type n0 = args[0]->eval(env)->get_number();
    Number_Type n1 = args[1]->eval(env)->get_number();

    auto ret = Exp::spawn();
    ret->type = Type::NUMBER;
    ret->data = new Number_Type;
    Number_Type* p = (Number_Type*)ret->data;
    *p = n0 * n1;

    return ret;
}

std::shared_ptr<Exp> builtin_div(Env& env, std::vector<std::shared_ptr<Exp>>& args)
{
    Number_Type n0 = args[0]->eval(env)->get_number();
    Number_Type n1 = args[1]->eval(env)->get_number();

    auto ret = Exp::spawn();
    ret->type = Type::NUMBER;
    ret->data = new Number_Type;
    Number_Type* p = (Number_Type*)ret->data;
    *p = n0 / n1;

    return ret;
}
