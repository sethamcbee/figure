/**
 * @file define_var.h
 */

#pragma once

#include "definition.h"

namespace Figure
{

struct DefineVar : public Definition
{
    const Datum* source = nullptr;
    Id keyword;
    Id identifier;
    Ref<Exp> expression;

    DefineVar(Env& env, const Datum& datum);

    virtual void print(std::ostream& o) const;

    void error() const;
    void error(const std::string& err) const;
};

}
