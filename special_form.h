/**
 * @file special_form.h
 * @brief Special forms.
 */

#ifndef SPECIAL_FORM_H
#define SPECIAL_FORM_H

#include <memory>
#include <stack>

class Task;

void special_lambda(std::stack<std::shared_ptr<Task>>& tasks);
void special_let(std::stack<std::shared_ptr<Task>>& tasks);
void special_if(std::stack<std::shared_ptr<Task>>& tasks);

#endif // SPECIAL_H
