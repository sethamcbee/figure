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
    init_env->builtin("lambda", builtin_lambda);
    init_env->builtin("if", builtin_if);
    init_env->builtin("let", builtin_let);
    init_env->builtin("display", builtin_display);
    init_env->builtin("+", builtin_add);
    init_env->builtin("-", builtin_sub);
    init_env->builtin("*", builtin_mul);
    init_env->builtin("/", builtin_div);

    // Parse input.
    std::shared_ptr<Exp> root = Exp::spawn();
    root->parse(prog, 0, prog.length());

    // Evaluate program.
    std::shared_ptr<Exp> result = root->eval(*init_env);
#if 0
    std::stack<Task> tasks;
    tasks.push(Task(init_env, root));
    std::shared_ptr<Exp> result = nullptr;
    while (!tasks.empty())
    {
        Task cur_task = tasks.top();
        tasks.pop();
        std::shared_ptr<Env> env = cur_task.first;
        std::shared_ptr<Exp> exp = cur_task.second;

        // Pass atoms as-is.
        if (exp->type == Type::STRING)
        {
            result = exp;
        }
        else if (exp->type == Type::BOOLEAN)
        {
            result = exp;
        }
        else if (exp->type == Type::NUMBER)
        {
            result = exp;
        }
        else if (exp->type == Type::VOID)
        {
            result = exp;
        }
        else if (exp->type == Type::NATIVE_FUNCTION)
        {
            result = exp;
        }
        else if (exp->type == Type::LAMBDA)
        {
            result = exp;
        }
        // Evaluate symbols, function applications, and lists.
        else if (exp->type == Type::SYMBOL)
        {
            std::string* sym = (std::string*)exp->data;
            result = env->get(*sym);
        }
    }
#endif

    return result;
}
