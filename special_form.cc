/**
 * @file special_form.cc
 */

#include <iostream>
#include <memory>
#include <vector>

#include "env.h"
#include "exp.h"
#include "special_form.h"
#include "type.h"

void special_if(std::stack<std::shared_ptr<Task>>& tasks)
{
    auto cur_task = tasks.top();
    auto parent = cur_task->parent;
    auto env = cur_task->env;
    auto exp = cur_task->exp->get_list();
    auto& args = cur_task->args;
    auto result = cur_task->result;

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
        dep->parent = cur_task->parent;
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
