/**
 * @file transformer_spec.cc
 */

#include "transformer_spec.h"

namespace Figure
{

TransformerSpec::TransformerSpec()
{}

TransformerSpec::TransformerSpec(Env& env, const Datum& datum)
{
    source = &datum;
    const auto& l = std::get<DatumList>(datum.value);
    auto sz = l.size();
    if (sz != 3 && sz != 4)
    {
        error("Invalid syntax-rules argument count.");
        exit(1);
    }

    auto kwd = l.begin();
    keyword = std::get<Id>(kwd->value);

    // Check for single identifier.
    auto id_arg = std::next(kwd);
    if (auto id = std::get_if<Id>(&id_arg->value))
    {
        identifier = *id;
        ++id_arg;
    }

    // Get rest of identifiers.
    if (auto id_l = std::get_if<DatumList>(&id_arg->value))
    {
        for (const auto& d : *id_l)
        {
            const auto& id = std::get<Id>(d.value);
            identifiers.push_back(id);
        }
    }
    else
    {
        error("Invalid identifier list.");
        exit(1);
    }

    // Build syntax rules.
    auto rule_it = std::next(id_arg);
    auto end = l.end();
    while (rule_it != end)
    {
        auto rule = SyntaxRule{env, *rule_it};
        syntax_rules.push_back(rule);
        ++rule_it;
    }
}

void TransformerSpec::print(std::ostream& o) const
{
    o << "(" << keyword << " ";
    if (identifier.value != "")
    {
        o << identifier << " ";
    }
    o << "(";
    const char* space = "";
    for (const auto& id : identifiers)
    {
        o << space << id;
        space = " ";
    }
    o << ")";

    for (const auto& rule : syntax_rules)
    {
        o << space;
        rule.print(o);
    }
    o << ")";
}

void TransformerSpec::error() const
{
    error("Unspecified error.");
}

void TransformerSpec::error(const std::string& err) const
{
    if (source)
    {
        std::cerr << "At character: " << source->pos << std::endl;
        std::cerr << "At datum: ";
        source->print(std::cerr);
        std::cerr << std::endl;
    }
    std::cerr << "Error parsing syntax-rules: ";
    std::cerr << err << std::endl;
}

}
