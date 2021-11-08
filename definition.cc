/**
 * @file definition.cc
 */

#include "definition.h"
#include "env.h"

namespace Figure
{

DefineVar::DefineVar(Env& env, const Datum& datum)
{
    source = &datum;
    const auto& l = std::get<DatumList>(datum.value);
    
    auto kwd = l.begin();
    keyword = std::get<Id>(kwd->value);

    auto id = std::next(kwd);
    identifier = std::get<Id>(id->value);

    auto exp = std::next(id);
    expression = make_exp(env, *exp);
    if (expression)
    {
        env.bind(identifier, expression);
    }
    else
    {
        error("Error parsing sub-expression.");
        exit(1);
    }
}

void DefineVar::print(std::ostream& o) const
{
    o << "(" << keyword << " " << identifier << " ";
    expression->print(o);
    o << ")";
}

void DefineVar::error() const
{
    error("Unspecified error.");
}

void DefineVar::error(const std::string& err) const
{
    if (source)
    {
        std::cerr << "At character: " << source->pos << std::endl;
        std::cerr << "At datum: ";
        source->print(std::cerr);
        std::cerr << std::endl;
    }
    std::cerr << "Error parsing variable definition: ";
    std::cerr << err << std::endl;
}

Ref<Exp> make_definition(Env& env, const Datum& datum)
{
    const auto& l = std::get<DatumList>(datum.value);
    auto kwd = l.begin();
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
