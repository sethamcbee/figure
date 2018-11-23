/**
 * @file builtin.h
 * @brief Interpreter built-in forms.
 */

#ifndef BUILTIN_H
#define BUILTIN_H

#include <stack>
#include <vector>

#include "type.h"

class Exp;
class Env;

std::shared_ptr<Exp> builtin_lambda(Env& env, std::vector<std::shared_ptr<Exp>>& args);
std::shared_ptr<Exp> builtin_let(Env& env, std::vector<std::shared_ptr<Exp>>& args);
std::shared_ptr<Exp> builtin_if(Env& env, std::vector<std::shared_ptr<Exp>>& args);
std::shared_ptr<Exp> builtin_print(Env& env, std::vector<std::shared_ptr<Exp>>& args);
std::shared_ptr<Exp> builtin_add(Env& env, std::vector<std::shared_ptr<Exp>>& args);
std::shared_ptr<Exp> builtin_sub(Env& env, std::vector<std::shared_ptr<Exp>>& args);
std::shared_ptr<Exp> builtin_mul(Env& env, std::vector<std::shared_ptr<Exp>>& args);
std::shared_ptr<Exp> builtin_div(Env& env, std::vector<std::shared_ptr<Exp>>& args);

#endif // BUILTIN_H
