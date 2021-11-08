/**
 * @file lambda.cc
 */

#include "body.h"
#include "lambda.h"

namespace Figure
{

Lambda::Lambda(Env& e, const DatumList& l)
{
    env.parent = &e;

    auto kwd = l.begin();
    const auto& id = std::get<Id>(kwd->value);
    keyword = id;

    auto formals_p = ++kwd;
    formals = Formals{env, *formals_p};

    auto body_begin = ++formals_p;
    auto body_end = l.end();
    body = make_body(env, body_begin, body_end);
}

void Lambda::print() const
{
    std::cout << "(" << keyword << " ";
    formals.print();
    std::cout << " ";
    body->print();
    std::cout << ")";
}

Ref<Exp> make_lambda(Env& e, const DatumList& l)
{
    Lambda tmp{e, l};
    return make_ref(tmp);
}

}
