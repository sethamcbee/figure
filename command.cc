/**
 * @file command.cc
 */

#include "command.h"

namespace Figure
{

Ref<Exp> make_command(Env& env, const Datum& datum)
{
    return make_exp(env, datum);
}

}
