/**
 * @file literal.cc
 */

#include "literal.h"
#include "lexer.h"

namespace Figure
{

Bool::operator Value&()
{
	return value;
}

Bool::operator Value() const
{
	return value;
}

Number::operator Value&()
{
	return value;
}

Number::operator Value() const
{
	return value;
}

Char::operator Value&()
{
	return value;
}

Char::operator Value() const
{
	return value;
}

String::operator Value&()
{
	return value;
}

String::operator Value() const
{
	return value;
}

Id::operator Value&()
{
	return value;
}

Id::operator Value() const
{
	return value;
}

}
