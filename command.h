/**
 * @file command.h
 */

#pragma once

#include "exp.h"

namespace Figure
{

struct Datum;
struct Env;

struct Command : public Exp
{};

Ref<Exp> make_command(Env& env, const Datum& datum);

};
