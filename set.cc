/**
 * @file set.cc
 */

#include "env.h"
#include "set.h"

namespace Figure
{

Set::Set(Env& env, const Datum& datum)
{
    source = &datum;
    const auto& l = std::get<DatumList>(datum.value);

    auto kwd = l.begin();
    keyword = std::get<Id>(kwd->value);

    auto id = std::next(kwd);
    identifier = std::get<Id>(id->value);
    auto val = env.get(identifier);
    if (auto ref = std::get_if<Ref<Exp>>(&val))
    {
        if (!*ref)
        {
            error("Variable not previously defined.");
            exit(1);
        }
    }

    auto exp = std::next(id);
    expression = make_exp(env, *exp);
    //env.set(identifier, expression);
}

void Set::print(std::ostream& o) const
{
    o << "(" << keyword << " " << identifier << " ";
    expression->print(o);
    o << ")";
}

void Set::error() const
{
    error("Unspecified error.");
}

void Set::error(const std::string& err) const
{
    if (source)
    {
        std::cerr << "At character: " << source->pos << std::endl;
        std::cerr << "At datum: ";
        source->print(std::cerr);
        std::cerr << std::endl;
    }
    std::cerr << "Error parsing set! expression: ";
    std::cerr << err << std::endl;
}

Ref<Exp> make_set(Env& env, const Datum& datum)
{
    Set tmp{env, datum};
    return make_ref(tmp);
}

}
