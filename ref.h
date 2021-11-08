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
using WeakRef = std::weak_ptr<T>;

template <class T>
Ref<T> make_ref(const T& t)
{
    return Ref<T> {new T{t}};
}

template <class T>
WeakRef<T> make_weakref(const T& t)
{
    return WeakRef<T> {new T{t}};
}

}
