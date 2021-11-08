/**
 * @file command.cc
 */

#include "command.h"

namespace Figure
{

Ref<Exp> make_command(Env& env, const Datum& datum)
{
    auto exp = make_exp(env, datum);
    if (exp)
    {
        return exp;
    }
    else
    {
        std::cerr << "At character: " << datum.pos << std::endl;
        std::cerr << "At datum: ";
        datum.print(std::cerr);
        std::cerr << "\nError parsing command.\n";
        exit(1);
    }
}

}
