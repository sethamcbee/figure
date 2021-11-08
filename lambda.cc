/**
 * @file lambda.cc
 */

#include "body.h"
#include "lambda.h"

namespace Figure
{

Lambda::Lambda(Env& e, const Datum& datum)
{
    source = &datum;
    env.parent = &e;
    const auto& l = std::get<DatumList>(datum.value);
    if (l.size() < 3)
    {
        error("Invalid argument count.");
        exit(1);
    }

    auto kwd = l.begin();
    const auto& id = std::get<Id>(kwd->value);
    keyword = id;

    auto formals_p = ++kwd;
    formals = Formals{env, *formals_p};

    auto body_begin = ++formals_p;
    auto body_end = l.end();
    body = make_body(env, body_begin, body_end);
}

void Lambda::print(std::ostream& o) const
{
    o << "(" << keyword << " ";
    formals.print(o);
    o << " ";
    body->print(o);
    o << ")";
}

void Lambda::error() const
{
    error("Unspecified error.");
}

void Lambda::error(const std::string& err) const
{
    if (source)
    {
        std::cerr << "At character: " << source->pos << std::endl;
        std::cerr << "At datum: ";
        source->print(std::cerr);
        std::cerr << std::endl;
    }
    std::cerr << "Error parsing lambda expression: ";
    std::cerr << err << std::endl;
}

Ref<Exp> make_lambda(Env& e, const Datum& datum)
{
    Lambda tmp{e, datum};
    return make_ref(tmp);
}

}
