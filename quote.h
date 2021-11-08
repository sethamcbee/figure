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

    Quote(const DatumList& l);

    virtual void print(std::ostream& o) const;
};

Ref<Exp> make_quote(const DatumList& l);
const Datum& get_quoted(const Ref<Exp>& ref);

}
