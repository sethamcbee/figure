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

void eval_if(std::stack<std::shared_ptr<Task>>& tasks)
{
    auto cur_task = tasks.top();
    auto parent = cur_task->parent;
    auto env = cur_task->env;
    auto exp = cur_task->exp;
    auto& args = cur_task->args;

    // Check if the condition needs evaluated.
    if (args.size() == 1)
    {
        std::shared_ptr<Task> dep(new Task);
        dep->parent = cur_task;
        dep->env = env;
        dep->exp = exp->get_list()->link;
        tasks.push(dep);
        return;
    }

    // Evaluate if statement.
    if (args[1]->get_bool())
    {
        std::shared_ptr<Task> next(new Task);
        next->parent = parent;
        next->env = env;
        next->exp = exp->get_list()->link->link;
        tasks.pop();
        tasks.push(next);
    }
    else
    {
        std::shared_ptr<Task> next(new Task);
        next->parent = parent;
        next->env = env;
        next->exp = exp->get_list()->link->link->link;
        tasks.pop();
        tasks.push(next);
    }
}

void eval_lambda(std::stack<std::shared_ptr<Task>>& tasks)
{
    auto cur_task = tasks.top();
    auto parent = cur_task->parent;
    auto env = cur_task->env;
    auto exp = cur_task->exp;

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
    if (parent)
    {
        parent->args.push_back(ret);
    }
    tasks.pop();
}

void eval_let(std::stack<std::shared_ptr<Task>>& tasks)
{
    auto cur_task = tasks.top();
    auto parent = cur_task->parent;
    auto env = cur_task->env;
    auto exp = cur_task->exp;
    auto& args = cur_task->args;

    // Check if all let expressions have been evaluated.
    if (args.size() == 1)
    {
        if (exp->get_list()->link->type == Type::SYMBOL)
        {
            std::shared_ptr<Task> dep(new Task);
            dep->parent = cur_task;
            dep->env = env;
            dep->exp = exp->get_list()->link->link;
            tasks.push(dep);
            return;
        }
        else if (exp->get_list()->link->type == Type::LIST)
        {
            // Get bodies.
            std::stack<std::shared_ptr<Task>> deps;
            auto ind = exp->get_list()->link->get_list();
            while (ind)
            {
                std::shared_ptr<Task> dep(new Task);
                dep->parent = cur_task;
                dep->env = env;
                dep->exp = ind->get_list()->link;
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
    std::shared_ptr<Exp> body;
    auto new_env = env->spawn();
    size_t arg = 1;
    auto it = exp->get_list()->link;
    if (it->type == Type::LIST)
    {
        it = it->get_list();
        while (it)
        {
            new_env->let(it->get_list()->get_string(), args[arg]);
            it = it->link;
            ++arg;
        }
        body = exp->get_list()->link->link;
    }
    else if (it->type == Type::SYMBOL)
    {
        new_env->let(it->get_string(), args[1]);
        body = exp->get_list()->link->link->link;
    }

    // Build task.
    std::shared_ptr<Task> ret(new Task);
    ret->parent = parent;
    ret->env = new_env;
    ret->exp = body;
    tasks.pop();
    tasks.push(ret);
}

void eval_add(std::stack<std::shared_ptr<Task>>& tasks)
{
    auto cur_task = tasks.top();
    auto parent = cur_task->parent;
    auto env = cur_task->env;
    auto exp = cur_task->exp;
    auto& args = cur_task->args;

    // Check if arguments need evaluated.
    if (args.size() == 1)
    {
        std::stack<std::shared_ptr<Task>> arg_stack;
        auto it = exp->get_list()->link;
        while (it)
        {
            std::shared_ptr<Task> dep(new Task);
            dep->parent = cur_task;
            dep->env = env;
            dep->exp = it;
            arg_stack.push(dep);
            it = it->link;
        }

        while (!arg_stack.empty())
        {
            tasks.push(arg_stack.top());
            arg_stack.pop();
        }

        return;
    }

    // Calculate sum.
    Number_Type sum = 0;
    size_t arg_count = args.size();
    for (size_t i = 1; i < arg_count; ++i)
    {
        sum += args[i]->get_number();
    }

    // Build and return expression.
    auto ret = Exp::spawn();
    ret->type = Type::NUMBER;
    ret->data = new Number_Type;
    Number_Type* p = (Number_Type*)ret->data;
    *p = sum;
    tasks.pop();

    if (parent)
    {
        parent->args.push_back(ret);
    }
}

void eval_sub(std::stack<std::shared_ptr<Task>>& tasks)
{
    auto cur_task = tasks.top();
    auto parent = cur_task->parent;
    auto env = cur_task->env;
    auto exp = cur_task->exp;
    auto& args = cur_task->args;

    // Check if arguments need evaluated.
    if (args.size() == 1)
    {
        std::stack<std::shared_ptr<Task>> arg_stack;
        auto it = exp->get_list()->link;
        while (it)
        {
            std::shared_ptr<Task> dep(new Task);
            dep->parent = cur_task;
            dep->env = env;
            dep->exp = it;
            arg_stack.push(dep);
            it = it->link;
        }

        while (!arg_stack.empty())
        {
            tasks.push(arg_stack.top());
            arg_stack.pop();
        }

        return;
    }

    // Calculate difference.
    Number_Type sum = args[1]->get_number();
    size_t arg_count = args.size();
    for (size_t i = 2; i < arg_count; ++i)
    {
        sum -= args[i]->get_number();
    }

    // Build and return expression.
    auto ret = Exp::spawn();
    ret->type = Type::NUMBER;
    ret->data = new Number_Type;
    Number_Type* p = (Number_Type*)ret->data;
    *p = sum;
    tasks.pop();

    if (parent)
    {
        parent->args.push_back(ret);
    }
}

void eval_mul(std::stack<std::shared_ptr<Task>>& tasks)
{
    auto cur_task = tasks.top();
    auto parent = cur_task->parent;
    auto env = cur_task->env;
    auto exp = cur_task->exp;
    auto& args = cur_task->args;

    // Check if arguments need evaluated.
    if (args.size() == 1)
    {
        std::stack<std::shared_ptr<Task>> arg_stack;
        auto it = exp->get_list()->link;
        while (it)
        {
            std::shared_ptr<Task> dep(new Task);
            dep->parent = cur_task;
            dep->env = env;
            dep->exp = it;
            arg_stack.push(dep);
            it = it->link;
        }

        while (!arg_stack.empty())
        {
            tasks.push(arg_stack.top());
            arg_stack.pop();
        }

        return;
    }

    // Calculate result.
    Number_Type n = args[1]->get_number();
    size_t arg_count = args.size();
    for (size_t i = 2; i < arg_count; ++i)
    {
        n *= args[i]->get_number();
    }

    // Build and return expression.
    auto ret = Exp::spawn();
    ret->type = Type::NUMBER;
    ret->data = new Number_Type;
    Number_Type* p = (Number_Type*)ret->data;
    *p = n;
    tasks.pop();

    if (parent)
    {
        parent->args.push_back(ret);
    }
}

void eval_div(std::stack<std::shared_ptr<Task>>& tasks)
{
    auto cur_task = tasks.top();
    auto parent = cur_task->parent;
    auto env = cur_task->env;
    auto exp = cur_task->exp;
    auto& args = cur_task->args;

    // Check if arguments need evaluated.
    if (args.size() == 1)
    {
        std::stack<std::shared_ptr<Task>> arg_stack;
        auto it = exp->get_list()->link;
        while (it)
        {
            std::shared_ptr<Task> dep(new Task);
            dep->parent = cur_task;
            dep->env = env;
            dep->exp = it;
            arg_stack.push(dep);
            it = it->link;
        }

        while (!arg_stack.empty())
        {
            tasks.push(arg_stack.top());
            arg_stack.pop();
        }

        return;
    }

    // Calculate result.
    Number_Type n = args[1]->get_number();
    size_t arg_count = args.size();
    for (size_t i = 2; i < arg_count; ++i)
    {
        n /= args[i]->get_number();
    }

    // Build and return expression.
    auto ret = Exp::spawn();
    ret->type = Type::NUMBER;
    ret->data = new Number_Type;
    Number_Type* p = (Number_Type*)ret->data;
    *p = n;
    tasks.pop();

    if (parent)
    {
        parent->args.push_back(ret);
    }
}

void eval_numeq(std::stack<std::shared_ptr<Task>>& tasks)
{
    auto cur_task = tasks.top();
    auto parent = cur_task->parent;
    auto env = cur_task->env;
    auto exp = cur_task->exp;
    auto& args = cur_task->args;

    // Check if arguments need evaluated.
    if (args.size() == 1)
    {
        std::stack<std::shared_ptr<Task>> arg_stack;
        auto it = exp->get_list()->link;
        while (it)
        {
            std::shared_ptr<Task> dep(new Task);
            dep->parent = cur_task;
            dep->env = env;
            dep->exp = it;
            arg_stack.push(dep);
            it = it->link;
        }

        while (!arg_stack.empty())
        {
            tasks.push(arg_stack.top());
            arg_stack.pop();
        }

        return;
    }

    // Calculate result.
    bool equal = true;
    Number_Type n = args[1]->get_number();
    size_t arg_count = args.size();
    for (size_t i = 2; i < arg_count; ++i)
    {
        if (n != args[i]->get_number())
        {
            equal = false;
            break;
        }
    }

    // Build and return expression.
    auto ret = Exp::spawn();
    ret->type = Type::BOOLEAN;
    ret->data = new bool;
    bool* p = (bool*)ret->data;
    *p = equal;
    tasks.pop();

    if (parent)
    {
        parent->args.push_back(ret);
    }
}

#if 0

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
