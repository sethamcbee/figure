/**
 * @file definition.cc
 */

#include "define_syntax.h"
#include "define_var.h"
#include "definition.h"
#include "env.h"

namespace Figure
{

Ref<Definition> make_definition(Env& env, const Datum& datum)
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
    else if (keyword.value == "define-syntax")
    {
        DefineSyntax tmp{env, datum};
        return make_ref(tmp);
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
        else if (id.value == "define-syntax")
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
