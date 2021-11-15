/**
 * @file ref.h
 */

#pragma once

#include <iostream>
#include <memory>
#include <queue>
#include <variant>

namespace Figure
{

template <class T>
using StrongRef = std::shared_ptr<T>;

template <class T>
using WeakRef = std::weak_ptr<T>;

template <class T>
using Queue = std::queue<T>;

template <class T>
struct Ref
{
    using Value = std::variant<StrongRef<T>, WeakRef<T>, T*, nullptr_t>;

    Value value;

    Ref()
    {
        value = nullptr;
    }

    Ref(const StrongRef<T>& strong)
    {
        value = strong;
    }

    Ref(const WeakRef<T>& weak)
    {
        value = weak;
    }

    Ref(T* p)
    {
        value = p;
    }

    Ref(const nullptr_t n)
    {
        value = nullptr;
    }

    // Cast to base class.
    template <class Y>
    operator Ref<Y>() const
    {
        if (auto strong = std::get_if<StrongRef<T>>(&value))
        {
            return std::static_pointer_cast<Y>(*strong);
        }
        else if (auto weak = std::get_if<WeakRef<T>>(&value))
        {
            auto shared = weak->lock();
            return std::static_pointer_cast<Y>(shared);
        }
        else if (auto raw = std::get_if<T*>(&value))
        {
            return (Y*)*raw;
        }
        else
        {
            // nullptr_t
            return nullptr;
        }
    }

    operator bool() const
    {
        if (auto strong = std::get_if<StrongRef<T>>(&value))
        {
            return (bool)*strong;
        }
        else if (auto weak = std::get_if<WeakRef<T>>(&value))
        {
            auto shared = weak->lock();
            return (bool)shared;
        }
        else if (auto raw = std::get_if<T*>(&value))
        {
            return (bool)*raw;
        }
        else
        {
            // nullptr_t
            return false;
        }
    }

    const T& operator*() const
    {
        if (auto strong = std::get_if<StrongRef<T>>(&value))
        {
            return **strong;
        }
        else if (auto weak = std::get_if<WeakRef<T>>(&value))
        {
            auto shared = weak->lock();
            return *shared;
        }
        else if (auto raw = std::get_if<T*>(&value))
        {
            return *raw;
        }
        else
        {
            // nullptr_t
            std::cerr << "nullptr access" << std::endl;
            exit(1);
        }
    }

    T& operator*()
    {
        if (auto strong = std::get_if<StrongRef<T>>(&value))
        {
            return **strong;
        }
        else if (auto weak = std::get_if<WeakRef<T>>(&value))
        {
            auto shared = weak->lock();
            return *shared;
        }
        else if (auto raw = std::get_if<T*>(&value))
        {
            return *raw;
        }
        else
        {
            // nullptr_t
            std::cerr << "nullptr access" << std::endl;
            exit(1);
        }
    }

    const T* operator->() const
    {
        if (auto strong = std::get_if<StrongRef<T>>(&value))
        {
            return strong->get();
        }
        else if (auto weak = std::get_if<WeakRef<T>>(&value))
        {
            auto shared = weak->lock();
            return shared.get();
        }
        else if (auto raw = std::get_if<T*>(&value))
        {
            return *raw;
        }
        else
        {
            // nullptr_t
            return nullptr;
        }
    }

    T* operator->()
    {
        if (auto strong = std::get_if<StrongRef<T>>(&value))
        {
            return strong->get();
        }
        else if (auto weak = std::get_if<WeakRef<T>>(&value))
        {
            auto shared = weak->lock();
            return shared.get();
        }
        else if (auto raw = std::get_if<T*>(&value))
        {
            return *raw;
        }
        else
        {
            // nullptr_t
            return nullptr;
        }
    }
};

template <class T, class U>
Ref<T> static_ref_cast(const Ref<U>& r)
{
    if (auto strong = std::get_if<StrongRef<U>>(&r.value))
    {
        return std::static_pointer_cast<T>(*strong);
    }
    else if (auto weak = std::get_if<WeakRef<U>>(&r.value))
    {
        auto shared = weak->lock();
        return std::static_pointer_cast<T>(shared);
    }
    else if (auto raw = std::get_if<U*>(&r.value))
    {
        return (T*)*raw;
    }
    else
    {
        // nullptr_t
        return nullptr;
    }
}

template <class T>
Ref<T> make_ref(const T& t)
{
    return StrongRef<T>{new T{t}};
}

template <class T>
Ref<T> make_weak(const StrongRef<T>& t)
{
    return WeakRef<T>{t};
}

template <class T>
Ref<T> make_unmanaged(T* t)
{
    return t;
}

}
