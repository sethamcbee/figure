/**
 * @file define_syntax.h
 */

#pragma once

#include "definition.h"

namespace Figure
{

struct DefineSyntax : public Definition
{
    const Datum* source = nullptr;
    Id keyword;

    DefineSyntax(Env& env, const Datum& datum);

    virtual void print(std::ostream& o) const;

    void error() const;
    void error(const std::string& err) const;
};

}
