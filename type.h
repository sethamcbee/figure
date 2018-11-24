/**
 * @file type.h
 */

#ifndef TYPE_H
#define TYPE_H

#include <functional>
#include <memory>
#include <stack>
#include <string>
#include <utility>
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

// Expressions to be evaluated, with their matching environment.
typedef std::pair<std::shared_ptr<Env>, std::shared_ptr<Exp>> Task;

typedef double Number_Type;
typedef std::function
<std::shared_ptr<Exp>
(Env& env,
 std::vector<std::shared_ptr<Exp>>&)>
Native_Function;

#endif // TYPE_H
