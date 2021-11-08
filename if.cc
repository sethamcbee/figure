/**
 * @file if.cc
 */

#include "if.h"

namespace Figure
{

If::If(Env& env, const DatumList& l)
{
    auto kwd = l.begin();
    const auto& id = std::get<Id>(kwd->value);
    keyword = id;
    
    auto t = ++kwd;
    test = make_exp(env, *t);
    
    auto c = ++t;
    consequent = make_exp(env, *c);
    
    auto a = ++c;
    if (a != l.end())
    {
        alternate = make_exp(env, *a);
    }
}

void If::print(std::ostream& o) const
{
    o << "(";
    o << keyword;
    o << " ";
    test->print(o);
    o << " ";
    consequent->print(o);
    if (alternate)
    {
        std::cout << " ";
        alternate->print(o);
    }
    o << ")";
}

Ref<Exp> make_if(Env& env, const DatumList& l)
{
    If tmp{env, l};
    return make_ref(tmp);
}

}
