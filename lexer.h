/**
 * @file lexer.h
 */

#pragma once

#include <cctype>
#include <cstdlib>
#include <iostream>
#include <list>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "literal.h"

namespace Figure
{

struct LeftParen {};
struct RightParen {};
struct Comma {};
struct CommaAt {};
struct Quote {};
struct Backtick {};
struct Dot {};

class Token
{
public:

    char* character = 0;
    std::string source;
    std::variant
        <Bool, Char, Number, String, Id, LeftParen, RightParen,
        Comma, CommaAt, Quote, Backtick, Dot> value;

    Token() {}

    Token(std::string_view src, auto start, auto end) : character{&*start}, source{start, end}
    {
        // Check for string literal.
        if (*start == '"')
        {
            value = String{std::string{start + 1, end - 1}};
        }
        // Check for number.
        else if (isdigit(*start))
        {
            value = Number{std::stod(source)};
        }
        // Check for booleans.
        else if (source == "#t" || source == "#true")
        {
            value = Bool{true};
        }
        else if (source == "#f" || source == "#false")
        {
            value = Bool{false};
        }
        // Check for special symbols.
        else if (source == "(")
        {
            value = LeftParen{};
        }
        else if (source == ")")
        {
            value = RightParen{};
        }
        else if (source == ",")
        {
            value = Comma{};
        }
        else if (source == ",@")
        {
            value = CommaAt{};
        }
        else if (source == "'")
        {
            value = Quote{};
        }
        else if (source == "`")
        {
            value = Backtick{};
        }
        else if (source == ".")
        {
            value = Dot{};
        }
        // Else, treat as identifier.
        else
        {
            value = Id{source};
        }

        // TODO: Implement characters, verticle lines, and others.
    }
};

bool is_delim(char c);

class Lexer
{
public:

    std::string source;
    std::list<Token> tokens;

    Lexer() {}

    Lexer(std::string_view src) : source{src}
    {
        auto start = source.begin();
        auto prev = start;
        auto next = start;
        auto end = source.end();

        while (next < end)
        {
            // Skip over delimiters.
            while (is_delim(*next) && next < end)
            {
                // Skip over comments.
                if (*next == ';')
                {
                    while (*next != '\n' && next < end)
                    {
                        ++next;
                    }
                    prev = next;
                }

                // Skip over whitespace.
                while (isspace(*next) && next < end)
                {
                    ++next;
                }
                prev = next;

                // Quotes are tokens.
                if (*next == '\'')
                {
                    ++next;
                    auto tok = Token{source, prev, next};
                    tokens.push_back(tok);
                }
                prev = next;

                // Parenthesis are tokens.
                if (*next == '(' || *next == ')')
                {
                    ++next;
                    auto tok = Token{source, prev, next};
                    tokens.push_back(tok);
                }
                prev = next;

                // Strings are tokens.
                if (*next == '"')
                {
                    while (next < end)
                    {
                        ++next;
                        if (*next == '"' && *std::prev(next, 1) != '\\')
                        {
                            ++next;
                            break;
                        }
                    }
                    auto tok = Token{source, prev, next};
                    tokens.push_back(tok);
                }
                prev = next;
            }

            // Check if there are any tokens left.
            if (prev < end)
            {
                // Token found, get all characters.
                do
                {
                    ++next;
                }
                while (!is_delim(*next) && next <= end);

                auto tok = Token{source, prev, next};
                tokens.push_back(tok);

                prev = next;
            }
        }

        // Perform quote substitution.
        for (auto it = tokens.begin(); it != tokens.end(); ++it)
        {
            size_t quote_n{0};
            while (auto quote = std::get_if<Quote>(&it->value))
            {
                static std::string lp_src{"("};
                static std::string q_src{"quote"};

                ++quote_n;
                it = tokens.erase(it);

                Token lp{lp_src, lp_src.begin(), lp_src.end()};
                tokens.insert(it, lp);

                Token q{q_src, q_src.begin(), q_src.end()};
                it = tokens.insert(it, q);
                ++it;
            }

            if (quote_n > 0)
            {
                auto next_run = it;

                // Find matching parenthesis.
                if (auto lp = std::get_if<LeftParen>(&it->value))
                {
                    int balance{-1};
                    do
                    {
                        ++it;
                        if (it == tokens.end())
                        {
                            error();
                        }

                        if (std::get_if<LeftParen>(&it->value))
                        {
                            --balance;
                        }

                        if (std::get_if<RightParen>(&it->value))
                        {
                            ++balance;
                        }
                    }
                    while (balance != 0);
                }

                while (quote_n > 0)
                {
                    static std::string rp_src{")"};

                    ++it;
                    --quote_n;
                    Token rp{rp_src, rp_src.begin(), rp_src.end()};
                    it = tokens.insert(it, rp);
                }

                it = next_run;
            }
        }
    }

    void print()
    {
        for (auto tok : tokens)
        {
            std::cout << tok.source << " ";
        }
    }

    void error()
    {
        error("Unspecified error.");
    }

    void error(std::string_view msg)
    {
        std::cerr << "lexer error:\n" << msg << std::endl;
        exit(1);
    }
};

}
