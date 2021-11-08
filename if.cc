/**
 * @file if.cc
 */

#include "if.h"

namespace Figure
{

If::If(Env& env, const Datum& datum)
{
    source = &datum;
    const auto& l = std::get<DatumList>(datum.value);
    auto sz = l.size();
    if (sz != 3 && sz != 4)
    {
        error("Invalid argument count.");
        exit(1);
    }

    auto kwd = l.begin();
    const auto& id = std::get<Id>(kwd->value);
    keyword = id;

    auto t = ++kwd;
    test = make_exp(env, *t);
    if (!test)
    {
        error("Error parsing conditional expression.");
        exit(1);
    }

    auto c = ++t;
    consequent = make_exp(env, *c);
    if (!consequent)
    {
        error("Error parsing consequent expression.");
        exit(1);
    }

    auto a = ++c;
    if (a != l.end())
    {
        alternate = make_exp(env, *a);
        if (!alternate)
        {
            error("Error parsing alternate expression.");
            exit(1);
        }
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

void If::error() const
{
    error("Unspecified error.");
}

void If::error(const std::string& err) const
{
    if (source)
    {
        std::cerr << "At character: " << source->pos << std::endl;
        std::cerr << "At datum: ";
        source->print(std::cerr);
        std::cerr << std::endl;
    }
    std::cerr << "Error parsing if expression: ";
    std::cerr << err << std::endl;
}

Ref<Exp> make_if(Env& env, const Datum& datum)
{
    If tmp{env, datum};
    return make_ref(tmp);
}

}
