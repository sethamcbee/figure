/**
 * @file cons.h
 */

#pragma once

#include "exp.h"
#include "ref.h"

namespace Figure
{

struct Cons : public Exp
{
    Ref<Exp> car;
    Ref<Exp> cdr;
};

Ref<Cons> cons();
Ref<Cons> cons(const Ref<Exp>& a, const Ref<Exp>& d);

Ref<Exp> car(const Ref<Exp>& p);
Ref<Exp> cdr(const Ref<Exp>& p);

extern const Ref<Cons> NIL;

}
