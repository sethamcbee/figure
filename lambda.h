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

struct Body;

struct Lambda : public Exp
{
    const Datum* source = nullptr;
    Id keyword;
    Formals formals;
    Env env;
    Ref<Body> body;

    Lambda(Env& e, const Datum& datum);

    virtual void print(std::ostream& o) const;

    void error() const;
    void error(const std::string& err) const;
};

Ref<Lambda> make_lambda(Env& e, const Datum& datum);

}
