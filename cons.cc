/**
 * @file cons.cc
 */

#include "cons.h"

namespace Figure
{

Ref<Exp> cons()
{
    Cons tmp;
    return make_ref(tmp);
}

Ref<Exp> cons(const Ref<Exp>& a, const Ref<Exp>& d)
{
    Cons tmp;
    tmp.car = a;
    tmp.car = d;
    return make_ref(tmp);
}

Ref<Exp> car(const Ref<Exp>& p)
{
    const auto& c = std::static_pointer_cast<Cons>(p);
    return c->car;
}

Ref<Exp> cdr(const Ref<Exp>& p)
{
    const auto& c = std::static_pointer_cast<Cons>(p);
    return c->cdr;
}

const Ref<Cons> NIL;

}
