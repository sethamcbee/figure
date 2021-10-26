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

bool is_delim(Char c);
bool is_special(Char c);
bool is_id_start(Char c);
bool is_initial(Char c);
bool is_subsequent(Char c);
bool is_special_subsequent(Char c);

using Location = size_t;

struct LEOF {};
struct LeftParen {};
struct RightParen {};
struct Comma {};
struct CommaAt {};
struct Quote {};
struct Backtick {};
struct Dot {};

struct Token
{
    using Value = std::variant<
		LEOF, Bool, Char, Number, String, Id, LeftParen, RightParen,
        Comma, CommaAt, Quote, Backtick, Dot>;

    Value value;
    Location pos;

    Token(Value v, Location l)
    {
		value = v;
		pos = l;
	}

    operator Value&()
    {
		return value;
	}
    
    operator Value() const
    {
		return value;
	}
};

class Lexer
{
public:

	Location pos = 0;
	Char ch;
    std::string source;
    std::istream* input;
    std::list<Token> tokens;

    Lexer() {}

    Lexer(std::istream& s);

	void next_token();
    Token get_token();
    void push_token(const Token& tok);
    bool good();

    void print() {}

    void error()
    {
        error("Unspecified error.");
    }

    void error(std::string_view msg)
    {
        std::cerr << "Lexer error: " << msg << std::endl;
        exit(1);
    }
};

}
