/**
 * @file exp.h
 */

#ifndef EXP_H
#define EXP_H

#include <string>

#include "type.h"

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

class Exp
{
public:

    /// Parse a string to build an s-expression.
    Exp(const std::string& str, size_t start, size_t end);

    ~Exp();

    /// Print the contents of an expression.
    void print() const;

    Type type = Type::VOID;

    /// Pointer to the next list entry.
    Exp* link = nullptr;

    void* data = nullptr;
};

#endif // EXP_H
