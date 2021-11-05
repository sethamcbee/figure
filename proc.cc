/**
 * @file proc.cc
 */

#include <iostream>

#include "exp.h"
#include "proc.h"

namespace Figure
{

Proc::Proc(Env& e, const Datum& d)
{
    env = &e;
    const auto& l = std::get<DatumList>(d.value);
    auto first = l.begin();
    op = Ref<Exp>{new Exp{env, *first}};
    for (auto datum = first; datum != l.end(); ++datum)
    {
        auto exp = Exp{env, *datum};
        args.push_back(exp);
    }
}

void Proc::print() const
{
    std::cout << "(";
    const auto& op_exp = std::get<Ref<Exp>>(op->value);
    const auto& lam = std::get<Lambda>(op_exp->value);
    lam.print();
    for (const auto& exp : args)
    {
        std::cout << " ";
        exp.print();
    }
    std::cout << ")";
}

}
