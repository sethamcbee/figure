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

    // Parse input.
    std::shared_ptr<Exp> root = Exp::spawn();
    root->parse(prog, 0, prog.length());

    // Evaluate program.
    Task result;
    std::stack<Task> tasks;
    Task first_task;
    first_task.parent = &result;
    first_task.env = init_env;
    first_task.exp = root;
    tasks.push(first_task);
    while (!tasks.empty())
    {
        Task& cur_task = tasks.top();
        Task* parent = cur_task.parent;
        std::shared_ptr<Env>& env = cur_task.env;
        std::shared_ptr<Exp>& exp = cur_task.exp;
        std::vector<std::shared_ptr<Exp>>& args = cur_task.args;

        switch (exp->type)
        {
        case Type::STRING:
        case Type::BOOLEAN:
        case Type::NUMBER:
        case Type::VOID:
        case Type::NATIVE_FUNCTION:
        case Type::LAMBDA:
            // Atoms need no evaluation.
            parent->args.push_back(exp);
            tasks.pop();
            break;

        case Type::SYMBOL:
            // Retrieve value bound to this symbol.
            parent->args.push_back(env->get(exp->get_string()));
            tasks.pop();
            break;

        case Type::LIST:
            // If empty list, just return.
            if (exp->get_list() == nullptr)
            {
                parent->args.push_back(exp);
                tasks.pop();
                break;
            }
            // If one element, evaluate it and return.
            else if (!exp->get_list()->link)
            {
                Task dep;
                dep.parent = parent;
                dep.env = env;
                dep.exp = exp->get_list();
                tasks.pop();
                tasks.push(dep);
                break;
            }
            // Check if we should evaluate the first element.
            else if (args.size() == 0 && exp->get_list()->type == Type::SYMBOL)
            {
                Task dep;
                dep.parent = &cur_task;
                dep.env = env;
                dep.exp = exp->get_list();
                tasks.push(dep);
                break;
            }
            // Check if this is a lambda substitution.
            else if (args.size() > 0 && args[0]->type == Type::LAMBDA)
            {
                std::cerr << "Lambda's are not yet supported.\n";
                std::exit(1);
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
                parent->args.push_back(exp);
                tasks.pop();
                break;
            }

        default:
            std::cerr << "Unsupported expression type.\n";
            std::exit(1);
            break;
        }
    }

    return result.args[0];
}
