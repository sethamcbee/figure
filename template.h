/**
 * @file template.h
 */

#pragma once

#include "exp.h"

namespace Figure
{

struct Template : public Exp
{};

struct TemplateId : public Template
{
    Id value;

    TemplateId(const Id& v);

    void print(std::ostream& o) const;
};

struct TemplateString : public Template
{
    String value;

    TemplateString(const String& v);

    void print(std::ostream& o) const;
};

struct TemplateChar : public Template
{
    Char value;

    TemplateChar(const Char& v);

    void print(std::ostream& o) const;
};

struct TemplateBool : public Template
{
    Bool value;

    TemplateBool(const Bool& v);

    void print(std::ostream& o) const;
};

struct TemplateNumber : public Template
{
    Number value;

    TemplateNumber(const Number& v);

    void print(std::ostream& o) const;
};

struct TemplateList : public Template
{
    std::list<Ref<Template>> value;

    TemplateList(Env& env, const Datum& datum);

    void print(std::ostream& o) const;
};

struct TemplateEllipsesList : public Template
{
    std::list<Ref<Template>> before;
    std::list<Ref<Template>> after;

    TemplateEllipsesList(Env& env, const Datum& datum);

    void print(std::ostream& o) const;
};

Ref<Template> make_template(Env& env, const Datum& datum);

}
