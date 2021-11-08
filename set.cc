/**
 * @file set.cc
 */

#include "env.h"
#include "set.h"

namespace Figure
{

Set::Set(Env& env, const DatumList& l)
{
    auto kwd = l.begin();
    keyword = std::get<Id>(kwd->value);

    auto id = std::next(kwd);
    identifier = std::get<Id>(id->value);
    auto val = env.get(identifier);
    if (auto ref = std::get_if<Ref<Exp>>(&val))
    {
        if (!*ref)
        {
            error();
            exit(1);
        }
    }

    auto exp = std::next(id);
    expression = make_exp(env, *exp);
    env.set(identifier, expression);
}

void Set::print() const
{
    std::cout << "(" << keyword << " " << identifier << " ";
    expression->print();
    std::cout << ")";
}

Ref<Exp> make_set(Env& env, const DatumList& l)
{
    Set tmp{env, l};
    return make_ref(tmp);
}

}
