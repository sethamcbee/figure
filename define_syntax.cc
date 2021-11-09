/**
 * @file define_syntax.cc
 */

#include "define_syntax.h"

namespace Figure
{

DefineSyntax::DefineSyntax(Env& env, const Datum& datum)
{
    source = &datum;
    const auto& l = std::get<DatumList>(datum.value);

    auto kwd = l.begin();
    keyword = std::get<Id>(kwd->value);

    auto id = std::next(kwd);
    identifier = std::get<Id>(id->value);

    auto spec = std::next(id);
    transformer_spec = TransformerSpec{env, *spec};
}

void DefineSyntax::print(std::ostream& o) const
{
    o << "(" << keyword << " " << identifier << " ";
    transformer_spec.print(o);
    o << ")";
}

void DefineSyntax::error() const
{
}

void DefineSyntax::error(const std::string& err) const
{
}

}
