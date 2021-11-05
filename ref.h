/**
 * @file ref.h
 */

#pragma once

#include <memory>

namespace Figure
{

template <class T>
using Ref = std::shared_ptr<T>;

template <class T>
Ref<T> make_ref(T&& t)
{
    return Ref<T>{new T{t}};
}

template <class T>
Ref<T> make_ref(const T&& t)
{
    return Ref<T>{new T{t}};
}

}
