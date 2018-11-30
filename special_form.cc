/**
 * @file special_form.cc
 */

#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>

#include "env.h"
#include "exp.h"
#include "lambda.h"
#include "special_form.h"
#include "type.h"

void special_lambda(std::stack<std::shared_ptr<Task>>& tasks)
{
    auto cur_task = tasks.top();
    auto env = cur_task->env;
    auto exp = cur_task->exp->get_list();
    auto& args = cur_task->args;
    auto& result = cur_task->result;

    Lambda* lam = new Lambda;
    lam->env = env;

    // Get parameters.
    auto lam_args = exp->link;
    if (lam_args->type == Type::LIST)
    {
        // Get multiple parameters.
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
    if (result)
    {
        *result = ret;
    }
    tasks.pop();
}

void special_letrec(std::stack<std::shared_ptr<Task>>& tasks)
{
    auto cur_task = tasks.top();
    auto env = cur_task->env;
    auto exp = cur_task->exp->get_list();
    auto& args = cur_task->args;
    auto& result = cur_task->result;
    size_t& eval_count = cur_task->eval_count;

    std::stack<std::shared_ptr<Exp>> bodies;

    // Check whether assigned values need evaluated.
    if (args.size() == 0)
    {
        args.resize(exp->length());

        size_t eval_needed = 0;
        if (exp->link->type == Type::SYMBOL)
        {
            if (!exp->link->link->self_eval())
            {
                args.resize(1);

                std::shared_ptr<Task> dep(new Task);
                dep->env = env;
                dep->exp = exp->link->link;
                dep->result = &args[0];

                tasks.push(dep);
                return;
            }
            else
            {
                args[0] = exp->link->link;
            }
        }
        else if (exp->link->type == Type::LIST)
        {
            size_t arg_i = 0;
            auto this_arg = exp->link->get_list();
            while (this_arg)
            {
                if (!this_arg->get_list()->link->self_eval())
                {
                    ++eval_needed;
                    args.push_back(nullptr);

                    std::shared_ptr<Task> dep(new Task);
                    dep->env = env;
                    dep->exp = this_arg->get_list()->link;
                    dep->result = &args[arg_i];

                    tasks.push(dep);
                }
                else
                {
                    args[arg_i] = this_arg->get_list()->link;
                }

                this_arg = this_arg->link;
                ++arg_i;
            }

            if (eval_needed > 0)
            {
                return;
            }
        }
    }

    // Build environment.
    auto new_env = env->spawn();
    auto it = exp->link;
    size_t arg = 0;
    if (it->type == Type::SYMBOL)
    {
        // Get single assignment.
        new_env->let(it->get_string(), args[0]);

        // Point to start of bodies.
        it = it->link->link;
    }
    else if (it->type == Type::LIST)
    {
        it = it->get_list();
        while (it)
        {
            new_env->let(it->get_list()->get_string(), args[arg]);
            it = it->link;
            ++arg;
        }
        it = exp->link->link;
    }

    // Get bodies.
    while (it)
    {
        bodies.push(it);
        it = it->link;
    }

    // Build tasks.
    tasks.pop();
    std::shared_ptr<Task> ret(new Task);
    ret->env = new_env;
    ret->exp = bodies.top();
    ret->result = result;
    tasks.push(ret);
    bodies.pop();
    while (!bodies.empty())
    {
        std::shared_ptr<Task> next(new Task);
        next->env = new_env;
        next->exp = bodies.top();

        tasks.push(next);
        bodies.pop();
    }
}

void special_if(std::stack<std::shared_ptr<Task>>& tasks)
{
    auto cur_task = tasks.top();
    auto env = cur_task->env;
    auto exp = cur_task->exp->get_list();
    auto& args = cur_task->args;
    auto& result = cur_task->result;

    // Check argument count.
    if (args.size() != 2 && args.size() != 3)
    {
        std::cerr << "Incorrect arguments for \"if\".\n";
        std::exit(1);
    }

    // Check if the condition needs evaluated.
    if (args[0]->self_eval() == false)
    {
        std::shared_ptr<Task> dep(new Task);
        dep->env = env;
        dep->exp = args[0];
        dep->result = &args[0];
        tasks.push(dep);
    }
    else
    {
        // Evaluate if statement.
        std::shared_ptr<Exp> next_exp;
        if (args[0]->get_bool() == true)
        {
            next_exp = args[1];
        }
        else
        {
            next_exp = args[2];
        }

        // Check if we need to further evaluate the result.
        if (next_exp->self_eval() == true)
        {
            if (result != nullptr)
            {
                *result = next_exp;
                tasks.pop();
            }
            else
            {
                // Do nothing. Self-evaluating expression
                // cannot have side-effects.
                tasks.pop();
            }
        }
        else
        {
            // Build new task.
            std::shared_ptr<Task> next(new Task);
            next->env = env;
            next->exp = next_exp;
            next->result = result;

            // Replace expression on the evaluation stack.
            tasks.pop();
            tasks.push(next);
        }
    }
}
