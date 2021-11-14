/**
 * @file pattern.h
 */

#pragma once

#include "exp.h"

namespace Figure
{

struct Pattern : public Exp
{
};

struct PatternId : public Pattern
{
    Id value;

    PatternId(const Id& v);

    void print(std::ostream& o) const;
};

struct PatternString : public Pattern
{
    String value;

    PatternString(const String& v);

    void print(std::ostream& o) const;
};

struct PatternChar : public Pattern
{
    Char value;

    PatternChar(const Char& v);

    void print(std::ostream& o) const;
};

struct PatternBool : public Pattern
{
    Bool value;

    PatternBool(const Bool& v);

    void print(std::ostream& o) const;
};

struct PatternNumber : public Pattern
{
    Number value;

    PatternNumber(const Number& v);

    void print(std::ostream& o) const;
};

struct PatternUnderscore : public Pattern
{
    void print(std::ostream& o) const;
};

struct PatternList : public Pattern
{
    std::list<Ref<Pattern>> value;

    PatternList(Env& env, const Datum& datum);

    void print(std::ostream& o) const;
};

struct PatternEllipsesList : public Pattern
{
    std::list<Ref<Pattern>> before;
    std::list<Ref<Pattern>> after;

    PatternEllipsesList(Env& env, const Datum& datum);

    void print(std::ostream& o) const;
};

struct PatternImproper : public Pattern
{
};

Ref<Pattern> make_pattern(Env& env, const Datum& datum);

}