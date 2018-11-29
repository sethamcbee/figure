/**
 * @file figure.cc
 */

#include <iostream>
#include <memory>
#include <stack>
#include <utility>

#include "builtin.h"
#include "env.h"
#include "exp.h"
#include "figure.h"
#include "lambda.h"
#include "special_form.h"
#include "type.h"

std::shared_ptr<Exp> Figure::run(const std::string& prog)
{
    // Build initial environment.
    std::shared_ptr<Env> init_env(new Env);
    std::shared_ptr<Exp> exp_true = Exp::spawn();
    exp_true->type = Type::BOOLEAN;
    exp_true->data = new bool(true);
    init_env->let("true", exp_true);
    std::shared_ptr<Exp> exp_false = Exp::spawn();
    exp_false->type = Type::BOOLEAN;
    exp_false->data = new bool(false);
    init_env->let("false", exp_false);
    init_env->special_form("lambda", special_lambda);
    init_env->special_form("let", special_letrec);
    init_env->special_form("letrec", special_letrec);
    init_env->special_form("if", special_if);
    init_env->builtin("+", eval_add);
    init_env->builtin("-", eval_sub);
    init_env->builtin("*", eval_mul);
    init_env->builtin("/", eval_div);
    init_env->builtin("=", eval_numeq);
    init_env->builtin("display", eval_display);
#if 0
    init_env->builtin("and", eval_and);
    init_env->builtin("or", eval_or);
    init_env->builtin("lambda", eval_lambda);
    init_env->builtin("let", eval_let);
    init_env->builtin("letrec", eval_letrec);
    init_env->builtin("begin", eval_begin);
    init_env->builtin("write", eval_write);
#endif

    // Parse input.
    size_t pos = next_token(prog, 0, prog.length());
    size_t end = skip_token(prog, pos, prog.length());

    // Read first top level definition.
    std::shared_ptr<Exp> root = Exp::spawn();
    root->parse(prog, pos, end);

    // Read additional expressions.
    std::shared_ptr<Exp> next = root;
    while (next_token(prog, end - 1, prog.length()) != std::string::npos)
    {
        // Get a single top-level definition or expression.
        pos = next_token(prog, end - 1, prog.length());
        end = skip_token(prog, pos, prog.length());
        next->link = Exp::spawn();
        next = next->link;
        next->parse(prog, pos, end);
    }

    // Add all top level definitions to the global environment
    // and add all other expressions to the execution list.
    std::vector<std::shared_ptr<Exp>> execution_list;
    next = root;
    while (next)
    {
        // Check if this is a define.
        if (next->type == Type::LIST
            && next->get_list()->type == Type::SYMBOL
            && next->get_list()->get_string() == "define")
        {
            // Check if this is a variable definition.
            auto def = next->get_list();
            if (def->link->type == Type::SYMBOL)
            {
                init_env->let(def->link->get_string(), def->link->link);
            }
            // Check if this is a function shorthand.
            else if (def->link->type == Type::LIST)
            {
                // Get parameter names.

                // Build lambda.
            }
        }
        else
        {
            // Add this expression to the execution list.
            execution_list.push_back(next);
        }

        // Iterate to next top-level expression.
        next = next->link;
    }

    // Evaluate program.
    std::shared_ptr<Exp> ret = nullptr;
    for (auto exp : execution_list)
    {
        ret = eval(init_env, exp);
    }

    return ret;
}

std::shared_ptr<Exp> Figure::eval(
    std::shared_ptr<Env> init_env,
    std::shared_ptr<Exp> root)
{
    std::shared_ptr<Exp> ret;
    std::stack<std::shared_ptr<Task>> tasks;
    std::shared_ptr<Task> first_task(new Task);
    first_task->result = &ret;
    first_task->env = init_env;
    first_task->exp = root;
    tasks.push(first_task);
    while (!tasks.empty())
    {
        auto cur_task = tasks.top();
        auto env = cur_task->env;
        auto exp = cur_task->exp;
        auto& alias = cur_task->alias;
        auto& args = cur_task->args;
        auto& result = cur_task->result;
        size_t& eval_count = cur_task->eval_count;
        ++eval_count;

#if 1
        exp->print();
        std::cout << std::endl;
#endif

        switch (exp->type)
        {
        case Type::STRING:
        case Type::BOOLEAN:
        case Type::NUMBER:
        case Type::VOID:
        case Type::NATIVE_FUNCTION:
        case Type::SPECIAL_FORM:
        case Type::LAMBDA:
            // Atoms need no evaluation.
            if (result)
            {
                *result = exp;
            }
            tasks.pop();
            break;

        case Type::SYMBOL:
            // Retrieve value bound to this symbol.
            if (result)
            {
                *result = env->get(exp->get_string());
            }
            else
            {
                // Retrieve and discard, for side-effects.
                auto discard = env->get(exp->get_string());
                discard = nullptr;
            }
            tasks.pop();
            break;

        case Type::LIST:
            // Check for empty list.
            if (exp->get_list() == nullptr)
            {
                if (result)
                {
                    // Return void.
                    *result = Exp::spawn();
                    tasks.pop();
                }
            }
            else
            {
                auto first = exp->get_list();

                // Check if this is a function application, special
                // form, or data.
                if (first->self_eval() == true || alias != nullptr)
                {
                    // Use alias if it exists.
                    if (alias)
                    {
                        first = alias;
                        first->link = exp->get_list()->link;
                    }

                    // Check if this is a special form.
                    if (first->type == Type::SPECIAL_FORM)
                    {
                        // Pass arguments un-evaluated to handler.
                        if (args.size() == 0)
                        {
                            auto it = first->link;
                            while (it != nullptr)
                            {
                                args.push_back(it);
                                it = it->link;
                            }
                        }
                        auto fn = first->get_special_form();
                        fn(tasks);
                    }
                    // Check if this is a lambda evaluation.
                    else if (first->type == Type::LAMBDA)
                    {
                        auto lam = first->get_lambda();

                        // Evaluate arguments.
                        if (args.size() == 0 && first->link)
                        {
                            auto it = first->link;

                            while (it)
                            {
                                args.push_back(it);

                                if (!it->self_eval())
                                {
                                    std::shared_ptr<Task> dep(new Task);
                                    dep->env = env;
                                    dep->exp = it;
                                    dep->result = &args[args.size() - 1];

                                    tasks.push(dep);
                                }

                                it = it->link;
                            }

                            break;
                        }

                        // Build closure.
                        auto clos = lam.env->spawn();
                        size_t arg_count = lam.args.size();
                        for (size_t i = 0; i < arg_count; ++i)
                        {
                            clos->let(lam.args[i], args[i]);
                        }

                        // Evaluate bodies and return result from
                        // last body.
                        std::stack<std::shared_ptr<Task>> bodies;
                        size_t body_count = lam.bodies.size();
                        for (size_t i = 0; i < body_count - 1; ++i)
                        {
                            std::shared_ptr<Task> body(new Task);
                            body->env = clos;
                            body->exp = lam.bodies[i];
                            bodies.push(body);
                        }
                        std::shared_ptr<Task> ret(new Task);
                        ret->env = clos;
                        ret->exp = lam.bodies[body_count - 1];
                        ret->result = result;
                        tasks.pop();
                        tasks.push(ret);
                        while (!bodies.empty())
                        {
                            tasks.push(bodies.top());
                            bodies.pop();
                        }

                        break;
                    }
                    // Check if this is a builtin function.
                    else if (first->type == Type::NATIVE_FUNCTION)
                    {
                        // Evaluate arguments.
                        if (eval_count == 2 && first->link)
                        {
                            auto it = first->link;
                            while (it)
                            {
                                args.push_back(it);
                                if (!it->self_eval())
                                {
                                    std::shared_ptr<Task> dep(new Task);
                                    dep->env = env;
                                    dep->exp = it;
                                    dep->result = &args[args.size() - 1];

                                    tasks.push(dep);
                                }

                                it = it->link;
                            }

                            continue;
                        }

                        // Pass arguments to built-in function.
                        auto fn = first->get_native_function();
                        auto ret = fn(args);
                        if (result)
                        {
                            *result = ret;
                        }
                        tasks.pop();
                    }
                    // Treat as data.
                    else
                    {
                        if (result != nullptr)
                        {
                            // Return data list as-is.
                            *result = exp;
                        }
                        tasks.pop();
                    }
                }
                else
                {
                    // Evaluate first parameter.
                    if (args.size() == 0)
                    {
                        std::shared_ptr<Task> dep(new Task);
                        dep->env = env;
                        dep->exp = first;
                        dep->result = &alias;

                        tasks.push(dep);
                    }
                }
            }
            break;

        default:
            std::cerr << "Unsupported expression type.\n";
            std::exit(1);
            break;
        }
    }

    return ret;
}
