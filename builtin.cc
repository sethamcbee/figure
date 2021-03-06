/**
 * @file builtin.cc
 * @brief Implementation of built-in functions (not special forms).
 */

#include <cstdlib>
#include <iostream>

#include "builtin.h"
#include "exp.h"
#include "type.h"

/// Replace all instances of substr in src.
static void replace_all(
    std::string& str,
    const std::string& from,
    const std::string& to)
{
    size_t len = from.length();
    size_t pos = str.find(from);
    while (pos != std::string::npos)
    {
        str.replace(pos, len, to);
        pos = str.find(from);
    }
}

std::shared_ptr<Exp> eval_add(std::vector<std::shared_ptr<Exp>>& args)
{
    Number_Type sum = 0;
    for (auto& n : args)
    {
        sum += n->get_number();
    }

    auto p = new Number_Type;
    *p = sum;

    auto ret = Exp::spawn();
    ret->type = Type::NUMBER;
    ret->data = p;
    return ret;
}

std::shared_ptr<Exp> eval_sub(std::vector<std::shared_ptr<Exp>>& args)
{
    Number_Type difference = args[0]->get_number();
    size_t arg_count = args.size();
    for (size_t i = 1; i < arg_count; ++i)
    {
        difference -= args[i]->get_number();
    }

    auto p = new Number_Type;
    *p = difference;

    auto ret = Exp::spawn();
    ret->type = Type::NUMBER;
    ret->data = p;
    return ret;
}

std::shared_ptr<Exp> eval_mul(std::vector<std::shared_ptr<Exp>>& args)
{
    Number_Type product = 1;
    for (auto& n : args)
    {
        product *= n->get_number();
    }

    auto p = new Number_Type;
    *p = product;

    auto ret = Exp::spawn();
    ret->type = Type::NUMBER;
    ret->data = p;
    return ret;
}

std::shared_ptr<Exp> eval_div(std::vector<std::shared_ptr<Exp>>& args)
{

    Number_Type quotient = args[0]->get_number();
    size_t arg_count = args.size();
    for (size_t i = 1; i < arg_count; ++i)
    {
        quotient /= args[i]->get_number();
    }

    auto p = new Number_Type;
    *p = quotient;

    auto ret = Exp::spawn();
    ret->type = Type::NUMBER;
    ret->data = p;
    return ret;
}

std::shared_ptr<Exp> eval_numeq(std::vector<std::shared_ptr<Exp>>& args)
{
    // Check arg count.
    if (args.size() != 2)
    {
        std::cerr << "Invalid parameter count in \"=\" call.\n";
        std::exit(1);
    }

    auto p = new bool;
    *p = (args[0]->get_number() == args[1]->get_number());

    auto exp = Exp::spawn();
    exp->type = Type::BOOLEAN;
    exp->data = p;

    return exp;
}

std::shared_ptr<Exp> eval_display(std::vector<std::shared_ptr<Exp>>& args)
{
    // Check arg count.
    if (args.size() != 1)
    {
        std::cerr << "Invalid parameter count in \"display\" call.\n";
        std::exit(1);
    }

    if (args[0]->type == Type::STRING)
    {
        // Escape newlines.
        const std::string& base_str = args[0]->get_string();
        if (base_str.find("\\n") != std::string::npos)
        {
            std::string escaped = base_str;
            replace_all(escaped, "\\n", "\n");
            std::cout << escaped;
        }
        else
        {
            std::cout << base_str;
        }
    }
    else
    {
        args[0]->print();
    }

    // Return void.
    return Exp::spawn();
}
