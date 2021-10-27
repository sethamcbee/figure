/**
 * @file literal.h
 */

#pragma once

#include <string>

namespace Figure
{

struct Lexer;

struct Bool
{
	using Value = bool;
	
    Value value;
    
    operator Value&();
    
    operator Value() const;
};

struct Number
{
	using Value = double;
	
    Value value;

    operator Value&();

	operator Value() const;
};

struct Char
{
	using Value = char;
	
    Value value;

    operator Value&();

    operator Value() const;
};

struct String
{
	using Value = std::string;
	
    Value value;

    operator Value&();

	operator Value() const;
};

struct Id
{
	using Value = std::string;
	 
    Value value;

    operator Value&();

	operator Value() const;
};

}
