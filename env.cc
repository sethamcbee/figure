/**
 * @file env.cc
 */

#include <cstdlib>
#include <iostream>

#include "env.h"
#include "exp.h"
#include "type.h"

bool Env::is_bound(const std::string& name)
{
    auto it = symbols.find(name);
    if (it != symbols.end())
    {
        return true;
    }
    else
    {
        if (parent)
        {
            return parent->is_bound(name);
        }
        else
        {
            return false;
        }
    }
}

std::shared_ptr<Exp> Env::get(const std::string& name)
{
    auto it = symbols.find(name);
    if (it != symbols.end())
    {
        return symbols[name];
    }
    else if (parent != nullptr)
    {
        return parent->get(name);
    }
    else
    {
        std::cerr << "Reference to unbound symbol: " << name << "\n";
        std::exit(1);
    }
}

void Env::let(const std::string& name, std::shared_ptr<Exp> val)
{
    symbols[name] = val;
}

void Env::builtin(const std::string& name, const Native_Function& fn)
{
    Native_Function* p = new Native_Function(fn);
    std::shared_ptr<Exp> exp = Exp::spawn();
    exp->type = Type::NATIVE_FUNCTION;
    exp->data = (void*)p;
    let(name, exp);
}

void Env::special_form(const std::string& name, const Special_Form& fn)
{
    Special_Form* p = new Special_Form(fn);
    std::shared_ptr<Exp> exp = Exp::spawn();
    exp->type = Type::SPECIAL_FORM;
    exp->data = (void*)p;
    let(name, exp);
}

std::shared_ptr<Env> Env::spawn()
{
    std::shared_ptr<Env> new_env(new Env);
    new_env->parent = shared_from_this();
    return new_env;
}
