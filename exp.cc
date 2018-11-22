/**
 * @file exp.cc
 */

#include <cctype>
#include <cstdlib>
#include <iostream>

#include "exp.h"

size_t next_token(const std::string& str, size_t start, size_t end)
{
    // Iterate over substring.
    for (auto i = start; i < end; ++i)
    {
        // Break at first non-whitespace character.
        if (!isspace(str[i]) && str[i] != ')')
        {
            return i;
        }
    }

    // Report no tokens.
    return std::string::npos;
}

size_t skip_token(const std::string& str, size_t start, size_t end)
{
    // Check for string.
    if (str[start] == '"')
    {
        return skip_string(str, start, end) + 1;
    }
    // Check for list.
    else if (str[start] == '(')
    {
        return skip_list(str, start, end) + 1;
    }
    // Treat as symbol.
    else
    {
        return skip_symbol(str, start, end) + 1;
    }
}

size_t skip_symbol(const std::string& str, size_t start, size_t end)
{
    for (auto i = start; i < end; ++i)
    {
        if (isspace(str[i]) || str[i] == '(' || str[i] == ')' || str[i] == '"')
        {
            return i;
        }
    }
    return end;

    // Report error.
    std::cerr << "Error parsing substring:\n"
              << str.substr(start, end - start)
              << "\nSymbol does not end.\n";
    std::exit(1);
}

size_t skip_string(const std::string& str, size_t start, size_t end)
{
    // Iterate over substring.
    for (auto i = start + 1; i < end; ++i)
    {
        if (str[i] == '\"' && str[i-1] != '\\')
        {
            return i;
        }

        // Newline characters are invalid.
        if (str[i] == '\n')
        {
            std::cerr << "Error parsing substring:\n"
                      << str.substr(start, end - start)
                      << "\nImproperly formatted string literal.\n";
            std::exit(1);
        }
    }

    // Report error.
    std::cerr << "Error parsing substring:\n"
              << str.substr(start, end - start)
              << "\nString literal does not terminate.\n";
    std::exit(1);
}

size_t skip_list(const std::string& str, size_t start, size_t end)
{
    size_t count = 1;

    for (auto i = start + 1; i < end; ++i)
    {
        if (str[i] == '(')
        {
            ++count;
        }
        else if (str[i] == ')')
        {
            --count;

            // Check if this is the end.
            if (count == 0)
            {
                return i;
            }
        }
    }

    // Report error.
    std::cerr << "Error parsing substring:\n"
              << str.substr(start, end - start)
              << "\nList does not terminate.\n";
    std::exit(1);
}

Exp::Exp(const std::string& str, size_t start, size_t end)
{
    // Find next token.
    size_t tok_start = next_token(str, start, end);
    if (tok_start == std::string::npos)
    {
        std::cerr << "No tokens available.\n";
        std::exit(1);
    }

    // Check if this is a string literal.
    if (str[tok_start] == '"')
    {
        size_t tok_end = skip_string(str, tok_start, end) - 1;

        // Check that this is the last element.
        if (next_token(str, tok_end + 2, end) != std::string::npos)
        {
            std::cerr << "Invalid s-expression.\n";
            std::exit(1);
        }

        // Copy data.
        type = Type::STRING;
        std::string* p = new std::string;
        *p = str.substr(tok_start + 1, tok_end - tok_start);
        data = p;
    }
    // Check if this is a list.
    else if (str[tok_start] == '(')
    {
        size_t list_end = skip_list(str, tok_start, end) + 1;

        // Check that this is the last element.
        if (next_token(str, list_end + 1, end) != std::string::npos)
        {
            std::cerr << "Invalid s-expression.\n";
            std::exit(1);
        }

        // Build list.
        type = Type::LIST;
        size_t pos = tok_start + 1;
        Exp* exp = this;
        while (exp != nullptr)
        {
            // Get next element.
            pos = next_token(str, pos, list_end);
            if (pos != std::string::npos)
            {
                size_t tok_end = skip_token(str, pos, list_end);

                // Check if this is the first element.
                if (data)
                {
                    exp->link = new Exp(str, pos, tok_end);
                    exp = exp->link;
                    pos = tok_end;
                }
                else
                {
                    data = new Exp(str, pos, tok_end);
                    exp = (Exp*)data;
                    pos = tok_end;
                }
            }
            else
            {
                exp = nullptr;
            }
        }
    }
    // Treat as symbol.
    else
    {
        size_t tok_end = skip_symbol(str, tok_start, end);

        // Check that this is the last element.
        if (next_token(str, tok_end + 1, end) != std::string::npos)
        {
            std::cerr << "Invalid s-expression.\n";
            std::exit(1);
        }

        if (tok_end - tok_start == 0)
        {
            std::cerr << "Empty symbol in substring:\n";
            std::cerr << str.substr(start, end);
            std::exit(1);
        }

        // Copy data.
        type = Type::SYMBOL;
        std::string* p = new std::string;
        *p = str.substr(tok_start, tok_end - tok_start);
        data = p;
    }
}

Exp::~Exp()
{
    // Delete data according to type.
    if (type == Type::NUMBER)
    {
        Number_Type* p = (Number_Type*)data;
        delete p;
    }
    else if (type == Type::BOOLEAN)
    {
        bool* p = (bool*)data;
        delete p;
    }
    else if (type == Type::STRING)
    {
        std::string* p = (std::string*)data;
        delete p;
    }
    else if (type == Type::SYMBOL)
    {
        std::string* p = (std::string*)data;
        delete p;
    }
    else if (type == Type::LIST)
    {
        Exp* p = (Exp*)data;
        delete p;
    }
    else if (type == Type::VOID)
    {
        // Do nothing.
    }
    else
    {
        std::cerr << "Attempt to delete unsupported type.\n";
        std::exit(1);
    }
}

void Exp::print() const
{
    // Print by type.
    if (type == Type::STRING)
    {
        std::string* p = (std::string*)data;
        std::cout << '"' << *p << '"';
    }
    else if (type == Type::SYMBOL)
    {
        std::string* p = (std::string*)data;
        std::cout << *p;
    }
    else if (type == Type::LIST)
    {
        std::cout << '(';

        // Print first element.
        Exp* p = (Exp*)data;
        if (p != nullptr)
        {
            p->print();
            p = p->link;
        }

        // Print additional elements.
        while (p != nullptr)
        {
            std::cout << ' ';
            p->print();
            p = p->link;
        }

        std::cout << ')';
    }
}
