/**
 * @file set.cc
 */

#include <iostream>

#include "datum.h"
#include "exp.h"
#include "set.h"

namespace Figure
{

Set::Set(Env& e, const Datum& d)
{
    env = &e;
    const auto& l = std::get<DatumList>(d.value);
    if (l.size() != 3)
    {
        std::cerr << "\nInvalid set! expression arguments.\n";
        exit(1);
    }
    auto first = ++l.begin();
    id = std::get<Id>(first->value);
    auto second = ++first;
    const auto& datum = *second;
    auto exp = new Exp{env, datum};
    env->map[id] = exp;
}

void Set::print() const
{
    std::cout << "(set! " << id.value << " ";
    auto exp = std::get<Exp*>(env->get(id));
    exp->print();
    std::cout << ")";
}

}
