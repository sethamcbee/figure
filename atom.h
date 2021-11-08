/**
 * @file atom.h
 */

#pragma once

#include <iostream>

#include "datum.h"
#include "exp.h"
#include "literal.h"
#include "ref.h"

namespace Figure
{

class Env;
class Datum;

template <class T>
struct Atom : public Exp
{
    T value;

    Atom(const T& v);

    virtual void print(std::ostream& o) const;
};

template <class T>
Atom<T>::Atom(const T& v)
{
    value = v;
}

template <class T>
void Atom<T>::print(std::ostream& o) const
{
    o << value;
}

template <class T>
auto make_atom(const Datum& datum)
{
    if (const auto t = std::get_if<T>(&datum.value))
    {
        Atom<T> tmp{T{*t}};
        return make_ref(tmp);
    }
    else
    {
        Exp err;
        err.error();
        exit(1);
    }
}

template <class T>
auto make_atom(const T& t)
{
    Atom<T> tmp{T{t}};
    return make_ref(tmp);
}

template <class T>
const auto& get_atom(const Ref<Exp>& ref)
{
    auto p = std::static_pointer_cast<Atom<T>>(ref);
    return p->value;
}

}
