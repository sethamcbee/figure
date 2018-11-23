/**
 * @file env.h
 * @brief Defines an environment, which stores symbol bindings in the
 * current scope.
 */

#ifndef ENV_H
#define ENV_H

#include <map>
#include <memory>
#include <string>

#include "exp.h"

class Env
{
public:

    /// Get the value of a bound symbol.
    const Exp& get(const std::string& name);

    /// Bind a symbol to a value in the current scope.
    void let(const std::string& name, const Exp& val);

    /// Spawn a new environment, with this as the parent environment.
    std::unique_ptr<Env> spawn();

private:

    /// Map of symbols at the current scope.
    std::map<std::string, Exp> symbols;

    /// Pointer to parent scope's environment. Allows efficient
    /// creation and deletion of new scopes, as well as the
    /// easy rebinding of symbols in a given scope.
    Env* parent = nullptr;
};

#endif // ENV_H
