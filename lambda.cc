/**
 * @file lambda.cc
 */

#include <iostream>

#include "datum.h"
#include "exp.h"
#include "lambda.h"

namespace Figure
{

Lambda::Lambda(Env& e, const Datum& d)
{
    const auto& l = std::get<DatumList>(d.value);
    auto var = ++l.begin();
    formals = Formals{e, *var};
    for (auto body = ++var; body != l.end(); ++body)
    {
        auto exp = Exp{&e, *body};
        bodies.push_back(exp);
    }
}

void Lambda::print() const
{
    std::cout << "(lambda ";
    formals.print();
    for (const auto& body : bodies)
    {
        std::cout << " ";
        body.print();
    }
    std::cout << ")";
}

}
