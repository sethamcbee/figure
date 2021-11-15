/**
 * @file any.h
 */

#pragma once

#include <iostream>
#include <memory>

namespace Figure
{

struct Any
{
    using Type = int;

    static Type make_type()
    {
        static Type type{0};
        return type++;
    }

    template <class T>
    static Type get_type()
    {
        static Type type{make_type()};
        return type;
    }

    struct ValueBase
    {
        Type type;

        ValueBase(int t) : type(t) {}

        virtual ~ValueBase() {}

        virtual ValueBase* make_copy() const
        {
            return nullptr;
        }
    };

    template <class T>
    struct Value : public ValueBase
    {
        T value;

        Value(const T& t) : ValueBase(get_type<T>()), value(t) {}

        virtual ~Value() final {}

        virtual ValueBase* make_copy() const final
        {
            return new Value<T>{value};
        }
    };

    std::unique_ptr<ValueBase> value;

    Any() {}

    Any(const Any& other)
    {
        if (other.value)
        {
            value.reset(other.value->make_copy());
        }
    }

    Any(Any&& other) : value(std::move(other.value)) {}

    template <class T>
    Any(const T& t) : value(new Value{t}) {}

    template <class T>
    Any& operator=(const T& t)
    {
        value.reset(new Value<T>{t});
        return *this;
    }

    Any& operator=(const Any& other)
    {
        if (other.value)
        {
            value.reset(other.value->make_copy());
        }
        else
        {
            value.release();
        }
        return *this;
    }

    Any& operator=(Any&& other)
    {
        if (other.value)
        {
            value = std::move(other.value);
        }
        else
        {
            value.release();
        }
        return *this;
    }
};

template <class T>
const T& any_cast(const Any& v)
{
    if (v.value->type == Any::get_type<T>())
    {
        auto p = v.value.get();
        auto t = (Any::Value<T>*)p;
        return t->value;
    }
    else
    {
        std::cerr << "Invalid Any cast.\n";
        exit(1);
    }
}

template <class T>
T& any_cast(Any& v)
{
    if (v.value->type == Any::get_type<T>())
    {
        auto p = v.value.get();
        auto t = (Any::Value<T>*)p;
        return t->value;
    }
    else
    {
        std::cerr << "Invalid Any cast.\n";
        exit(1);
    }
}

template <class T>
const T* any_cast(const Any* v)
{
    if (v->value->type == Any::get_type<T>())
    {
        auto p = v->value.get();
        auto t = (Any::Value<T>*)p;
        return &t->value;
    }
    else
    {
        return nullptr;
    }
}

template <class T>
T* any_cast(Any* v)
{
    if (v->value->type == Any::get_type<T>())
    {
        auto p = v->value.get();
        auto t = (Any::Value<T>*)p;
        return &t->value;
    }
    else
    {
        return nullptr;
    }
}

template <class T>
const T& fast_cast(const Any& v)
{
    auto p = v.value.get();
    auto t = (Any::Value<T>*)p;
    return t->value;
}

template <class T>
T& fast_cast(Any& v)
{
    auto p = v.value.get();
    auto t = (Any::Value<T>*)p;
    return t->value;
}

template <class T>
const T* fast_cast(const Any* v)
{
    auto p = v->value.get();
    auto t = (Any::Value<T>*)p;
    return &t->value;
}

template <class T>
T* fast_cast(Any* v)
{
    auto p = v->value.get();
    auto t = (Any::Value<T>*)p;
    return &t->value;
}

}
