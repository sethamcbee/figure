/**
 * @file builtin.h
 * @brief Interpreter built-in forms.
 */

#ifndef BUILTIN_H
#define BUILTIN_H

#include <vector>

#include "type.h"

class Exp;
class Env;
class Task;

// Mathematical operators.
std::shared_ptr<Exp> eval_add(std::vector<std::shared_ptr<Exp>>& args);
std::shared_ptr<Exp> eval_sub(std::vector<std::shared_ptr<Exp>>& args);
std::shared_ptr<Exp> eval_mul(std::vector<std::shared_ptr<Exp>>& args);
std::shared_ptr<Exp> eval_div(std::vector<std::shared_ptr<Exp>>& args);
std::shared_ptr<Exp> eval_numeq(std::vector<std::shared_ptr<Exp>>& args);

std::shared_ptr<Exp> eval_display(std::vector<std::shared_ptr<Exp>>& args);

#endif // BUILTIN_H
