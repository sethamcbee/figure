/**
 * @file exp.cc
 */

#include <cctype>
#include <cstdlib>
#include <iostream>

#include "env.h"
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

Exp::Exp()
{
    type = Type::VOID;
    data = nullptr;
    link = nullptr;
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
        std::string* p = new std::string;
        *p = str.substr(tok_start, tok_end - tok_start);
        type = Type::SYMBOL;

        // Check if this symbol forms a numeric literal.
        char* num_end;
        double num = std::strtod(p->c_str(), &num_end);
        if (num_end == p->c_str() + p->length())
        {
            delete p;

            type = Type::NUMBER;
            Number_Type* d = new Number_Type;
            *d = num;
            data = d;
        }
        else
        {
            data = p;
        }
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
    else if (type == Type::NATIVE_FUNCTION)
    {
        Native_Function* p = (Native_Function*)data;
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

Exp* Exp::eval(Env& env)
{
    // Return atoms as-is.
    if (type == Type::STRING)
    {
        return this;
    }
    else if (type == Type::BOOLEAN)
    {
        return this;
    }
    else if (type == Type::NUMBER)
    {
        return this;
    }
    else if (type == Type::VOID)
    {
        return this;
    }

    // Evaluate symbols, functions, and lists.
    if (type == Type::SYMBOL)
    {
        std::string* sym = (std::string*)data;
        const Exp* ret = &env.get(*sym);
        return (Exp*)ret;
    }
    if (type == Type::NATIVE_FUNCTION)
    {
        return this;
    }
    else if (type == Type::LIST)
    {
        Exp* first = (Exp*)data;

        // Check if this is a function application.
        Exp* op = first->eval(env);
        if (op->type == Type::NATIVE_FUNCTION)
        {
            // Check for built-in function.
            Native_Function& fn = op->get_native_function();
            std::vector<Exp*> args;
            Exp* next_arg = first->link;
            while (next_arg)
            {
                args.push_back(next_arg);
                next_arg = next_arg->link;
            }
            return fn(env, args);
        }

        // Else, evaluate and return the first argument.
        return op;
    }
    else
    {
        std::cerr << "Unsupported evaluation.\n";
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
    else if (type == Type::NUMBER)
    {
        Number_Type* p = (Number_Type*)data;
        std::cout << *p;
    }
    else if (type == Type::BOOLEAN)
    {
        bool* p = (bool*)data;
        std::cout << *p;
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

const std::string& Exp::get_string() const
{
    if (type == Type::STRING || type == Type::SYMBOL)
    {
        const std::string* p = (const std::string*)data;
        return *p;
    }
    else
    {
        std::cerr << "Error: Attempt to treat non-string type as string.\n";
        std::exit(1);
    }
}

Number_Type Exp::get_number() const
{
    if (type == Type::NUMBER)
    {
        const Number_Type* p = (const Number_Type*)data;
        return *p;
    }
    else
    {
        std::cerr << "Error: Attempt to treat non-number type as number.\n";
        std::exit(1);
    }
}

bool Exp::get_bool() const
{
    if (type == Type::BOOLEAN)
    {
        const bool* p = (const bool*)data;
        return *p;
    }
    else
    {
        std::cerr << "Error: Attempt to treat non-boolean type as boolean.\n";
        std::exit(1);
    }
}

Native_Function& Exp::get_native_function() const
{
    if (type == Type::NATIVE_FUNCTION)
    {
        Native_Function* p = (Native_Function*)data;
        return *p;
    }
    else
    {
        std::cerr << "Error: Invalid attempt to treat object as a native function.\n";
        std::exit(1);
    }
}
