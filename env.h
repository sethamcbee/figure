/**
 * @file env.h
 */

#pragma once

#include <map>
#include <variant>

#include "literal.h"

namespace Figure
{

struct Exp;

struct KeywordQuote {};
struct KeywordLambda {};
struct KeywordIf {};
struct KeywordSet {};
struct Arg {};

struct Env
{
    using Key = Id;
    using Value = std::variant<
        KeywordQuote, KeywordLambda, KeywordIf, KeywordSet, Arg,
        Exp*>;
    using Map = std::map<Key, Value>;

    Map map;
    const Env* parent = nullptr;

    Value get(const Key& k) const;
};

Env init_env();

}
