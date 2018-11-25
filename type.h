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
class Task
{
public:

    /// Task to forward return values to.
    Task* parent = nullptr;
    std::shared_ptr<Env> env;
    std::shared_ptr<Exp> exp;
    std::vector<std::shared_ptr<Exp>> args;
};

typedef double Number_Type;
typedef std::function<void(std::stack<Task>&)> Native_Function;

#endif // TYPE_H
