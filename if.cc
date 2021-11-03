/**
 * @file if.cc
 */

#include <iostream>

#include "env.h"
#include "exp.h"
#include "if.h"

namespace Figure
{

If::If(Env& e, const Datum& d)
{
    const auto& l = std::get<DatumList>(d.value);
    // No consequent.
    if (l.size() == 3)
    {
        auto test = ++l.begin();
        auto test_exp = Exp{&e, *test};
        value.push_back(test_exp);
        auto consequent = ++test;
        auto consequent_exp = Exp{&e, *consequent};
        value.push_back(consequent_exp);
    }
    // With consequent.
    else if (l.size() == 4)
    {
        auto test = ++l.begin();
        auto test_exp = Exp{&e, *test};
        value.push_back(test_exp);
        auto consequent = ++test;
        auto consequent_exp = Exp{&e, *consequent};
        value.push_back(consequent_exp);
        auto alternate = ++consequent;
        auto alternate_exp = Exp{&e, *alternate};
        value.push_back(alternate_exp);
    }
    else
    {
        std::cerr << "\nInvalid arguments to if form.\n";
        exit(1);
    }
}

void If::print() const
{
    std::cout << "(if ";
    const char* space = "";
    for (const auto& v : value)
    {
        if (const auto exp = std::get_if<Exp>(&v))
        {
            std::cout << space;
            exp->print();
            space = " ";
        }
    }
    std::cout << ")";
}

}
