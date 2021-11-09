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
            // Ellipses not supported.
            return nullptr;
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
