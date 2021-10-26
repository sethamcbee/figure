/**
 * @file sexp.h
 */

#pragma once

#include <cstdlib>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <string_view>
#include <variant>

#include "lexer.h"

namespace Figure
{

class Sexp;
using SexpPtr = std::unique_ptr<Sexp>;
using SexpList = std::list<Sexp>;

class Sexp
{
public:

    std::vector<Token> source;
    std::variant<Bool, Char, Number, String, Id, Dot, SexpList> value;

    Sexp() {}

    Sexp(auto start, auto end) : source{start, end}
    {
        // Check for single element.
        if (std::next(start) == end)
        {
            if (auto tok = std::get_if<Bool>(&start->value))
            {
                value = Bool{tok->value};
            }
            else if (auto tok = std::get_if<Char>(&start->value))
            {
                value = Char{tok->value};
            }
            else if (auto tok = std::get_if<Number>(&start->value))
            {
                value = Number{tok->value};
            }
            else if (auto tok = std::get_if<String>(&start->value))
            {
                value = String{tok->value};
            }
            else if (auto tok = std::get_if<Id>(&start->value))
            {
                value = Id{tok->value};
            }
            else if (auto tok = std::get_if<Dot>(&start->value))
            {
                value = Dot{};
            }
            else
            {
                error("Invalid element.");
            }
        }
        else
        {
            if (auto first = std::get_if<Dot>(&start->value))
            {
                error("Dot notation not supported.");
            }
            else if (auto first = std::get_if<LeftParen>(&start->value))
            {
                auto last = std::prev(end);
                if (auto right_paren = std::get_if<RightParen>(&last->value))
                {
                    SexpList l;
                    auto it{std::next(start)};
                    while (it != last)
                    {
                        if (auto tok = std::get_if<LeftParen>(&it->value))
                        {
                            // Find matching parenthesis.
                            auto prev{it};
                            int balance{-1};
                            do
                            {
                                ++it;
                                if (it == last)
                                {
                                    error("Mismatched parenthesis.");
                                }

                                if (auto par = std::get_if<LeftParen>(&it->value))
                                {
                                    --balance;
                                }
                                else if (auto par = std::get_if<RightParen>(&it->value))
                                {
                                    ++balance;
                                }
                            }
                            while (balance != 0);
                            l.push_back(Sexp{prev, std::next(it)});
                        }
                        else
                        {
                            l.push_back(Sexp{it, std::next(it)});
                        }

                        ++it;
                    }
                    value = l;
                }
                else
                {
                    error("Mismatched parenthesis.");
                }
            }
            else
            {
                error("Parenthesis expected to initialize list.");
            }
        }
    }

    void print()
    {
        if (auto val = std::get_if<Bool>(&value))
        {
            std::cout << val->value;
        }
        else if (auto val = std::get_if<Char>(&value))
        {
            std::cout << val->value;
        }
        else if (auto val = std::get_if<Number>(&value))
        {
            std::cout << val->value;
        }
        else if (auto val = std::get_if<String>(&value))
        {
            std::cout << "\"" << val->value << "\"";
        }
        else if (auto val = std::get_if<Id>(&value))
        {
            std::cout << val->value;
        }
        else if (auto val = std::get_if<Dot>(&value))
        {
            std::cout << ".";
        }
        else if (auto val = std::get_if<SexpList>(&value))
        {
            std::cout << "(";
            auto end = std::prev(val->end());
            for (auto it = val->begin(); it != end; ++it)
            {
                it->print();
                std::cout << " ";
            }
            if (val->size() > 0)
            {
                end->print();
            }
            std::cout << ")";
        }
    }

    void error()
    {
        error("Unspecified error.");
    }

    void error(std::string_view msg)
    {
        std::cerr << "s-exp parsing error: " << msg << std::endl;
        exit(1);
    }
};

}
