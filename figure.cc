/**
 * @file figure.cc
 */

#include <iostream>
#include <memory>

#include "builtin.h"
#include "env.h"
#include "exp.h"
#include "figure.h"
#include "type.h"

std::shared_ptr<Exp> Figure::eval(const std::string& prog)
{
    // Build initial environment.
    Env init_env;
    Exp exp_true;
    exp_true.type = Type::BOOLEAN;
    exp_true.data = new bool(true);
    init_env.let("true", &exp_true);
    Exp exp_false;
    exp_false.type = Type::BOOLEAN;
    exp_false.data = new bool(false);
    init_env.let("false", &exp_false);
    init_env.builtin("lambda", builtin_lambda);
    init_env.builtin("if", builtin_if);
    init_env.builtin("let", builtin_let);
    init_env.builtin("print", builtin_print);
    init_env.builtin("+", builtin_add);
    init_env.builtin("-", builtin_sub);
    init_env.builtin("*", builtin_mul);
    init_env.builtin("/", builtin_div);

    // Parse input.
    Exp exp(prog, 0, prog.length());

    // Evaluate program.
    Exp* result = exp.eval(init_env);
    return std::shared_ptr<Exp>(result);
}
