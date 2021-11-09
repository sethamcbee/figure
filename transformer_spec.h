/**
 * @file transformer_spec.h
 */

#pragma once

#include <list>

#include "exp.h"
#include "syntax_rule.h"

namespace Figure
{

struct Env;

using IdList = std::list<Id>;

struct TransformerSpec : public Exp
{
    const Datum* source = nullptr;
    Id keyword;
    Id identifier;
    IdList identifiers;
    std::list<SyntaxRule> syntax_rules;

    TransformerSpec();
    TransformerSpec(Env& env, const Datum& datum);

    virtual void print(std::ostream& o) const;

    void error() const;
    void error(const std::string& err) const;
};

}
