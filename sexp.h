/**
 * @file sexp.h
 */

#pragma once

#include <list>
#include <variant>

#include "lexer.h"
#include "literal.h"

namespace Figure
{

struct Sexp;
using SexpList = std::list<Sexp>;

struct Sexp
{
	using Value = std::variant<LEOF, Bool, Char, Number, String, Id, SexpList>;

	Value value;
	Location pos;

	Sexp(Value v, Location p);
	Sexp(Lexer& l);

	void print() const;

	void error();
	void error(std::string_view m);
};

}
