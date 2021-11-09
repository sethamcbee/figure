/**
 * @file syntax_rule.h
 */

#pragma once

#include "exp.h"
#include "pattern.h"
#include "template.h"

namespace Figure
{

struct Env;

struct SyntaxRule : public Exp
{
    const Datum* source = nullptr;
    Ref<Pattern> pattern;
    Ref<Template> pattern_template;

    SyntaxRule(Env& env, const Datum& datum);

    virtual void print(std::ostream& o) const;
};

}
