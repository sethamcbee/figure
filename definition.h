/**
 * @file definition.h
 */

#pragma once

#include "datum.h"
#include "exp.h"
#include "literal.h"

namespace Figure
{

struct Env;

struct Definition : public Exp
{};

Ref<Definition> make_definition(Env& env, const Datum& datum);

bool is_definition(const Datum& datum);

}
