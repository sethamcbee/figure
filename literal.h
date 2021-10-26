/**
 * @file literal.h
 */

#pragma once

#include <string>

namespace Figure
{

struct Bool
{
	using Value = bool;
	
    Value value;
    
    operator Value&()
    {
		return value;
	}
    
    operator Value() const
    {
		return value;
	}
};

struct Number
{
	using Value = double;
	
    Value value;

    operator Value&()
    {
		return value;
	}

	operator Value() const
	{
		return value;
	}
};

struct Char
{
	using Value = char;
	
    Value value;

    operator Value&()
    {
		return value;
	}

    operator Value() const
    {
		return value;
	}
};

struct String
{
	using Value = std::string;
	
    Value value;

    operator Value&()
    {
		return value;
	}

	operator Value() const
	{
		return value;
	}
};

struct Id
{
	using Value = std::string;
	 
    Value value;

    operator Value&()
    {
		return value;
	}

	operator Value()
	{
		return value;
	}
};

}
