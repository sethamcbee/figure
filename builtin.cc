/**
 * @file builtin.cc
 * @brief Implementation of built-in functions.
 */

#include <cassert>
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

void eval_if(std::stack<Task>& tasks)
{
    Task& cur_task = tasks.top();
    Task* parent = cur_task.parent;
    auto& env = cur_task.env;
    auto& exp = cur_task.exp;
    auto& args = cur_task.args;

    // Check if the condition needs evaluated.
    if (args.size() == 1)
    {
        Task dep;
        dep.parent = &cur_task;
        dep.env = env;
        dep.exp = exp->get_list()->link;
        tasks.push(dep);
        return;
    }

    // Evaluate if statement.
    if (args[1]->get_bool())
    {
        Task next;
        next.parent = parent;
        next.env = env;
        next.exp = exp->get_list()->link->link;
        tasks.pop();
        tasks.push(next);
    }
    else
    {
        Task next;
        next.parent = parent;
        next.env = env;
        next.exp = exp->get_list()->link->link->link;
        tasks.pop();
        tasks.push(next);
    }
}

void eval_lambda(std::stack<Task>& tasks)
{
    Task& cur_task = tasks.top();
    Task* parent = cur_task.parent;
    auto& env = cur_task.env;
    auto& exp = cur_task.exp;

    Lambda* lam = new Lambda;
    lam->env = *env;

    // Get parameters.
    auto lam_args = exp->get_list()->link;
    if (lam_args->type == Type::LIST)
    {
        auto it = lam_args->get_list();
        while (it)
        {
            lam->args.push_back(it->get_string());
            it = it->link;
        }
    }
    else if (lam_args->type == Type::SYMBOL)
    {
        // Get single parameter.
        lam->args.push_back(lam_args->get_string());
    }
    else
    {
        std::cerr << "Improperly formatted lambda.\n";
        std::exit(1);
    }

    // Get bodies.
    auto& body = lam_args->link;
    while (body)
    {
        lam->bodies.push_back(body);
        body = body->link;
    }

    // Build and return expression.
    auto ret = Exp::spawn();
    ret->type = Type::LAMBDA;
    ret->data = lam;
    parent->args.push_back(ret);
    tasks.pop();
}

void eval_let(std::stack<Task>& tasks)
{
    Task& cur_task = tasks.top();
    Task* parent = cur_task.parent;
    auto& env = cur_task.env;
    auto& exp = cur_task.exp;
    auto& args = cur_task.args;

    // Check if all let expressions have been evaluated.
    if (args.size() == 1)
    {
        if (exp->get_list()->link->type == Type::SYMBOL)
        {
            Task dep;
            dep.parent = &cur_task;
            dep.env = env;
            dep.exp = exp->get_list()->link->link;
            tasks.push(dep);
            return;
        }
        else if (exp->get_list()->link->type == Type::LIST)
        {
            // Get bodies.
            std::stack<Task> deps;
            auto ind = exp->link->get_list();
            while (ind)
            {
                Task dep;
                dep.parent = &cur_task;
                dep.env = env;
                dep.exp = ind->get_list()->link;
                deps.push(dep);
                ind = ind->link;
            }

            // Add to evaluation stack in reverse order.
            while (!deps.empty())
            {
                tasks.push(deps.top());
                deps.pop();
            }

            return;
        }
    }

    // Build environment.
    auto new_env = env->spawn();
    size_t arg = 1;
    auto it = exp->get_list()->link;
    if (it->type == Type::LIST)
    {
        while (it)
        {
            new_env->let(it->get_list()->get_string(), args[arg]);
            it = it->link;
            ++arg;
        }
    }
    else if (it->type == Type::SYMBOL)
    {
        new_env->let(it->get_string(), args[1]);
    }

    // Build task.
    Task ret;
    ret.parent = parent;
    ret.env = new_env;
    ret.exp = exp->get_list()->link->link;
    tasks.pop();
    tasks.push(ret);
}

#if 0

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
    Number_Type val = args[0]->eval(env)->get_number();
    size_t count = args.size();
    for (size_t i = 1; i < count; ++i)
    {
        val += args[i]->eval(env)->get_number();
    }

    auto ret = Exp::spawn();
    ret->type = Type::NUMBER;
    ret->data = new Number_Type;
    Number_Type* p = (Number_Type*)ret->data;
    *p = val;

    return ret;
}

std::shared_ptr<Exp> builtin_sub(Env& env, std::vector<std::shared_ptr<Exp>>& args)
{
    Number_Type val = args[0]->eval(env)->get_number();
    size_t count = args.size();
    for (size_t i = 1; i < count; ++i)
    {
        val -= args[i]->eval(env)->get_number();
    }

    auto ret = Exp::spawn();
    ret->type = Type::NUMBER;
    ret->data = new Number_Type;
    Number_Type* p = (Number_Type*)ret->data;
    *p = val;

    return ret;
}

std::shared_ptr<Exp> builtin_mul(Env& env, std::vector<std::shared_ptr<Exp>>& args)
{
    Number_Type val = args[0]->eval(env)->get_number();
    size_t count = args.size();
    for (size_t i = 1; i < count; ++i)
    {
        val *= args[i]->eval(env)->get_number();
    }

    auto ret = Exp::spawn();
    ret->type = Type::NUMBER;
    ret->data = new Number_Type;
    Number_Type* p = (Number_Type*)ret->data;
    *p = val;

    return ret;
}

std::shared_ptr<Exp> builtin_div(Env& env, std::vector<std::shared_ptr<Exp>>& args)
{
    Number_Type val = args[0]->eval(env)->get_number();
    size_t count = args.size();
    for (size_t i = 1; i < count; ++i)
    {
        val /= args[i]->eval(env)->get_number();
    }

    auto ret = Exp::spawn();
    ret->type = Type::NUMBER;
    ret->data = new Number_Type;
    Number_Type* p = (Number_Type*)ret->data;
    *p = val;

    return ret;
}

#endif
