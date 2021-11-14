/**
 * @file template.cc
 */

#include "env.h"
#include "template.h"

namespace Figure
{

TemplateId::TemplateId(const Id& v)
{
    value = v;
}

void TemplateId::print(std::ostream& o) const
{
    o << value;
}

TemplateString::TemplateString(const String& v)
{
    value = v;
}

void TemplateString::print(std::ostream& o) const
{
    o << value;
}

TemplateChar::TemplateChar(const Char& v)
{
    value = v;
}

void TemplateChar::print(std::ostream& o) const
{
    o << value;
}

TemplateBool::TemplateBool(const Bool& v)
{
    value = v;
}

void TemplateBool::print(std::ostream& o) const
{
    o << value;
}

TemplateNumber::TemplateNumber(const Number& v)
{
    value = v;
}

void TemplateNumber::print(std::ostream& o) const
{
    o << value;
}

TemplateList::TemplateList(Env& env, const Datum& datum)
{
    const auto& l = std::get<DatumList>(datum.value);
    for (const auto& d : l)
    {
        auto tmpl = make_template(env, d);
        value.push_back(tmpl);
    }
}

void TemplateList::print(std::ostream& o) const
{
    o << "(";
    const char* space = "";
    for (const auto& tmpl : value)
    {
        o << space;
        tmpl->print(o);
        space = " ";
    }
    o << ")";
}

TemplateEllipsesList::TemplateEllipsesList(Env& env, const Datum& datum)
{
    std::list<Datum>::const_iterator ellipses;
    const auto& l = std::get<DatumList>(datum.value);
    for (auto d = l.begin(); d != l.end(); ++d)
    {
        if (auto id = std::get_if<Id>(&d->value))
        {
            if (id->value == "...")
            {
                ellipses = d;
                break;
            }
        }
        auto tmpl = make_template(env, *d);
        before.push_back(tmpl);
    }
    for (auto d = std::next(ellipses); d != l.end(); ++d)
    {
        auto tmpl = make_template(env, *d);
        after.push_back(tmpl);
    }
}

void TemplateEllipsesList::print(std::ostream& o) const
{
    o << "(";
    const char* space = "";
    for (const auto& tmpl : before)
    {
        o << space;
        tmpl->print(o);
        space = " ";
    }
    o << space << "...";
    for (const auto& tmpl : after)
    {
        o << space;
        tmpl->print(o);
    }
    o << ")";
}

Ref<Template> make_template(Env& env, const Datum& datum)
{
    if (auto id = std::get_if<Id>(&datum.value))
    {
        if (id->value != "...")
        {
            TemplateId tmp{*id};
            return make_ref(tmp);
        }
        else
        {
            return nullptr;
        }
    }
    else if (auto str = std::get_if<String>(&datum.value))
    {
        TemplateString tmp{*str};
        return make_ref(tmp);
    }
    else if (auto ch = std::get_if<Char>(&datum.value))
    {
        TemplateChar tmp{*ch};
        return make_ref(tmp);
    }
    else if (auto b = std::get_if<Bool>(&datum.value))
    {
        TemplateBool tmp{*b};
        return make_ref(tmp);
    }
    else if (auto n = std::get_if<Number>(&datum.value))
    {
        TemplateNumber tmp{*n};
        return make_ref(tmp);
    }
    else if (auto lp = std::get_if<DatumList>(&datum.value))
    {
        const auto& l = *lp;

        DatumList::const_iterator ellipses = l.end();
        for (auto e = l.begin(); e != l.end(); ++e)
        {
            if (auto id = std::get_if<Id>(&e->value))
            {
                if (id->value == "...")
                {
                    ellipses = e;
                }
            }
        }
        if (ellipses != l.end())
        {
            TemplateEllipsesList tmp{env, datum};
            return make_ref(tmp);
        }
        else
        {
            TemplateList tmp{env, datum};
            return make_ref(tmp);
        }
    }
    else
    {
        std::cerr << "Invalid template.\n";
        exit(1);
    }
}


}
