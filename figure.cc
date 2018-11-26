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
#include "type.h"

std::shared_ptr<Exp> Figure::eval(const std::string& prog)
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
    init_env->builtin("if", eval_if);
    init_env->builtin("and", eval_and);
    init_env->builtin("lambda", eval_lambda);
    init_env->builtin("let", eval_let);
    init_env->builtin("letrec", eval_letrec);
    init_env->builtin("begin", eval_begin);
    init_env->builtin("+", eval_add);
    init_env->builtin("-", eval_sub);
    init_env->builtin("*", eval_mul);
    init_env->builtin("/", eval_div);
    init_env->builtin("=", eval_numeq);
    init_env->builtin("display", eval_display);
    init_env->builtin("write", eval_write);

    // Parse input.
    std::shared_ptr<Exp> root = Exp::spawn();
    root->parse(prog, 0, prog.length());

    // Evaluate program.
    std::shared_ptr<Task> result(new Task);
    result->parent = result;
    std::stack<std::shared_ptr<Task>> tasks;
    std::shared_ptr<Task> first_task(new Task);
    first_task->parent = result;
    first_task->env = init_env;
    first_task->exp = root;
    tasks.push(first_task);
    while (!tasks.empty())
    {
        std::shared_ptr<Task> cur_task = tasks.top();
        std::shared_ptr<Task> parent = cur_task->parent;
        std::shared_ptr<Env> env = cur_task->env;
        std::shared_ptr<Exp> exp = cur_task->exp;
        std::vector<std::shared_ptr<Exp>>& args = cur_task->args;

#if 0
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
        case Type::LAMBDA:
            // Atoms need no evaluation.
            if (parent)
            {
                parent->args.push_back(exp);
            }
            tasks.pop();
            break;

        case Type::SYMBOL:
            // Retrieve value bound to this symbol.
            if (parent)
            {
                parent->args.push_back(env->get(exp->get_string()));
            }
            tasks.pop();
            break;

        case Type::LIST:
            // If empty list, just return.
            if (exp->get_list() == nullptr)
            {
                if (parent)
                {
                    parent->args.push_back(exp);
                }
                tasks.pop();
                break;
            }
            // If one element, evaluate it and return.
            else if (!exp->get_list()->link)
            {
                std::shared_ptr<Task> dep(new Task);
                dep->parent = parent;
                dep->env = env;
                dep->exp = exp->get_list();
                tasks.pop();
                tasks.push(dep);
                break;
            }
            // Check if we should evaluate the first element.
            else if (args.size() == 0
                     && ((exp->get_list()->type == Type::SYMBOL)
                         || (exp->get_list()->type == Type::LIST)))
            {
                std::shared_ptr<Task> dep(new Task);
                dep->parent = cur_task;
                dep->env = env;
                dep->exp = exp->get_list();
                tasks.push(dep);
                break;
            }
            // Check if this is a lambda substitution.
            else if (args.size() > 0 && args[0]->type == Type::LAMBDA)
            {
                // Check if arguments need evaluated.
                if (args.size() == 1)
                {
                    // Build list of arguments.
                    std::stack<std::shared_ptr<Task>> arg_stack;
                    for (auto it = exp->get_list()->link; it; it = it->link)
                    {
                        std::shared_ptr<Task> dep(new Task);
                        dep->parent = cur_task;
                        dep->env = env;
                        dep->exp = it;
                        arg_stack.push(dep);
                    }

                    // Pass to be evaluated in reverse order.
                    while (!arg_stack.empty())
                    {
                        tasks.push(arg_stack.top());
                        arg_stack.pop();
                    }

                    break;
                }

                // Substitute evaluated arguments and build closure.
                auto& lam = args[0]->get_lambda();
                auto new_env = lam.env->spawn();
                size_t arg_count = lam.args.size();
                for (size_t i = 0; i < arg_count; ++i)
                {
                    new_env->let(lam.args[i], args[i+1]);
                }

                // Evaluate bodies and return result from last body.
                std::stack<std::shared_ptr<Task>> bodies;
                size_t body_count = lam.bodies.size();
                for (size_t i = 0; i < body_count - 1; ++i)
                {
                    std::shared_ptr<Task> body(new Task);
                    body->parent = nullptr;
                    body->env = new_env;
                    body->exp = lam.bodies[i];
                    bodies.push(body);
                }
                std::shared_ptr<Task> ret(new Task);
                ret->parent = parent;
                ret->env = new_env;
                ret->exp = lam.bodies[body_count - 1];
                tasks.pop();
                tasks.push(ret);
                while (!bodies.empty())
                {
                    tasks.push(bodies.top());
                    bodies.pop();
                }

                break;
            }
            // Check if this is a native function call.
            else if (args.size() > 0 && args[0]->type == Type::NATIVE_FUNCTION)
            {
                Native_Function& fn = args[0]->get_native_function();
                fn(tasks);
                break;
            }
            // Else, return list as-is.
            else
            {
                if (parent)
                {
                    parent->args.push_back(exp);
                }
                tasks.pop();
                break;
            }

        default:
            std::cerr << "Unsupported expression type.\n";
            std::exit(1);
            break;
        }
    }

    // Get return value before cleanup.
    std::shared_ptr<Exp> ret = result->args[0];

    return ret;
}
