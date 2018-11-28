/**
 * @file env.h
 * @brief Defines an environment, which stores symbol bindings in the
 * current scope.
 */

#ifndef ENV_H
#define ENV_H

#include <functional>
#include <map>
#include <memory>
#include <string>

#include "type.h"

class Exp;

class Env : public std::enable_shared_from_this<Env>
{
public:

    /// Check whether a symbol is bound.
    bool is_bound(const std::string& name);

    /// Get the value of a bound symbol.
    std::shared_ptr<Exp> get(const std::string& name);

    /// Bind a symbol to a value in the current scope.
    void let(const std::string& name, std::shared_ptr<Exp> val);

    /// Bind a new native function.
    void builtin(const std::string& name, const Native_Function& fn);

    /// Bind a new special form.
    void special_form(const std::string& name, const Special_Form& fn);

    /// Spawn a new environment, with this as the parent environment.
    std::shared_ptr<Env> spawn();

    /// Pointer to parent scope's environment. Allows efficient
    /// creation and deletion of new scopes, as well as the
    /// easy rebinding of symbols in a given scope.
    std::shared_ptr<Env> parent = nullptr;

private:

    /// Map of symbols at the current scope.
    std::map<std::string, std::shared_ptr<Exp>> symbols;
};

#endif // ENV_H
