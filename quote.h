/**
 * @file quote.h
 */

#pragma once

#include "datum.h"
#include "exp.h"

namespace Figure
{

struct Quote : public Exp
{
    Datum quoted;

    Quote(const Datum& datum);

    virtual void print(std::ostream& o) const;

    virtual void error() const;
    virtual void error(const std::string& err) const;
};

Ref<Quote> make_quote(const Datum& datum);
const Datum& get_quoted(const Ref<Exp>& ref);

}
