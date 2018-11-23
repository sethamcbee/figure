/**
 * @file type.h
 */

#ifndef TYPE_H
#define TYPE_H

#include <functional>
#include <string>
#include <vector>

class Env;
class Exp;

/// Defines a fundamental data type identifier.
enum class Type
{
    VOID,
    NUMBER,
    BOOLEAN,
    SYMBOL,
    STRING,
    LAMBDA,
    LIST,
    NATIVE_FUNCTION
};

typedef double Number_Type;
typedef std::function<Exp*(Env& env, std::vector<Exp*>&)> Native_Function;

#endif // TYPE_H
