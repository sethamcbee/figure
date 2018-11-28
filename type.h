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
    SPECIAL_FORM,
    NATIVE_FUNCTION
};

// Expressions to be evaluated, with their matching environment.
class Task
{
public:

    /// Current execution environment.
    std::shared_ptr<Env> env = nullptr;

    /// Expression being evaluated.
    std::shared_ptr<Exp> exp = nullptr;

    /// Alias for if the first element of a list needs evaluation.
    std::shared_ptr<Exp> alias = nullptr;

    /// Arguments to a function application or special form.
    std::vector<std::shared_ptr<Exp>> args;

    /// Where to put the result of this evaluation.
    std::shared_ptr<Exp>* result = nullptr;
};

typedef double Number_Type;
typedef std::function<void(std::stack<std::shared_ptr<Task>>&)> Special_Form;
typedef std::function<void(std::stack<std::shared_ptr<Task>>&)> Native_Function;

#endif // TYPE_H
