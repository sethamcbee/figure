/**
 * @file lambda.h
 */

#pragma once

#include "datum.h"
#include "env.h"
#include "exp.h"
#include "formals.h"
#include "literal.h"

namespace Figure
{

struct Lambda : public Exp
{
    Id keyword;
    Formals formals;
    Env env;
    Ref<Exp> body;

    Lambda(Env& e, const DatumList& l);

    virtual void print(std::ostream& o) const;
};

Ref<Exp> make_lambda(Env& e, const DatumList& l);

}
