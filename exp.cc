/**
 * @file exp.cc
 */

#include <utility>

#include "exp.h"
#include "sexp.h"

namespace Figure
{

Exp::Exp(ExpVal e) : value(e)
{}

Exp::Exp(Sexp src) : source(src)
{
    if (auto s = std::get_if<Bool>(&src.value))
    {
        value = Bool{*s};
    }
    else if (auto s = std::get_if<Number>(&src.value))
    {
        value = Number{*s};
    }
    else if (auto s = std::get_if<Char>(&src.value))
    {
        value = Char{*s};
    }
    else if (auto s = std::get_if<String>(&src.value))
    {
        value = String{*s};
    }
    else if (auto s = std::get_if<Id>(&src.value))
    {
        value = Id{*s};
    }
    else if (auto s = std::get_if<SexpList>(&src.value))
    {
        value = Id{"STUB"};
    }
    else
    {
        error("Invalid s-exp type.");
    }
}

ExpVal Exp::eval()
{
    if (auto e = std::get_if<Bool>(&value))
    {
        return Bool{*e};
    }
    else if (auto e = std::get_if<Number>(&value))
    {
        return Number{*e};
    }
    else if (auto e = std::get_if<Char>(&value))
    {
        return Char{*e};
    }
    else if (auto e = std::get_if<String>(&value))
    {
        return String{*e};
    }
    else if (auto e = std::get_if<Id>(&value))
    {
        return Id{*e};
    }
    else
    {
        error("Invalid exp type.");
    }
}

const void Exp::print()
{
    if (auto v = std::get_if<Bool>(&value))
    {
        std::cout << v->value;
    }
    else if (auto v = std::get_if<Number>(&value))
    {
        std::cout << v->value;
    }
    else if (auto v = std::get_if<Char>(&value))
    {
        std::cout << v->value;
    }
    else if (auto v = std::get_if<String>(&value))
    {
        std::cout << "\"" << v->value << "\"";
    }
    else if (auto v = std::get_if<Id>(&value))
    {
        std::cout << v->value;
    }
    else if (auto v = std::get_if<List>(&value))
    {
        auto last = v->end();
        prev(last);
    }
}

void Exp::error()
{
    error("Unspecified error.");
}

void Exp::error(std::string_view msg)
{
    std::cerr << "exp parsing error: " << msg << std::endl;
    exit(1);
}

};
