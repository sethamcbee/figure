/**
 * @file lexer.cc
 */

#include "lexer.h"

namespace Figure
{

bool is_delim(char c)
{
    if (isspace(c) || (c == '(') || (c == ')') || (c == '"') || (c == '\'') || (c == ';'))
    {
        return true;
    }
    else
    {
        return false;
    }
}

}
