/**
 * @file env.h
 */

#pragma once

#include <map>
#include <variant>

#include "literal.h"
#include "ref.h"

namespace Figure
{

struct Exp;

struct KeywordQuote {};
struct KeywordLambda {};
struct KeywordIf {};
struct KeywordSet {};
struct KeywordDefine {};
struct Arg {};

struct Env
{
    using Key = Id;
    using Value = std::variant<
                  KeywordQuote, KeywordLambda, KeywordIf, KeywordSet,
                  KeywordDefine, Arg, Ref<Exp>>;
    using Map = std::map<Key, Value>;

    Map map;
    const Env* parent = nullptr;

    Value get(const Key& k) const;
    void set(const Key& k, const Value& v);
};

Env init_env();

}
