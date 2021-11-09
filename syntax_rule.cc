/**
 * @file syntax_rule.cc
 */

#include "syntax_rule.h"

namespace Figure
{

SyntaxRule::SyntaxRule(Env& env, const Datum& datum)
{
    source = &datum;
    const auto& l = std::get<DatumList>(datum.value);
    if (l.size() != 2)
    {
        std::cerr << "Invalid syntax rule.\n";
        exit(1);
    }

    const auto& pat = l.front();
    pattern = make_pattern(env, pat);

    const auto& tmpl = l.back();
}

void SyntaxRule::print(std::ostream& o) const
{
    o << "(";
    pattern->print(o);
    o << " ";
    //template->print(o);
    o << ")";
}

}
