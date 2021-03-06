/**
 * @file exp.h
 */

#ifndef EXP_H
#define EXP_H

#include <string>

#include "type.h"

class Env;
class Lambda;

/// Find the start of the next symbol, string, or list.
size_t next_token(const std::string& str, size_t start, size_t end);

/// Skip to end of a token.
size_t skip_token(const std::string& str, size_t start, size_t end);

/// Skip to end of a symbol.
size_t skip_symbol(const std::string& str, size_t start, size_t end);

/// Skip to end of string literal.
size_t skip_string(const std::string& str, size_t start, size_t end);

/// Skip to end of a list.
size_t skip_list(const std::string& str, size_t start, size_t end);

class Exp : public std::enable_shared_from_this<Exp>
{
public:

    /// Spawn an Exp.
    static std::shared_ptr<Exp> spawn();

    /// Parse a string to build an s-expression.
    void parse(const std::string& str, size_t start, size_t end);

    ~Exp();

    /// Print the contents of an expression.
    void print() const;

    /// Return whether this expression is fully evaluated.
    bool self_eval() const;

    /// Remaining length of the list, past this element.
    size_t length() const;

    /// Get a string from this expression.
    const std::string& get_string() const;

    /// Get a number from this expression.
    Number_Type get_number() const;

    /// Get a boolean from this expression.
    bool get_bool() const;

    /// Get a native function from this expression.
    Native_Function& get_native_function() const;

    /// Get a special form evaluator from this expression.
    Special_Form& get_special_form() const;

    /// Get a lambda from this expression.
    Lambda& get_lambda() const;

    /// Get the first element of a list.
    std::shared_ptr<Exp> get_list() const;

    Type type = Type::VOID;

    /// Pointer to the next list entry.
    std::shared_ptr<Exp> link = nullptr;

    void* data = nullptr;

private:

    Exp();
};

#endif // EXP_H
