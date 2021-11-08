/**
 * @file definition.cc
 */

#include "definition.h"
#include "env.h"

namespace Figure
{

DefineVar::DefineVar(Env& env, const DatumList& datum)
{
    auto kwd = datum.begin();
    keyword = std::get<Id>(kwd->value);

    auto id = std::next(kwd);
    identifier = std::get<Id>(id->value);

    auto exp = std::next(id);
    expression = make_exp(env, *exp);
    env.set(identifier, expression);
}

void DefineVar::print() const
{
    std::cout << "(" << keyword << " " << identifier << " ";
    expression->print();
    std::cout << ")";
}

Ref<Exp> make_definition(Env& env, const DatumList& datum)
{
    auto kwd = datum.begin();
    auto keyword = std::get<Id>(kwd->value);
    if (keyword.value == "define")
    {
        auto second = std::next(kwd);
        if (std::get_if<Id>(&second->value))
        {
            DefineVar tmp{env, datum};
            return make_ref(tmp);
        }
        else
        {
            return nullptr;
        }
    }
    else
    {
        return nullptr;
    }
}

bool is_definition(const Datum& datum)
{
    if (auto l = std::get_if<DatumList>(&datum.value))
    {
        const auto& first = l->front();
        const auto& id = std::get<Id>(first.value);
        if (id.value == "define")
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

}
