/**
 * @file sexp.cc
 */

#include <variant>

#include "lexer.h"
#include "sexp.h"

namespace Figure
{

Sexp::Sexp(Value v, Location p)
{
    value = v;
    pos = p;
}

Sexp::Sexp(Lexer& l)
{
    Token tok = l.get_token();
    pos = tok.pos;
    if (auto id = std::get_if<Id>(&tok.value))
    {
        value = *id;
    }
    else if (auto b = std::get_if<Bool>(&tok.value))
    {
        value = *b;
    }
    else if (auto n = std::get_if<Number>(&tok.value))
    {
        value = *n;
    }
    else if (auto c = std::get_if<Char>(&tok.value))
    {
        value = *c;
    }
    else if (auto s = std::get_if<String>(&tok.value))
    {
        value = *s;
    }
    else if (std::get_if<LeftParen>(&tok.value))
    {
        value = SexpList{};
        SexpList& list = std::get<SexpList>(value);
        tok = l.get_token();
        while (!std::get_if<RightParen>(&tok.value))
        {
            l.push_token(tok);
            Sexp element{l};
            list.push_back(element);
            tok = l.get_token();
        }
    }
    else if (std::get_if<VectorParen>(&tok.value))
    {
        value = SexpVector{};
        SexpVector& vec = std::get<SexpVector>(value);
        SexpList& list = vec.value;
        tok = l.get_token();
        while (!std::get_if<RightParen>(&tok.value))
        {
            l.push_token(tok);
            Sexp element{l};
            list.push_back(element);
            tok = l.get_token();
        }
    }
    else if (std::get_if<Quote>(&tok.value))
    {
        value = SexpList{};
        SexpList& list = std::get<SexpList>(value);
        Sexp quote{Id{"quote"}, pos};
        list.push_back(quote);
        Sexp quoted{l};
        list.push_back(quoted);
    }
    else if (std::get_if<Backtick>(&tok.value))
    {
        value = SexpList{};
        SexpList& list = std::get<SexpList>(value);
        Sexp quasiquote{Id{"quasiquote"}, pos};
        list.push_back(quasiquote);
        Sexp datum{l};
        list.push_back(datum);
    }
    else if (std::get_if<Comma>(&tok.value))
    {
        value = SexpList{};
        SexpList& list = std::get<SexpList>(value);
        Sexp unquote{Id{"unquote"}, pos};
        list.push_back(unquote);
        Sexp datum{l};
        list.push_back(datum);
    }
    else if (std::get_if<CommaAt>(&tok.value))
    {
        value = SexpList{};
        SexpList& list = std::get<SexpList>(value);
        Sexp splice{Id{"unquote-splicing"}, pos};
        list.push_back(splice);
        Sexp datum{l};
        list.push_back(datum);
    }
    else if (std::get_if<Dot>(&tok.value))
    {
        value = Id{"."};
    }
    else
    {
        error("Invalid datum.\n");
    }
}

void Sexp::print() const
{
    if (auto id = std::get_if<Id>(&value))
    {
        std::cout << id->value;
    }
    else if (auto b = std::get_if<Bool>(&value))
    {
        std::cout << b->value;
    }
    else if (auto n = std::get_if<Number>(&value))
    {
        std::cout << n->value;
    }
    else if (auto c = std::get_if<Char>(&value))
    {
        std::cout << c->value;
    }
    else if (auto s = std::get_if<String>(&value))
    {
        std::cout << "\"" << s->value << "\"";
    }
    else if (auto l = std::get_if<SexpList>(&value))
    {
        std::cout << "(";
        const char* space = "";
        for (const auto& sexp : *l)
        {
            std::cout << space;
            sexp.print();
            space = " ";
        }
        std::cout << ")";
    }
    else if (auto v = std::get_if<SexpVector>(&value))
    {
        const auto& l = v->value;
        std::cout << "#(";
        const char* space = "";
        for (const auto& sexp : l)
        {
            std::cout << space;
            sexp.print();
            space = " ";
        }
        std::cout << ")";
    }
}

void Sexp::error()
{
    error("Unspecified error.");
}

void Sexp::error(std::string_view msg)
{
    std::cerr << "\nAt character: " << pos << std::endl;
    std::cerr << "S-exp parsing error: " << msg << std::endl;
    exit(1);
}

}
