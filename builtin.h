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
class Task;

void eval_if(std::stack<std::shared_ptr<Task>>& tasks);
void eval_and(std::stack<std::shared_ptr<Task>>& tasks);
void eval_lambda(std::stack<std::shared_ptr<Task>>& tasks);
void eval_let(std::stack<std::shared_ptr<Task>>& tasks);
void eval_letrec(std::stack<std::shared_ptr<Task>>& tasks);
void eval_begin(std::stack<std::shared_ptr<Task>>& tasks);
void eval_add(std::stack<std::shared_ptr<Task>>& tasks);
void eval_sub(std::stack<std::shared_ptr<Task>>& tasks);
void eval_mul(std::stack<std::shared_ptr<Task>>& tasks);
void eval_div(std::stack<std::shared_ptr<Task>>& tasks);
void eval_numeq(std::stack<std::shared_ptr<Task>>& tasks);
void eval_display(std::stack<std::shared_ptr<Task>>& tasks);
void eval_write(std::stack<std::shared_ptr<Task>>& tasks);

#endif // BUILTIN_H
